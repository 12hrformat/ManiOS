/*
 * pkg.c — package format: read .PKGINFO from inside a .tar.zst,
 *         and extract the archive to staging.
 *
 * A package is a .tar.zst archive rooted at "/" with one metadata file
 * inside, .PKGINFO:
 *
 *   name = nmap
 *   version = 7.95
 *   depend = libpcre
 *   depend = openssl
 *   sha256sum = ...
 */

#include "charge.h"

#include <archive.h>
#include <archive_entry.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/* ---- .PKGINFO (name = value, repeats allowed) ---- */

static char *find_field(const char *data, const char *field, int nth) {
  /* crude: walk lines, match "field = value" where field ends with '=' or space */
  const char *p = data;
  char line[512];
  int seen = 0;
  while (p && *p) {
    const char *nl = strchr(p, '\n');
    size_t len = nl ? (size_t)(nl - p) : strlen(p);
    if (len >= sizeof line) len = sizeof line - 1;
    memcpy(line, p, len);
    line[len] = '\0';

    size_t fl = strlen(field);
    if (strncmp(line, field, fl) == 0) {
      const char *v = line + fl;
      while (*v == ' ' || *v == '\t') v++;
      if (*v == '=') {
        v++;
        while (*v == ' ' || *v == '\t') v++;
        if (seen == nth) return xstrdup(v);
        seen++;
      }
    }
    p = nl ? nl + 1 : NULL;
  }
  return NULL;
}

int pkg_parse_pkginfo_string(const char *data, PkgRef *out) {
  char *name = find_field(data, "name", 0);
  char *ver  = find_field(data, "version", 0);
  if (!name || !ver) {
    free(name);
    free(ver);
    return CHARGE_ERR;
  }
  snprintf(out->name, MAX_NAME, "%s", name);
  snprintf(out->version, MAX_VERSION, "%s", ver);
  free(name);
  free(ver);
  return CHARGE_OK;
}

/* read .PKGINFO out of the archive (before extraction) */
int pkg_read_pkginfo(const char *pkgfile, PkgRef *out) {
  struct archive *a = archive_read_new();
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);

  if (archive_read_open_filename(a, pkgfile, 16384) != ARCHIVE_OK) {
    fprintf(stderr, "charge: cannot open %s: %s\n", pkgfile, archive_error_string(a));
    archive_read_free(a);
    return CHARGE_ERR;
  }

  int rc = CHARGE_ERR;
  struct archive_entry *entry;
  while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
    const char *name = archive_entry_pathname(entry);
    if (!name || strcmp(name, PKGINFO_IN_ARCHIVE) != 0) {
      archive_read_data_skip(a);
      continue;
    }

    /* read the whole .PKGINFO entry into memory */
    size_t size = (size_t)archive_entry_size(entry);
    char *data = malloc(size + 1);
    if (data) {
      size_t got = archive_read_data(a, data, size);
      data[got] = '\0';
      rc = pkg_parse_pkginfo_string(data, out);
      free(data);
    }
    break;
  }

  archive_read_free(a);
  return rc;
}

/* ---- extraction ---- */

int pkg_extract_to_staging(const char *pkgfile, const char *pkg, const char *version) {
  char *staging = path_join(path_join(DB_STAGING, pkg), version);
  if (mkdir_p(staging) != CHARGE_OK) { free(staging); return CHARGE_ERR; }

  struct archive *a = archive_read_new();
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);

  if (archive_read_open_filename(a, pkgfile, 16384) != ARCHIVE_OK) {
    fprintf(stderr, "charge: cannot open %s: %s\n", pkgfile, archive_error_string(a));
    archive_read_free(a);
    free(staging);
    return CHARGE_ERR;
  }

  struct archive_entry *entry;
  int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_FFLAGS;
  int rc = CHARGE_OK;

  while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
    const char *epath = archive_entry_pathname(entry);

    /* skip the metadata file; extracted, not installed */
    if (!epath || strcmp(epath, PKGINFO_IN_ARCHIVE) == 0) {
      archive_read_data_skip(a);
      continue;
    }

    /* strip the leading "/" so we land under staging/ */
    if (epath[0] == '/') {
      char *rel = xstrdup(epath + 1);
      archive_entry_set_pathname(entry, rel);
      free(rel);
    }

    if (archive_read_extract(a, entry, flags) != ARCHIVE_OK) {
      fprintf(stderr, "charge: extract failed: %s\n", archive_error_string(a));
      rc = CHARGE_ERR;
      break;
    }
  }

  archive_read_free(a);
  free(staging);
  return rc;
}