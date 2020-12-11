#include "screen.h"



void setChar(char c, uint32_t x, uint32_t y, uint8_t attr) {
    
    uint32_t position = getOffset(x,y);
    if(position > (SCREENHEIGHT * SCREENWIDTH * 2)){
        position = SCREENWIDTH * SCREENHEIGHT * 2;
    }
    unsigned char *vidmem = (unsigned char*) TEXTMEM;
    vidmem[position] = c;
    vidmem[position+1] = attr;
    
}

void setCursorPosition(uint8_t x, uint8_t y){
    uint16_t cursorLocation = y * 80 + x;
    outb(0x3D4, 14);
    outb(0x3D5, cursorLocation >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursorLocation); 
}

uint32_t getCursorPosition(){
    outb(0x3D4, 14);
    int offset = inb(0x3D5);
    offset = offset << 8;
    outb(0x3D4, 15);
    offset += inb(0x3D5);
    return 2 * offset;
}

void clearScreen(){
    
    for(int x = 0; x < SCREENWIDTH; x++){
        for(int y = 0; y < SCREENWIDTH; y++){
            setChar(' ',x,y,0x00);
        }
    }
    setCursorPosition(0,0);
}

void printChar(char c, uint8_t attr){
    // Get cursor position
    uint32_t cursPos = getCursorPosition();
    uint32_t cursX = getOffsetCol(cursPos);
    uint32_t cursY = getOffsetRow(cursPos);
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
            setChar(c,cursX,cursY,attr);
            cursX ++;
            break;
    }
    
    setCursorPosition(cursX, cursY);
}


void kprint(char * s){
    int i=0;
    while(s[i]){
        printChar(s[i],0x0f);
        i++;
    }
}

void putChar(char c){
    printChar(c,0x0f);
}

void printf(const char* format,  ...){
	const char *traverse; 
	unsigned int i; 
	char *s; 
    char buf[33];
    uint8_t color = 0x0f;
	va_list arg; 
	va_start(arg, format); 
	
	for(traverse = format; *traverse != '\0'; traverse++) 
	{ 
		//Module 2: Fetching and executing arguments
        if(*(traverse-1) == '%'){
		    switch(*traverse) 
		    { 
		    	case 'c' : i = va_arg(arg,int);		//Fetch char argument
		    				putChar(i);
		    				break; 

		    	case 'd' : i = va_arg(arg,int); 		//Fetch Decimal/Integer argument
		    				kprint(itoa(i,buf,10));
		    				break; 

		    	case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
		    				kprint(itoa(i,buf,8));
		    				break; 

		    	case 's': s = va_arg(arg,char *); 		//Fetch string
		    				kprint(s); 
		    				break; 

		    	case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                            kprint((char*)itoa(i,buf,16));
		    				break; 
                case 'C': i = va_arg(arg,unsigned int); //Get color
                            color = i;
		    				break; 
                default:
                    putChar('%');
                    putChar(*traverse);
                    break;
		    }
        } else {
            if(*traverse != '%'){
                printChar(*traverse,color);
            }
        }
    }
	
	//Module 3: Closing argument list to necessary clean-up
	va_end(arg); 
}








uint32_t getOffset(uint32_t x, uint32_t y){
    return ((y * SCREENWIDTH + x) * 2);
}
uint32_t getOffsetRow(uint32_t offset){
    return offset / (SCREENWIDTH * 2);
}
uint32_t getOffsetCol(uint32_t offset){
    return (offset - (getOffsetRow(offset) * SCREENWIDTH * 2))/2;
}