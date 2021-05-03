#include "vgatext.h"

uint16_t get_cursor_location(void) {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

uint16_t get_cursor_x(void) {
    uint16_t pos = get_cursor_location();
    return pos % VGA_ROW;
}

uint16_t get_cursor_y(void) {
    uint16_t pos = get_cursor_location();
    return pos / VGA_ROW;
}


void set_cursor_location(uint16_t x, uint16_t y) {
    uint16_t pos = y * VGA_ROW + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D4, (pos >> 8) & 0xFF);
}



/*
    Sets a character at a position
*/
void setch(uint32_t x, uint32_t y, char c, char forecolor, char backcolor){
    uint16_t pos = (y * VGA_ROW + (x));
    if(y * VGA_ROW + x > VGA_ROW * VGA_COL) {
        pos = VGA_ROW*VGA_COL;
    }
    uint64_t attr = (backcolor << 4) | (forecolor & 0xF);
    uint16_t* vidmem = (uint16_t*)VGA_TEXT_MEM;
    vidmem[pos] = (c) | (attr << 8);
}

/*
    Clears the screen
*/
void clear_screen(){
    for(int y = 0; y < VGA_COL; y++) {
        for(int x = 0; x < VGA_ROW; x++) {
            setch(x,y,' ',0x0,0x0);
        }
    }
    set_cursor_location(0,0);
}




void put_char(char c, char forecolor, char backcolor){
    uint16_t curx = get_cursor_x();
    uint16_t cury = get_cursor_y();

    switch(c) {
        case 0x08:
            curx--; // Backspace
            break;
        case '\t':
            curx = (curx+8)&~(8-1); // Tab
            break;
        case '\r':
            curx = 0; // Carriage return
            break;
        case '\n':
            cury++;
            curx = 0;
            if(cury >= VGA_COL) {
                char* dst = (char*)VGA_TEXT_MEM;
                char* src = (char*)(VGA_TEXT_MEM+(VGA_ROW*2));
                memmove(dst,src,(VGA_ROW*VGA_COL*2-(VGA_ROW*2)));
                char* ll = (char*)VGA_TEXT_MEM + (VGA_ROW*VGA_COL*2-(VGA_ROW*2));
                for(int i = 0; i < VGA_ROW; i++){
                    ll[i*2] = ' ';
                    ll[i*2+1] = 0x00;
                }
            }
            break;
        default:
            setch(curx,cury,c,forecolor,backcolor);
            curx++;
            break;
    }
    set_cursor_location(curx,cury);
}