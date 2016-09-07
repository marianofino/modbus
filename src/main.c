/*
 * ModbusCLI
 * ---------
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <modbus/modbus-tcp.h>
#include "config.h"

// Print usage information
void usage(void);
// Handle modbus connection
void connectModbus(modbus_t *, char *, int);
// Send modbus packet
void sendModbus(modbus_t *, int);
// Close everything and quit
void quitProgram(int);

// Modbus function 01
void read_coil_status(modbus_t *);
// Modbus function 02
void read_input_status(modbus_t *);
// Modbus function 03
void read_holding_registers(modbus_t *);
// Modbus function 04
void read_input_registers(modbus_t *);

// Global vars
modbus_t * mb;

void main(int argc, char *argv[]) {
  int option_index = 0, opt;
  /* configuration options */
  int port = 502, function = 0;
  char url[256] = {'\0'};
  //char serialport[256], measurement[256] = {'\0'}, url[256] = {'\0'};
  static struct option loptions[] = {
    {"help",        no_argument,       0, 'h'},
    {"port",        required_argument, 0, 'p'},
    {"url",         required_argument, 0, 'u'},
    {"function",    required_argument, 0, 'f'}
  };

  if (argc == 1) {
    usage();
    exit(EXIT_SUCCESS);
  }

  // Catch Ctrl+C
  struct sigaction act;
  act.sa_handler = quitProgram;
  sigaction(SIGINT, &act, NULL);

  while(1) {
    opt = getopt_long(argc, argv, "hu:p:f:", loptions, &option_index);
    if (opt == -1) break;
    switch (opt) {
      case 'u':
        strcpy(url, optarg);
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case 'f':
        function = atoi(optarg);
        break;
      default:
        usage();
        exit(EXIT_SUCCESS);
        break;
    }
  }

  // Check that we have the min requirements to continue
  if (function == 0 || url[0] == '\0') {
    usage();
    quitProgram(0);
  }

  printf("\nWelcome to Modbus CLI v%d.%d.%d", MODBUSCLI_VERSION_MAJOR, MODBUSCLI_VERSION_MINOR, MODBUSCLI_VERSION_PATCH);
  printf("\n============================\n");

  connectModbus(mb, url, port);
  sendModbus(mb, function);
  quitProgram(0);
}

void usage(void) {
    printf("\nUsage: modbus_cli -u <plc_ip> -f <function> [-p <port=502>]\n"
    "\n"
    "Options:\n"
    "  -h, --help\t\t\t\tPrint this help message\n"
    "  -f, --function=function number\tSupported functions:\n\t\t\t\t\t- 01: Read Coil Status\n\t\t\t\t\t- 02: Read Input Status\n\t\t\t\t\t- 03: Read Holding Registers\n\t\t\t\t\t- 04: Read Input Registers\n"
    "  -u, --url=ip address\t\t\tIP Address of the modbus server (e.g. PLC)\n"
    "  -p, --port=port\t\t\tPort of the modbus server; by default is 502\n"
    "\n");
}

void connectModbus(modbus_t * mb, char * url, int port) {
  // Connect to PLC through modbus TCP
  mb = modbus_new_tcp(url, port);

  modbus_set_debug(mb, 1);

  if (modbus_connect(mb) == -1) {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(mb);
    exit(EXIT_FAILURE);
  }
}

void sendModbus(modbus_t * mb, int function) {
  // Exit if unsupported function
  if (function > 4) {
    usage();
    exit(EXIT_SUCCESS);
  }

  // Launch function
  switch (function) {
    case 1:
      read_coil_status(mb);
      break;
    case 2:
      read_input_status(mb);
      break;
    case 3:
      read_holding_registers(mb);
      break;
    case 4:
      read_input_registers(mb);
      break;
    default:
      break;
  }

  // Close modbus connection
  modbus_close(mb);
  modbus_free(mb);
}

void read_coil_status(modbus_t * mb) {
  uint8_t * buffer_coil;
  int addr, nb, total, i;

  printf("Function: 01 Read Coil Status\n");
  printf("Address:");
  scanf("%d", &addr);
  printf("Number of coils:");
  scanf("%d", &nb);

  buffer_coil = malloc(nb * sizeof(uint8_t));

  total = modbus_read_bits(mb, addr, nb, buffer_coil);
  if (total == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    free(buffer_coil);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < total; i++) {
    printf("reg[%d]=%d\n", i + addr, buffer_coil[i]);
  }

  free(buffer_coil);
}

void read_input_status(modbus_t * mb) {
  uint8_t * buffer_input;
  int addr, nb, total, i;

  printf("Function: 02 Read Input Status\n");
  printf("Address:");
  scanf("%d", &addr);
  printf("Number of input bits:");
  scanf("%d", &nb);

  buffer_input = malloc(nb * sizeof(uint8_t));

  total = modbus_read_bits(mb, addr, nb, buffer_input);
  if (total == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    free(buffer_input);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < total; i++) {
    printf("reg[%d]=%d\n", i + addr, buffer_input[i]);
  }

  free(buffer_input);
}

void read_holding_registers(modbus_t * mb) {
  uint16_t * buffer_reg;
  int addr, nb, total, i;

  printf("Function: 03 Read Holding Registers\n");
  printf("Address:");
  scanf("%d", &addr);
  printf("Number of holding registers:");
  scanf("%d", &nb);

  buffer_reg = malloc(nb * sizeof(uint16_t));

  total = modbus_read_registers(mb, addr, nb, buffer_reg);
  if (total == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    free(buffer_reg);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < total; i++) {
    printf("reg[%d]=%d (0x%X)\n", i + addr, buffer_reg[i], buffer_reg[i]);
  }

  free(buffer_reg);
}

void read_input_registers(modbus_t * mb) {
  uint16_t * buffer_reg;
  int addr, nb, total, i;

  printf("Function: 04 Read Input Registers\n");
  printf("Address:");
  scanf("%d", &addr);
  printf("Number of input registers:");
  scanf("%d", &nb);

  buffer_reg = malloc(nb * sizeof(uint16_t));

  total = modbus_read_input_registers(mb, addr, nb, buffer_reg);
  if (total == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    free(buffer_reg);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < total; i++) {
    printf("reg[%d]=%d (0x%X)\n", i + addr, buffer_reg[i], buffer_reg[i]);
  }

  free(buffer_reg);
}

void quitProgram(int signal) {
  modbus_close(mb);
  modbus_free(mb);
  exit(EXIT_SUCCESS);
}
