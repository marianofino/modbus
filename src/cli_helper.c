#include "cli_helper.h"

void usage(void) {
    printf("\nUsage: modbus_cli -u <plc_ip> -f <function> [-p <port=502> -i <seconds=5>]\n"
    "\n"
    "Options:\n"
    "  -h, --help\t\t\t\tPrint this help message\n"
    "  -f, --function=function number\tSupported functions:\n\t\t\t\t\t- 01: Read Coil Status\n\t\t\t\t\t- 02: Read Input Status\n\t\t\t\t\t- 03: Read Holding Registers\n\t\t\t\t\t- 04: Read Input Registers\n"
    "  -u, --url=ip address\t\t\tIP Address of the modbus server (e.g. PLC)\n"
    "  -p, --port=port\t\t\tPort of the modbus server; by default is 502\n"
    "  -i, --interval=time in seconds\tPeriodicity of request of information; it\n\t\t\t\t\tmust be greater than zero\n"
    "  -d, --delta\t\t\t\tIf set, delta mapping will be used\n"
    "\n");
}

int parseArguments(int argc, char *argv[], cli_arguments_t * options) {
  int option_index = 0, opt;

  if (argc == 1) {
    usage();
    exit(EXIT_SUCCESS);
  }

  // initialize options
  strcpy(options->url, "");
  options->port = 502;
  options->function = 0;
  options->interval = 5;
  options->delta = 0;

  static struct option loptions[] = {
    {"help",        no_argument,       0, 'h'},
    {"port",        required_argument, 0, 'p'},
    {"url",         required_argument, 0, 'u'},
    {"function",    required_argument, 0, 'f'},
    {"interval",    required_argument, 0, 'i'}
  };

  while(1) {
    opt = getopt_long(argc, argv, "hu:p:f:i:d", loptions, &option_index);
    if (opt == -1) break;
    switch (opt) {
      case 'u':
        strcpy(options->url, optarg);
        break;
      case 'p':
        options->port = atoi(optarg);
        break;
      case 'f':
        options->function = atoi(optarg);
        break;
      case 'i':
        options->interval = atoi(optarg);
        break;
      case 'd':
        options->delta = 1;
        break;
      default:
        usage();
        exit(EXIT_SUCCESS);
        break;
    }
  }

  // Check that we have the min requirements to continue
  if (options->function == 0 || options->interval <= 0 || options->url[0] == '\0')
    return -1;

  printf("\nWelcome to Modbus CLI v%d.%d.%d", MODBUSCLI_VERSION_MAJOR, MODBUSCLI_VERSION_MINOR, MODBUSCLI_VERSION_PATCH);
  printf("\n============================\n");

  return 0;
}
