#ifndef MODBUS_IFC_H
#define MODBUS_IFC_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus/modbus-tcp.h>

// Handle modbus connection
void connectModbus(modbus_t **, char *, int);
// Handle modbus exit
void disconnectModbus(modbus_t *);
// Send modbus packet
void sendModbus(modbus_t *, int, int, int);

// Modbus function 01
void read_coil_status(modbus_t *, int, int);
// Modbus function 02
void read_input_status(modbus_t *, int, int);
// Modbus function 03
void read_holding_registers(modbus_t *, int, int);
// Modbus function 04
void read_input_registers(modbus_t *, int, int);
// Modbus function 05
void force_single_coil(modbus_t *, int);
// Modbus function 06
void preset_single_register(modbus_t *, int);
// Modbus function 15
void force_multiple_coils(modbus_t *, int);
// Modbus function 16
void preset_multiple_registers(modbus_t *, int);

// Delta mapping
int delta_map(char, int);

#endif /* MODBUS_IFC_H */
