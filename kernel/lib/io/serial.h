#pragma once
#include <kernel/lib/io/ports.h>

// Initializes serial port
int init_serial();

int serial_received();

char read_serial();

int is_transmit_empty();

// Sends character to serial port
void write_serial(char a);

// Prints null terminated string to serial port
void serial_print(char* str);