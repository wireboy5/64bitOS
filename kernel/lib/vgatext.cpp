#include "vgatext.h"



void setch(char c, uint32_t x, uint32_t y, uint8_t attr) {
    
    uint32_t position = get_offset(x,y);
    if(position > (SCREENHEIGHT * SCREENWIDTH * 2)){
        position = SCREENWIDTH * SCREENHEIGHT * 2;
    }
    unsigned char *vidmem = (unsigned char*) TEXTMEM;
    vidmem[position] = c;
    vidmem[position+1] = attr;
    
}

void set_cursor_position(uint8_t x, uint8_t y){
    uint16_t cursorLocation = y * 80 + x;
    outb(0x3D4, 14);
    outb(0x3D5, cursorLocation >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursorLocation); 
}

uint32_t get_cursor_position(){
    outb(0x3D4, 14);
    int offset = inb(0x3D5);
    offset = offset << 8;
    outb(0x3D4, 15);
    offset += inb(0x3D5);
    return 2 * offset;
}

void clear_screen(){
    
    for(int x = 0; x < SCREENWIDTH; x++){
        for(int y = 0; y < SCREENWIDTH; y++){
            setch(' ',x,y,0x00);
        }
    }
    set_cursor_position(0,0);
}

void print_char(char c, uint8_t attr){
    // Get cursor position
    uint32_t cursPos = get_cursor_position();
    uint32_t cursX = get_offset_col(cursPos);
    uint32_t cursY = get_offset_row(cursPos);
    switch (c){
        case 0x08:
            cursX--; // Backspace
            break;
        case '\t':
            cursX = (cursX+8) & ~(8-1); // Tab
            break;
        case '\r':
            cursX = 0;
            break;
        case '\n':
            cursY++;
            cursX = 0;
            if(cursY >= SCREENHEIGHT){
                char* dst = (char*)TEXTMEM;
                char* src = (char*)(TEXTMEM+(SCREENWIDTH*2));
                memmove(dst,src,(SCREENWIDTH*SCREENHEIGHT*2-(SCREENWIDTH*2)));
                char* ll = (char*)TEXTMEM+(SCREENWIDTH*SCREENHEIGHT*2)-(SCREENWIDTH*2);
                for(int i = 0; i < SCREENWIDTH; i++){
                    ll[i*2] = ' ';
                    ll[i*2+1] = 0x0f;
                }
                cursY--;
            }
            break;
        default:
            setch(c,cursX,cursY,attr);
            cursX ++;
            break;
    }
    
    set_cursor_position(cursX, cursY);
}


void kprint(char * s){
    int i=0;
    while(s[i]){
        print_char(s[i],0x0f);
        i++;
    }
}

void put_char(char c){
    print_char(c,0x0f);
}










uint32_t get_offset(uint32_t x, uint32_t y){
    return ((y * SCREENWIDTH + x) * 2);
}
uint32_t get_offset_row(uint32_t offset){
    return offset / (SCREENWIDTH * 2);
}
uint32_t get_offset_col(uint32_t offset){
    return (offset - (get_offset_row(offset) * SCREENWIDTH * 2))/2;
}