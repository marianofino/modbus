#ifndef CLI_HELPER_H
#define CLI_HELPER_H

#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

// Configuration options
typedef struct {
  char url[256];
  int port;
  int function;
} cli_arguments_t;

// Get program args and store them
int parseArguments(int, char **, cli_arguments_t *);
// Print usage information
void usage(void);

#endif /* CLI_HELPER_H */
