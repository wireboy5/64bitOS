#include "screen.h"


namespace screen {
    uint8_t cursor_x;
    uint8_t cursor_y;

    void setch(uint8_t x, uint8_t y, char c, uint8_t attr) {
        uint16_t screen_pos = y * SCREEN_WIDTH + x;

        char* vidmem = (char*)0xb8000;

        vidmem[screen_pos*2] = c;
        vidmem[screen_pos*2+1] = attr;
    }

    void set_cursor_position(uint8_t x, uint8_t y) {
        uint16_t pos = y * SCREEN_WIDTH + x;
 
	    outb(0x3D4, 0x0F);
	    outb(0x3D5, (uint8_t) (pos & 0xFF));
	    outb(0x3D4, 0x0E);
	    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
    }

    void refresh_cursor() {
        if (cursor_x > SCREEN_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }

        if (cursor_y > SCREEN_HEIGHT) {
            int i;
            char* vidmem = (char*)0xb8000;
            for(i = 1; i < SCREEN_HEIGHT - 1; i++) {
                memmove(vidmem - (SCREEN_WIDTH*2),vidmem,SCREEN_WIDTH*2);
            }
            
        }
        set_cursor_position(cursor_x,cursor_y);
    }

    void kprint(char* text) {      
        int i = 0;
        while(*text) {
            switch(*text){
                case '\n' :
                    cursor_y++;
                    cursor_x = 0;
                    break;
                case '\r':
                    cursor_x = 0;
                    break;
                default:
                    setch(cursor_x,cursor_y,*text,0x0f);
                    cursor_x++;
            }
            
            text++;
            refresh_cursor();
        }
        
    }
} // namespace screen