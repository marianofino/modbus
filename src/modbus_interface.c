#include "modbus_interface.h"

void connectModbus(modbus_t ** mb, char * url, int port) {
  // Connect to PLC through modbus TCP
  *mb = modbus_new_tcp(url, port);

  if (modbus_connect(*mb) == -1) {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(*mb);
    exit(EXIT_FAILURE);
  }

  // TODO: set debug options
  modbus_set_slave(*mb, 1);
  modbus_set_debug(*mb, 1);
}

void disconnectModbus(modbus_t * mb) {
  modbus_close(mb);
  modbus_free(mb);
}

void sendModbus(modbus_t * mb, int function, int interval, int delta) {
  // Launch function
  switch (function) {
    case 1:
      read_coil_status(mb, interval, delta);
      break;
    case 2:
      read_input_status(mb, interval, delta);
      break;
    case 3:
      read_holding_registers(mb, interval, delta);
      break;
    case 4:
      read_input_registers(mb, interval, delta);
      break;
    case 5:
      force_single_coil(mb, delta);
      break;
    case 6:
      preset_single_register(mb, delta);
      break;
    case 15:
      force_multiple_coils(mb, delta);
      break;
    case 16:
      preset_multiple_registers(mb, delta);
      break;
    default:
      printf("Function not supported.\n");
      break;
  }
}

void read_coil_status(modbus_t * mb, int interval, int delta) {
  uint8_t * buffer_coil;
  int addr, nb, total, i, delta_n;
  char delta_c;

  printf("Function: 01 Read Coil Status\n");
  printf("Address:");
  if (delta) {
    scanf("%c %d", &delta_c, &delta_n);
    addr = delta_map(delta_c, delta_n);
  } else
    scanf("%d", &addr);
  printf("Number of coils:");
  scanf("%d", &nb);

  buffer_coil = malloc(nb * sizeof(uint8_t));

  while (1) {

    total = modbus_read_bits(mb, addr, nb, buffer_coil);
    if (total == -1) {
      fprintf(stderr, "%s\n", modbus_strerror(errno));
      free(buffer_coil);
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < total; i++) {
      printf("reg[%d]=%d\n", i + addr, buffer_coil[i]);
    }

    sleep(interval);
  }

  free(buffer_coil);
}

void read_input_status(modbus_t * mb, int interval, int delta) {
  uint8_t * buffer_input;
  int addr, nb, total, i, delta_n;
  char delta_c;

  printf("Function: 02 Read Input Status\n");
  printf("Address:");
  if (delta) {
    scanf("%c %d", &delta_c, &delta_n);
    addr = delta_map(delta_c, delta_n);
  } else
    scanf("%d", &addr);
  printf("Number of input bits:");
  scanf("%d", &nb);

  buffer_input = malloc(nb * sizeof(uint8_t));

  while (1) {

    total = modbus_read_input_bits(mb, addr, nb, buffer_input);
    if (total == -1) {
      fprintf(stderr, "%s\n", modbus_strerror(errno));
      free(buffer_input);
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < total; i++) {
      printf("reg[%d]=%d\n", i + addr, buffer_input[i]);
    }

    sleep(interval);
  }

  free(buffer_input);
}

void read_holding_registers(modbus_t * mb, int interval, int delta) {
  uint16_t * buffer_reg;
  int addr, nb, total, i, delta_n;
  char delta_c;

  printf("Function: 03 Read Holding Registers\n");
  printf("Address:");
  if (delta) {
    scanf("%c %d", &delta_c, &delta_n);
    addr = delta_map(delta_c, delta_n);
  } else
    scanf("%d", &addr);
  printf("Number of holding registers:");
  scanf("%d", &nb);

  buffer_reg = malloc(nb * sizeof(uint16_t));

  while (1) {

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

    sleep(interval);
  }

  free(buffer_reg);
}

void read_input_registers(modbus_t * mb, int interval, int delta) {
  uint16_t * buffer_reg;
  int addr, nb, total, i, delta_n;
  char delta_c;

  printf("Function: 04 Read Input Registers\n");
  printf("Address:");
  if (delta) {
    scanf("%c %d", &delta_c, &delta_n);
    addr = delta_map(delta_c, delta_n);
  } else
    scanf("%d", &addr);
  printf("Number of input registers:");
  scanf("%d", &nb);

  buffer_reg = malloc(nb * sizeof(uint16_t));

  while (1) {

    total = modbus_read_input_registers(mb, addr, nb, buffer_reg);
    if (total == -1) {
      fprintf(stderr, "%s\n", modbus_strerror(errno));
      free(buffer_reg);
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < total; i++) {
      printf("reg[%d]=%d (0x%X)\n", i + addr, buffer_reg[i], buffer_reg[i]);
    }

    sleep(interval);
  }

  free(buffer_reg);
}

void force_single_coil(modbus_t * mb, int delta) {
  int addr, status, total, delta_n;
  char delta_c;

  printf("Function: 05 Force Single Coil\n");
  printf("Address:");
  if (delta) {
    scanf("%c %d", &delta_c, &delta_n);
    addr = delta_map(delta_c, delta_n);
  } else
    scanf("%d", &addr);
  printf("Status of coil:");
  scanf("%d", &status);

  total = modbus_write_bit(mb, addr, status);
  if (total == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void preset_single_register(modbus_t * mb, int delta) {
  int addr, value, total, delta_n;
  char delta_c;

  printf("Function: 06 Preset Single Register\n");
  printf("Address:");
  if (delta) {
    scanf("%c %d", &delta_c, &delta_n);
    addr = delta_map(delta_c, delta_n);
  } else
    scanf("%d", &addr);
  printf("Value of register:");
  scanf("%d", &value);

  total = modbus_write_register(mb, addr, value);
  if (total == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void force_multiple_coils(modbus_t * mb, int delta) {
  uint8_t * buffer_coil;
  int addr, nb, total, i, delta_n;
  char delta_c;

  printf("Function: 15 Force Multiple Coils\n");
  printf("Address:");
  if (delta) {
    scanf("%c %d", &delta_c, &delta_n);
    addr = delta_map(delta_c, delta_n);
  } else
    scanf("%d", &addr);
  printf("Number of coils:");
  scanf("%d", &nb);

  buffer_coil = malloc(nb * sizeof(uint8_t));

  for (i=0; i < nb; i++) {
    printf("Value for coil %d:", addr + i);
    scanf("%hhu", &buffer_coil[i]);
  }

  total = modbus_write_bits(mb, addr, nb, buffer_coil);
  if (total == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    free(buffer_coil);
    exit(EXIT_FAILURE);
  }

  free(buffer_coil);
}

void preset_multiple_registers(modbus_t * mb, int delta) {
  uint16_t * buffer_reg;
  int addr, nb, total, i, delta_n;
  char delta_c;

  printf("Function: 16 Preset Multiple Registers\n");
  printf("Address:");
  if (delta) {
    scanf("%c %d", &delta_c, &delta_n);
    addr = delta_map(delta_c, delta_n);
  } else
    scanf("%d", &addr);
  printf("Number of registers:");
  scanf("%d", &nb);

  buffer_reg = malloc(nb * sizeof(uint16_t));

  for (i=0; i < nb; i++) {
    printf("Value for register %d:", addr + i);
    scanf("%hu", &buffer_reg[i]);
  }

  total = modbus_write_registers(mb, addr, nb, buffer_reg);
  if (total == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    free(buffer_reg);
    exit(EXIT_FAILURE);
  }

  free(buffer_reg);
}

int delta_map(char delta_c, int delta_n) {
  int addr = 0;

  switch (delta_c) {
    case 'x':
      if (delta_n <= 16)
        addr = 1024 + delta_n; // 0x0400
      else
        addr = -1;
      break;
    case 'y':
      if (delta_n <= 16)
        addr = 1280 + delta_n; // 0x0500
      else
        addr = -1;
      break;
    case 'm':
      if (delta_n <= 255)
        addr = 2048 + delta_n; // 0x0800
      else
        addr = -1;
      break;
    case 'd':
      if (delta_n <= 255)
        addr = 4096 + delta_n; // 0x1000
      else
        addr = -1;
      break;
    default:
      printf("Unsupported mapping character (valid ones are x, y, m or d).\n");
      break;
  }

  if (addr == -1) {
    printf("Value out of range.\n");
    addr = 0;
  }

  printf("Using address %d.\n", addr);

  return addr;
}
