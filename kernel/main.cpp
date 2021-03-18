#include <kernel/main.h>


extern "C" int kmain () {
    screen::kprint("A\n");
    for(int i = 0; i < 25; i++){
        screen::kprint("B\n");
    }
    return 0;
}