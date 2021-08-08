#include "log.h"


/*
    Logs to all available sources at specified level
*/
void log(uint8_t level, char* format, ...) {

    // Variables for variable arguments
    va_list args;
    va_start(args, format);

    // Call vlog
    vlog(level, format, args);

    // End of variable arguments
    va_end(args);
}

/*
    Writes a character to all sources
*/
void log_put_char(char c) {
    write_serial(c);
}

/*
    Writes a string to all sources
*/
void log_puts(char* c) {
    while(*c) {
      log_put_char(*c);
      c++;
   }
}

/*
    Logs to all available sources at specified level.
    Accepts a va_list instead of variable arguments
*/
void vlog(uint8_t level, char* format, va_list args) {
    // If we should print at all
    bool print = level >= LOG_LEVEL;



    // if we should print, also print the log level
    if (print) {
        switch (level) {
            case LL_DEBUG:
                log_puts("[DEBUG] ");
                break;
            case LL_INFO:
                log_puts("[INFO]  ");
                break;
            case LL_WARN:
                log_puts("[WARN]  ");
                break;
            case LL_ERROR:
                log_puts("[ERROR] ");
                break;
            case LL_FATAL:
                log_puts("[FATAL] ");
                break;
        }
    }

    // Variables for traverseing the string
    char *current;

    // Variables for the current item
    char *s;
    uint64_t i;

    // Loop over the string
    for(current = format; *current != '\0'; current++) {

        while (*current != '%' && *current != '\0') {
            if(print) {
                log_put_char(*current);
            }
            current++;
        }

        if(*current == '\0') {
            break;
        }

        current++;

        switch (*current) {
            case '%': // Print a %
                if(print) {
                    log_put_char('%');
                }
                break;
            case 'c': i = va_arg(args, int); // Print a char
                if(print) {
                    log_put_char(i);
                }
                break;
            case 'd': i = va_arg(args, uint64_t); // Print a decimal
                if(print) {
                    char c[65];
                    itoa(i, c, 10);
                    log_puts(c);
                }
                break;
            case 'o': i = va_arg(args, uint64_t); // Print an octal
                if(print) {
                    char c[65];
                    itoa(i, c, 8);
                    log_puts(c);
                }
                break;
            case 'x': i = va_arg(args, uint64_t); // Print an hexadecimal
                if(print) {
                    char c[65];
                    itoa(i, c, 16);
                    log_puts(c);
                }
                break;
            case 'b': i = va_arg(args, uint64_t); // Print a binary
                if(print) {
                    char c[65];
                    itoa(i, c, 2);
                    log_puts(c);
                }
                break;
            case 's': s = va_arg(args, char*); // Print a string
                if(print) {
                    log_puts(s);
                }
                break;
            case 'n': { i = va_arg(args, uint64_t); // print an integer with a specific base
                current++;
                uint64_t pos = 0;
                while(true) {
                    bool in = false;
                    for(uint64_t i = 0; i < 10; i++) {
                        if(*current == "0123456"[i]) {
                            in = true;
                        }
                    }
                    if(!in) {
                        break;
                    }
                    current++;
                    pos++;
                    
                }
                
                uint64_t base = atoi(current - pos, pos);
                
                char c[65];
                itoa(i, c, base);
                log_puts(c);

                current--;
                break;
            }
            
        }
    }
}