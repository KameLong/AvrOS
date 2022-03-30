#pragma once

#include "avr/iom328p.h"
#include "avr/io.h"
#include <stdlib.h>

#include "UART.h"
// RAM使用状況を表示
void printMem() {
    u8 *stackptr;
    stackptr =  (u8*)(SP);
    int aSp = (int)stackptr;
    int aGvalEnd = (int)__malloc_heap_start - 1;


    UART::writeData("RAM \n");
    char buf[32];
    //global
    sprintf(buf,"G %d\n",aGvalEnd - 0x0100+ 1);
    UART::writeData(buf);
    //Heap
//    sprintf(buf,"H %d\n",aHeapEnd - 1 - (aGvalEnd + 1) + 1);
//    UART::writeData(buf);
    //free
    sprintf(buf,"F %d\n",aSp - (aGvalEnd + 1) + 1);
    UART::writeData(buf);
    sprintf(buf,"S %d\n",RAMEND - (aSp + 1) + 1);
    UART::writeData(buf);
}
