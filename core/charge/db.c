/*
 * db.c — the charge database, pacman-style plain-text directories.
 *
 *   /var/lib/charge/local/<name>-<version>/
 *       desc      name, version, date
 *       files     every path this package owns
 *       depend    dependencies
 */

#include "charge.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/* ---- transaction lock ---- */

int db_lock(void) {
  if (mkdir(DB_LOCK, 0700) == 0)
    return CHARGE_OK;
  if (errno == EEXIST) {
    fprintf(stderr, "charge: another transaction is already running (%s)\n", DB_LOCK);
    return CHARGE_ERR;
  }
  perror("charge: cannot create lock");
  return CHARGE_ERR;
}

void db_unlock(void) {
  rmdir(DB_LOCK);
}

/* ---- installed database ---- */

/* /var/lib/charge/local/<name>-<version>/  (caller frees) */
static char *entry_dir(const char *name, const char *version) {
  char *base = path_join(DB_LOCAL, name);
  char *dir = path_join(base, version);
  free(base);
  return dir;
}

int db_is_installed(const char *name) {
  char *v = db_installed_version(name);
  if (!v) return 0;
  free(v);
  return 1;
}

char *db_installed_version(const char *name) {
  /* the local/<name>/ dir holds one subdir per installed version */
  char *base = path_join(DB_LOCAL, name);
  if (!path_exists(base)) { free(base); return NULL; }

  DIR *d = opendir(base);
  if (!d) { free(base); return NULL; }

  char *best = NULL;
  struct dirent *e;
  while ((e = readdir(d)) != NULL) {
    if (e->d_name[0] == '.') continue;
    if (e->d_type != DT_DIR) continue;
    if (!best || strcmp(e->d_name, best) > 0)
      best = xstrdup(e->d_name);
  }
  closedir(d);
  free(base);
  return best; /* caller frees */
}

int db_list_installed(void) {
  if (!path_exists(DB_LOCAL)) {
    fprintf(stderr, "charge: no packages installed yet\n");
    return CHARGE_OK;
  }
  DIR *d = opendir(DB_LOCAL);
  if (!d) { perror("charge: cannot open local db"); return CHARGE_ERR; }

  struct dirent *e;
  int found = 0;
  while ((e = readdir(d)) != NULL) {
    if (e->d_name[0] == '.') continue;
    if (e->d_type != DT_DIR) continue;
    printf("%s\n", e->d_name);
    found++;
  }
  closedir(d);
  if (!found) fprintf(stderr, "charge: no packages installed yet\n");
  return CHARGE_OK;
}

/* ---- writing entry files ---- */

static void write_desc(const char *dir, const char *name, const char *version) {
  char *desc_path = path_join(dir, "desc");
  FILE *f = fopen(desc_path, "w");
  free(desc_path);
  if (!f) { perror("charge: cannot write desc"); return; }

  time_t now = time(NULL);
  fprintf(f, "name = %s\nversion = %s\ninstalled = %s",
          name, version, ctime(&now)); /* ctime ends with '\n' */
  fclose(f);
}

static void write_lines(const char *dir, const char *file, const char **lines) {
  char *p = path_join(dir, file);
  FILE *f = fopen(p, "w");
  free(p);
  if (!f) { perror("charge: cannot write db file"); return; }
  for (; lines && *lines; lines++)
    fprintf(f, "%s\n", *lines);
  fclose(f);
}

int db_write_entry(const char *name, const char *version,
                   const char **files, const char **deps) {
  char *dir = entry_dir(name, version);
  int ok = mkdir_p(dir);
  if (ok == CHARGE_OK) {
    write_desc(dir, name, version);
    write_lines(dir, "files", files);
    write_lines(dir, "depend", deps);
  }
  free(dir);
  return ok;
}

int db_remove_entry(const char *name, const char *version) {
  char *dir = entry_dir(name, version);
  int r = rmdir(dir);
  if (r != 0) perror("charge: cannot remove db entry");
  free(dir);
  return r == 0 ? CHARGE_OK : CHARGE_ERR;
}