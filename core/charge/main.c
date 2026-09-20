/*
 * main.c — `charge`: install is the default action.
 *
 *   sudo charge nmap        # installs nmap
 *   sudo charge down nmap   # deletes nmap
 *   sudo charge list        # shows installed packages
 *   sudo charge help        # usage
 */

#include "charge.h"

#include <stdio.h>
#include <string.h>

static void usage(FILE *out) {
  fprintf(out,
    "charge %s — Mani Linux package manager\n"
    "usage:\n"
    "  charge <pkg>          # install <pkg> from repo or local file\n"
    "  charge down <pkg>     # delete <pkg>\n"
    "  charge list           # show installed packages\n"
    "  charge update         # pull the repo index\n"
    "  charge repos add <url>      # add a repo (GitHub Pages, etc.)\n"
    "  charge repos remove <url>   # forget a repo\n"
    "  charge repos list           # show configured repos\n"
    "  charge help           # this message\n",
    CHARGE_VERSION);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    usage(stderr);
    return CHARGE_ERR;
  }

  /* dispatch on verbs; anything else is treated as a package to install */
  if (strcmp(argv[1], "help") == 0 ||
      strcmp(argv[1], "--help") == 0 ||
      strcmp(argv[1], "-h") == 0) {
    usage(stdout);
    return CHARGE_OK;
  }

  if (strcmp(argv[1], "list") == 0)
    return db_list_installed();

  if (strcmp(argv[1], "down") == 0) {
    if (argc < 3) {
      fprintf(stderr, "charge: down needs a package name\n");
      return CHARGE_ERR;
    }
    return cmd_remove(argv[2]);
  }

  if (strcmp(argv[1], "update") == 0)
    return repos_update();

  if (strcmp(argv[1], "repos") == 0) {
    if (argc < 3) return repos_list();
    if (strcmp(argv[2], "add") == 0) {
      if (argc < 4) { fprintf(stderr, "charge: repos add <url>\n"); return CHARGE_ERR; }
      return repos_add(argv[3]);
    }
    if (strcmp(argv[2], "remove") == 0) {
      if (argc < 4) { fprintf(stderr, "charge: repos remove <url>\n"); return CHARGE_ERR; }
      return repos_remove(argv[3]);
    }
    if (strcmp(argv[2], "list") == 0)
      return repos_list();
    fprintf(stderr, "charge: unknown repos command '%s'\n", argv[2]);
    return CHARGE_ERR;
  }

  /* default: install. future verbs: search, info, ... */
  return cmd_install(argv[1]);
}