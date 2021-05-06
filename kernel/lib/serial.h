#pragma once
#include <kernel/lib/io.h>
#define PORT 0x3f8


void sprint(char* c);

int init_serial();

int serial_received();
 
char read_serial();

int is_transmit_empty();
 
void write_serial(char a);
