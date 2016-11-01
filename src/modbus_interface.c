#include "modbus_interface.h"

void connectModbus(modbus_t ** mb, char * url, int port) {
  // Connect to PLC through modbus TCP
  *mb = modbus_new_tcp(url, port);

  // TODO: set debug options
  //modbus_set_slave(*mb, 1);
  //modbus_set_debug(*mb, 1);

  if (modbus_connect(*mb) == -1) {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(*mb);
    exit(EXIT_FAILURE);
  }
}

void disconnectModbus(modbus_t * mb) {
  modbus_close(mb);
  modbus_free(mb);
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

  total = modbus_read_input_bits(mb, addr, nb, buffer_input);
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

  printf("total: %d\n", total);

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
