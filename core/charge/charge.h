/*
 * charge.h — Mani Linux package manager, shared declarations
 */

#ifndef CHARGE_H
#define CHARGE_H

#include <stddef.h>

#define CHARGE_VERSION "0.0.1"

/* Where charge keeps its database (Linux path — irrelevant on Windows) */
#define DB_ROOT      "/var/lib/charge"
#define DB_LOCAL     DB_ROOT "/local"
#define DB_STAGING   DB_ROOT "/staging"
#define DB_LOCK      DB_ROOT "/lock"

/* A parsed package header from .PKGINFO */
#define PKGINFO_IN_ARCHIVE ".PKGINFO"

/* maximum sizes for header fields */
#define MAX_NAME    128
#define MAX_VERSION 64

/* result codes */
#define CHARGE_OK   0
#define CHARGE_ERR  1

/* ---- util.c ---- */
char *xstrdup(const char *s);
int   mkdir_p(const char *path);                    /* mkdir -p */
int   path_exists(const char *path);
char *path_join(const char *a, const char *b);      /* caller frees */

/* ---- db.c ---- */
int  db_lock(void);                                  /* 0 on success */
void db_unlock(void);
int  db_list_installed(void);                        /* prints each package */
int  db_is_installed(const char *name);
int  db_write_entry(const char *name, const char *version,
                    const char **files, const char **deps); /* writes desc/files/depend */
int  db_remove_entry(const char *name, const char *version);
char *db_installed_version(const char *name);        /* caller frees, NULL if none */

/* ---- pkg.c ---- */
typedef struct {
  char name[MAX_NAME];
  char version[MAX_VERSION];
} PkgRef;

int pkg_read_pkginfo(const char *pkgfile, PkgRef *out);          /* read .PKGINFO from archive */
int pkg_extract_to_staging(const char *pkgfile, const char *pkg, const char *version);

/* ---- install.c ---- */
int  cmd_install(const char *pkg);                   /* charge <pkg> (local file or repo) */
int  cmd_remove(const char *name);                   /* charge down <pkg> */
int  cmd_upgrade(const char *name);                  /* recharge [<pkg>] */
int  copy_files_to_root(const char *staging_dir);    /* stage -> / , record file list */

/* ---- repo.c ---- */
int  repos_add(const char *url);
int  repos_remove(const char *url);
int  repos_list(void);
int  repos_update(void);                             /* charge update */
char *fetch_from_repo(const char *name);             /* resolve + download, caller frees */

#endif /* CHARGE_H */