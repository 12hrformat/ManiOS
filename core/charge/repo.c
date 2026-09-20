/*
 * repo.c — repository handling for charge.
 *
 *   /var/lib/charge/repos.conf   one repo base URL per line
 *   /var/lib/charge/index.txt    combined index from `charge update`
 *   /var/lib/charge/pkgs/        downloaded .tar.zst cache
 *
 * A repo is just an HTTP folder: `<base>/index.txt` + `<base>/<file>.tar.zst`.
 * Git polling GitHub Pages or InfinityFree works with zero config.
 */

#include "charge.h"

#include <curl/curl.h>
#include <openssl/sha.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define REPOS_CONF DB_ROOT "/repos.conf"
#define CACHE_DIR  DB_ROOT "/pkgs"
#define COMBINED   DB_ROOT "/index.txt"

static FILE *open_db_file(const char *path, const char *mode) {
  char *dir = NULL;
  const char *slash = strrchr(path, '/');
  if (slash) {
    size_t n = (size_t)(slash - path);
    dir = malloc(n + 1);
    memcpy(dir, path, n);
    dir[n] = '\0';
    mkdir_p(dir);
    free(dir);
  }
  return fopen(path, mode);
}

/* ---- repos.conf management ---- */

int repos_list(void) {
  FILE *f = fopen(REPOS_CONF, "r");
  if (!f) {
    fprintf(stderr, "charge: no repos configured. `charge repos add <url>`\n");
    return CHARGE_OK;
  }
  char line[1024];
  printf("configured repos:\n");
  while (fgets(line, sizeof line, f)) {
    size_t n = strlen(line);
    while (n && (line[n-1] == '\n' || line[n-1] == '\r')) line[--n] = '\0';
    if (line[0] == '\0' || line[0] == '#') continue;
    printf("  %s\n", line);
  }
  fclose(f);
  return CHARGE_OK;
}

int repos_add(const char *url) {
  FILE *f = open_db_file(REPOS_CONF, "a");
  if (!f) { perror("charge: cannot write repos.conf"); return CHARGE_ERR; }
  fprintf(f, "%s\n", url);
  fclose(f);
  printf("charge: added repo %s\n", url);
  return CHARGE_OK;
}

int repos_remove(const char *url) {
  FILE *in = fopen(REPOS_CONF, "r");
  if (!in) { fprintf(stderr, "charge: no repos configured\n"); return CHARGE_ERR; }

  char *tmp = malloc(strlen(REPOS_CONF) + 8);
  sprintf(tmp, "%s.tmp", REPOS_CONF);
  FILE *out = fopen(tmp, "w");

  char line[1024];
  int removed = 0;
  while (fgets(line, sizeof line, in)) {
    size_t n = strlen(line);
    while (n && (line[n-1] == '\n' || line[n-1] == '\r')) line[--n] = '\0';
    if (strcmp(line, url) == 0) { removed = 1; continue; }
    fprintf(out, "%s\n", line);
  }
  fclose(in);
  fclose(out);
  rename(tmp, REPOS_CONF);
  free(tmp);

  if (!removed) fprintf(stderr, "charge: %s is not a configured repo\n", url);
  else printf("charge: removed repo %s\n", url);
  return CHARGE_OK;
}

/* ---- downloading (libcurl) ---- */

static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *ud) {
  return fwrite(ptr, size, nmemb, (FILE *)ud);
}

static int http_get(const char *url, const char *dest) {
  FILE *f = open_db_file(dest, "wb");
  if (!f) { perror("charge: cannot open download target"); return CHARGE_ERR; }

  CURL *c = curl_easy_init();
  if (!c) { fclose(f); return CHARGE_ERR; }
  curl_easy_setopt(c, CURLOPT_URL, url);
  curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(c, CURLOPT_FAILONERROR, 1L);
  curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(c, CURLOPT_WRITEDATA, f);

  CURLcode rc = curl_easy_perform(c);
  curl_easy_cleanup(c);
  fclose(f);

  if (rc != CURLE_OK) {
    unlink(dest);
    fprintf(stderr, "charge: download failed: %s\n", curl_easy_strerror(rc));
    return CHARGE_ERR;
  }
  return CHARGE_OK;
}

static void sha256_hex(const char *path, char out[65]) {
  unsigned char digest[SHA256_DIGEST_LENGTH];
  unsigned char buf[65536];
  FILE *f = fopen(path, "rb");
  SHA256_CTX ctx;
  SHA256_Init(&ctx);
  if (f) {
    size_t r;
    while ((r = fread(buf, 1, sizeof buf, f)) > 0) SHA256_Update(&ctx, buf, r);
    fclose(f);
  }
  SHA256_Final(digest, &ctx);
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    sprintf(out + 2*i, "%02x", digest[i]);
  out[64] = '\0';
}

/* ---- package lookup in the combined index ---- */

typedef struct {
  char name[MAX_NAME];
  char file[512];
  char sha[65];
} IndexRow;

static int find_in_index(const char *name, IndexRow *out) {
  FILE *f = fopen(COMBINED, "r");
  if (!f) return CHARGE_ERR;
  char line[1024];
  int rc = CHARGE_ERR;
  while (fgets(line, sizeof line, f)) {
    if (line[0] == '\0' || line[0] == '#') continue;
    char n[MAX_NAME], ver[64], file[512], sha[65], tier[32], deps[256];
    if (sscanf(line, "%127s %63s %511s %64s %31s %255s",
               n, ver, file, sha, tier, deps) != 6) continue;
    if (strcmp(n, name) == 0) {
      snprintf(out->name, MAX_NAME, "%s", n);
      snprintf(out->file, 512, "%s", file);
      snprintf(out->sha, 65, "%s", sha);
      rc = CHARGE_OK;
      break;
    }
  }
  fclose(f);
  return rc;
}

static char *first_repo_url(void) {
  FILE *f = fopen(REPOS_CONF, "r");
  if (!f) return NULL;
  char line[1024];
  char *url = NULL;
  while (fgets(line, sizeof line, f)) {
    size_t n = strlen(line);
    while (n && (line[n-1] == '\n' || line[n-1] == '\r')) line[--n] = '\0';
    if (line[0] == '\0' || line[0] == '#') continue;
    url = xstrdup(line);
    break;
  }
  fclose(f);
  return url;
}

/* `charge update`: pull index.txt from every configured repo, merge */
int repos_update(void) {
  FILE *conf = fopen(REPOS_CONF, "r");
  if (!conf) {
    fprintf(stderr, "charge: no repos configured. `charge repos add <url>`\n");
    return CHARGE_ERR;
  }

  FILE *out = open_db_file(COMBINED, "w");
  fprintf(out, "# combined index — built by `charge update`\n");

  char url[1024];
  int any = 0;
  while (fgets(url, sizeof url, conf)) {
    size_t n = strlen(url);
    while (n && (url[n-1] == '\n' || url[n-1] == '\r')) url[--n] = '\0';
    if (url[0] == '\0' || url[0] == '#') continue;

    char *idx_url = path_join(url, "index.txt");
    char *dest = malloc(strlen(COMBINED) + 7);
    sprintf(dest, "%s.part", COMBINED);
    printf("charge: updating from %s\n", url);
    if (http_get(idx_url, dest) == CHARGE_OK) {
      FILE *part = fopen(dest, "r");
      if (part) {
        char line[1024];
        while (fgets(line, sizeof line, part)) fprintf(out, "%s", line);
        fclose(part);
        any = 1;
      }
      unlink(dest);
    }
    free(idx_url);
    free(dest);
  }
  fclose(conf);
  fclose(out);

  if (!any) { unlink(COMBINED); fprintf(stderr, "charge: no index downloaded\n"); return CHARGE_ERR; }
  printf("charge: done\n");
  return CHARGE_OK;
}

/*
 * `charge <name>`: resolve the package against the repo, download the
 * .tar.zst into the cache, verify sha256 ('pending' skips the check).
 * Returns the local path (caller frees) or NULL on failure.
 */
char *fetch_from_repo(const char *name) {
  IndexRow row;
  if (find_in_index(name, &row) != CHARGE_OK) {
    fprintf(stderr, "charge: %s not in the repo index. run `charge update`\n", name);
    return NULL;
  }

  char *base = first_repo_url();
  if (!base) {
    fprintf(stderr, "charge: no repos configured. `charge repos add <url>`\n");
    return NULL;
  }

  char *url = path_join(base, row.file);
  char *cache = path_join(CACHE_DIR, row.file);
  char *dest = xstrdup(cache);

  printf("charge: downloading %s\n", url);
  if (http_get(url, dest) != CHARGE_OK) {
    free(url); free(dest); free(cache); free(base);
    return NULL;
  }
  free(url);
  free(base);
  free(cache);

  if (strcmp(row.sha, "pending") != 0 && strcmp(row.sha, "-") != 0) {
    char hex[65];
    sha256_hex(dest, hex);
    if (strcmp(hex, row.sha) != 0) {
      fprintf(stderr, "charge: sha256 mismatch for %s (trust the index? delete %s)\n", name, dest);
      unlink(dest);
      free(dest);
      return NULL;
    }
    printf("charge: sha256 verified\n");
  }

  return dest; /* caller frees */
}