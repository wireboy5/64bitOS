#pragma once
#include <stdint.h>
#include <kernel/x86/dt/idt.h>
#include <kernel/x86/libc/io.h>
#include <kernel/x86/drivers/screen.h>


void initTimer(uint32_t frequency);