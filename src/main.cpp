#include "avr/iom328p.h"
#include "avr/io.h"



#include "AvrOS/def.h"
#include <stdio.h>
#include "AvrOS/TaskManager.h"
#include "AvrOS/UART.h"
#include "AvrOS/I2C.h"
#include "AvrOS/memory.h"


 void setupMPU9255(I2C &i2c) {
    if(!i2c.start(0x68,0)){
        UART::writeData("err\n");
        PORTB=0xFF;
    }

    i2c.write(0x6B);
    i2c.write(0x00);
    i2c.stop();

    i2c.start(0x68,0);
    i2c.write(0x1A);
    i2c.write(0x05);
    i2c.stop();

    if(!i2c.start(0x68,0)){
        UART::writeData("err\n");
        PORTB=0xFF;
    }
    i2c.write(0x37);
    i2c.write(0x02);
    i2c.stop();

    if(!i2c.start(0x0c,0)){
        UART::writeData("err\n");
        PORTB=0xFF;
    }
    i2c.write(0x0A);
    i2c.write(0x16);
    i2c.stop();
    for(int i=0;i<100;i++){
        delayUS(4000);

    }
}


int main() {
    UART::init(9600);
    TaskManager::init();
    sei();
    printMem();
    PORTB = 0;
    DDRB = 0xFF;


    //ここからはMPU9255と接続します。
    I2C i2c;
    setupMPU9255(i2c);
    if (!i2c.start(0x0c, 0)) {
        UART::writeData("err\n");
        PORTB = 0xFF;
    }
    i2c.write(0x02);
    i2c.stop();
    if (!i2c.start(0x0c, 1)) {
        UART::writeData("err\n");
        PORTB = 0xFF;

    }
    u8 ST1 = i2c.read(true);
    char str[16];
    sprintf(str, "%x\n", ST1);
    UART::writeData(str);
    i2c.stop();

    if (ST1 & 0x01) {
        while (true) {
            i2c.start(0x0C, 0);
            i2c.write(0x03);
            i2c.stop();
            u8 buf[7] = {0};
            i2c.start(0x0C, 1);
            for (int i = 0; i < 6; i++) {
                buf[i] = i2c.read(false);
            }
            buf[6] = i2c.read(true);
            i2c.stop();
            //        if (!(buf[6] & 0x08)) {
            u16 mx = ((int16_t) buf[1] << 8) | buf[0];
            u16 my = ((int16_t) buf[3] << 8) | buf[2];
            u16 mz = ((int16_t) buf[5] << 8) | buf[4];
            //        }
            sprintf(str, "%x\n", mx);
            UART::writeData(str);
            sprintf(str, "%x\n", my);
            UART::writeData(str);
            sprintf(str, "%x\n", mz);
            UART::writeData(str);
            for (unsigned int i = 0; i < 50000; i++) {
                delayUS(10);
            }

        }
    }
}
