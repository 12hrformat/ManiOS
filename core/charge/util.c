/*
 * util.c — small helpers
 */

#include "charge.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/* strdup, but dies on OOM like any good distro tool */
char *xstrdup(const char *s) {
  char *copy = strdup(s);
  if (!copy) {
    perror("charge: out of memory");
    exit(CHARGE_ERR);
  }
  return copy;
}

/* a/b, no extra slashes */
char *path_join(const char *a, const char *b) {
  size_t la = strlen(a), lb = strlen(b);
  int sep = (la > 0 && a[la - 1] == '/') ? 0 : 1;
  char *out = malloc(la + sep + lb + 1);
  if (!out) {
    perror("charge: out of memory");
    exit(CHARGE_ERR);
  }
  memcpy(out, a, la);
  if (sep) out[la++] = '/';
  memcpy(out + la, b, lb);
  out[la + lb] = '\0';
  return out;
}

int path_exists(const char *path) {
  struct stat st;
  return stat(path, &st) == 0;
}

int mkdir_p(const char *path) {
  /* walk the path and mkdir each component (0700) */
  char *tmp = xstrdup(path);
  size_t len = strlen(tmp);
  if (len > 1 && tmp[len - 1] == '/') tmp[len - 1] = '\0';

  for (char *p = tmp + 1; *p; p++) {
    if (*p == '/') {
      *p = '\0';
      if (mkdir(tmp, 0700) < 0 && errno != EEXIST) { perror("charge: mkdir"); free(tmp); return CHARGE_ERR; }
      *p = '/';
    }
  }
  if (mkdir(tmp, 0700) < 0 && errno != EEXIST) { perror("charge: mkdir"); free(tmp); return CHARGE_ERR; }
  free(tmp);
  return CHARGE_OK;
}