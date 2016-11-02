/*
 * ModbusCLI
 * ---------
**/

#include <stdlib.h>
#include "modbus_interface.h"
#include "cli_helper.h"

// Global vars
modbus_t * mb;

// Close everything and quit
void quitProgram(int signal) {
  disconnectModbus(mb);
  exit(EXIT_SUCCESS);
}

void main(int argc, char *argv[]) {
  cli_arguments_t options;

  // Catch Ctrl+C
  struct sigaction act;
  act.sa_handler = quitProgram;
  sigaction(SIGINT, &act, NULL);

  if (parseArguments(argc, argv, &options) == -1) {
    usage();
    quitProgram(0);
  }

  connectModbus(&mb, options.url, options.port);
  sendModbus(mb, options.function, options.interval, options.delta);
  quitProgram(0);
}
