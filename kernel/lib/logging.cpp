#include "logging.h"


void log(char* str, uint8_t level) {
    switch (level) {
        case 0:
            serial_print("[ DEBUG ]");
            break;
        case 1:
            serial_print("[ INFO ] ");
            break;
        case 2:
            serial_print("[ WARN ] ");
            break;
        case 3:
            serial_print("[ ERROR ]");
            break;
        case 4:
            serial_print("[ PANIC ]");
    }

    // Print some padding
    serial_print("   ");

    // Print the text
    serial_print(str);


    return;
}