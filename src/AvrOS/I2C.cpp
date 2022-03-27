
#include "I2C.h"
/**
 * usとして利用できる値は0-4000
 */
void delayUS(unsigned int us)
{
    constexpr u8 cpuUS=F_CPU/1000000;
    us=us * cpuUS;
    if(us<26){
        return;
    }
    us=us-22;
    us=us/4;

    // busy wait
    __asm__ __volatile__(
    "1: sbiw %0,1" "\n\t" // 2 cycles
    "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
    );
}
