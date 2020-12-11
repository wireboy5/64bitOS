#include "timer.h"


uint32_t tick = 0;

static void timer_callback(registers_t regs)
{
    printf("Tick: %d\n",tick);
    tick++;
}

void initTimer(uint32_t frequency){
    // Register the interrupt handler
    register_interrupt_handler(IRQ0, &timer_callback);

    // Calculate divisor
    uint32_t divisor = 1193180 / frequency;

    // Send command byte
    outb(0x43, 0x36);

    // Send divisor high byte
    outb(0x40,(uint8_t)(divisor & 0xFF));
    // Send divisor low byte
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}