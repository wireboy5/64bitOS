#include "kernel.h"

/*
    Screen Colors (For Reference)
        0 : black
        1 : blue
        2 : green
        3 : cyan
        4 : red
        5 : magenta
        6 : brown
        7 : light grey
        8 : dark grey
        9 : light blue
        A : light green
        B : light cyan
        C : light red
        D : light magneta
        E : light brown
        F : white. 
*/

extern "C" int kernel_main(){
    
    
    // Print that we are online
    printf("[%C OK %C] Running in 32 bit mode.\n",0x02,0x0f);

    // Initializing GDT
    initializeGDT();

    // Say that we have initialized the GDT
    printf("[%C OK %C] Initialized the GDT.\n",0x02,0x0f);
    

    // Initializing IDT
    initializeIDT();
    printf("[%C OK %C] Initialized the IDT.\n",0x02,0x0f);
    
    // Initialize Timer
    initTimer(50);
    printf("[%C OK %C] Initialized the PIT.\n",0x02,0x0f);

	return 0;

}


