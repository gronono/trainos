#include "typedefs.h"

uint8_t* g_ScreenBuffer = (uint8_t*) 0xB8000;

void start() {
    g_ScreenBuffer[0] = 'H';
    g_ScreenBuffer[2] = 'E';
    g_ScreenBuffer[4] = 'L';
    g_ScreenBuffer[6] = 'L';
    g_ScreenBuffer[8] = 'O';
    g_ScreenBuffer[10] = ' ';
    g_ScreenBuffer[12] = ' ';
    g_ScreenBuffer[14] = ' ';
    g_ScreenBuffer[16] = ' ';
    for (;;);
}