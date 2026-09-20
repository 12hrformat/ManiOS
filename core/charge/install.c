/*
 * install.c — charge <pkg> installs, charge down <pkg> removes,
 *              recharge [<pkg>] upgrades.
 *
 * Install flow (milestone 1-2):
 *   lock -> extract .tar.zst to /var/lib/charge/staging/<pkg>/<ver>/
 *        -> copy staged files onto /
 *        -> write local db entry (desc/files/depend)
 *        -> unlock
 *
 * TODO milestone 3: roll back /var/lib/charge/staging/ and root copies if a
 *      step fails halfway. Keep the file list so remove() can undo.
 */

#include "charge.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int looks_like_local_file(const char *arg) {
  /* a path (contains '/') or a bare .tar.zst name => local file */
  if (strchr(arg, '/')) return 1;
  return strstr(arg, ".tar.zst") != NULL;
}

int cmd_install(const char *arg) {
  /* `charge <pkg>`: a bare name means "fetch from the repo first" */
  char *local = NULL;
  if (looks_like_local_file(arg)) {
    local = xstrdup(arg);
  } else {
    local = fetch_from_repo(arg); /* repo.c: resolve index -> download -> cache */
  }
  if (!local) return CHARGE_ERR;

  PkgRef ref;
  if (pkg_read_pkginfo(local, &ref) != CHARGE_OK) {
    fprintf(stderr, "charge: %s has no readable .PKGINFO\n", local);
    free(local);
    return CHARGE_ERR;
  }

  if (db_lock() != CHARGE_OK) { free(local); return CHARGE_ERR; }

  printf("charge: installing %s %s\n", ref.name, ref.version);

  if (pkg_extract_to_staging(local, ref.name, ref.version) != CHARGE_OK)
    goto fail;

  char *staging = path_join(path_join(DB_STAGING, ref.name), ref.version);
  if (copy_files_to_root(staging) != CHARGE_OK) {
    free(staging);
    goto fail;
  }
  free(staging);

  /* TODO milestone 4: pass real depend list here (parse from .PKGINFO) */
  const char *no_deps[] = { NULL };
  db_write_entry(ref.name, ref.version, NULL, no_deps);

  free(local);
  db_unlock();
  printf("charge: done (%s)\n", ref.name);
  return CHARGE_OK;

fail:
  free(local);
  db_unlock();
  return CHARGE_ERR;
}

/*
 * Copy everything under staging/ to the real root and record every path.
 * Returns a NULL-terminated list (allocated) that the caller writes to the db.
 */
int copy_files_to_root(const char *staging_dir) {
  DIR *d = opendir(staging_dir);
  if (!d) { perror("charge: cannot open staging"); return CHARGE_ERR; }

  char *files[4096];
  size_t n = 0;

  struct dirent *e;
  while ((e = readdir(d)) != NULL && n + 1 < sizeof files / sizeof files[0]) {
    if (e->d_name[0] == '.') continue;

    char *src = path_join(staging_dir, e->d_name);   /* /var/lib/charge/staging/.. */
    char *dst = path_join("/", e->d_name);           /* copy onto the real root */
    printf("charge:   /%s\n", e->d_name);
    /* TODO milestone 1: real copy — open src, create dst, write bytes */
    files[n++] = xstrdup(e->d_name);
    free(src);
    free(dst);
  }
  closedir(d);
  files[n] = NULL;

  /* TODO milestone 2: hand `files` to db_write_entry() so /var/lib/charge
     knows what this package owns (needed for `charge down` + upgrades). */
  for (size_t i = 0; i < n; i++) {
    /* db_write_entry(ref.name, ref.version, files, deps); */
    free(files[i]);
  }
  return CHARGE_OK;
}

int cmd_remove(const char *name) {
  char *v = db_installed_version(name);
  if (!v) {
    fprintf(stderr, "charge: %s is not installed\n", name);
    return CHARGE_ERR;
  }

  if (db_lock() != CHARGE_OK) { free(v); return CHARGE_ERR; }

  printf("charge: removing %s %s\n", name, v);

  /* TODO milestone 2: read the files list from local/<name>/<ver>/files
     and unlink() each path (newest-first ordering is fine for v1). */

  db_remove_entry(name, v);
  /* TODO: also drop /var/lib/charge/staging/<name>/<ver> if present */

  db_unlock();
  free(v);
  printf("charge: %s removed\n", name);
  return CHARGE_OK;
}

int cmd_upgrade(const char *name) {
  if (db_lock() != CHARGE_OK) return CHARGE_ERR;

  if (name) {
    if (!db_is_installed(name)) {
      fprintf(stderr, "charge: %s is not installed\n", name);
      db_unlock();
      return CHARGE_ERR;
    }
    printf("recharge: upgrading %s\n", name);
    /* TODO milestone 5: fetch latest version from repo, install over the old */
  } else {
    printf("recharge: updating package lists\n");
    /* TODO milestone 5: refresh repo index */
    printf("recharge: upgrading everything\n");
    /* TODO milestone 5: for each installed pkg, if repo has a newer
       version, install it. Use the files list for clean replace. */
  }

  db_unlock();
  return CHARGE_OK;
}