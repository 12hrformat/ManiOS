/*
 * recharge.c — update + upgrade.
 *
 *   sudo recharge          # update lists + upgrade everything
 *   sudo recharge nmap     # update lists + upgrade nmap
 */

#include "charge.h"

#include <stdio.h>
#include <string.h>

static void usage(FILE *out) {
  fprintf(out,
    "recharge %s — update and upgrade packages\n"
    "usage:\n"
    "  recharge            # update + upgrade all\n"
    "  recharge <pkg>      # update + upgrade a single package\n",
    CHARGE_VERSION);
}

int main(int argc, char **argv) {
  if (argc > 2) {
    usage(stderr);
    return CHARGE_ERR;
  }

  /* no argument => upgrade everything */
  const char *target = (argc == 2) ? argv[1] : NULL;
  return cmd_upgrade(target);
}