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
void sendModbus(modbus_t *, int);

// Modbus function 01
void read_coil_status(modbus_t *);
// Modbus function 02
void read_input_status(modbus_t *);
// Modbus function 03
void read_holding_registers(modbus_t *);
// Modbus function 04
void read_input_registers(modbus_t *);

#endif /* MODBUS_IFC_H */
