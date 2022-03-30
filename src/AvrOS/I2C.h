#pragma once

#ifndef F_CPU
#error Please define F_CPU
#define    F_CPU    16000000UL      // 16MHz
#endif

#include "def.h"

#include <util/delay.h>
#include <avr/io.h>
#include<avr/iom328p.h>
#ifndef SCL_PORT
#error SCLに用いるポートを設定してください。
#define SCL_PORT PORTC
#endif
#ifndef SCL_DDR
#error SCLに用いるポートを設定してください。
#define SCL_DDR PORTC
#endif
#ifndef SCL_PORT_BIT
#error SCLに用いるポートのBITを設定してください。
#define SCL_PORT_BIT PORTC5
#endif
#ifndef SDA_PORT
#error SDAに用いるポートを設定してください。
#define SDA_PORT PORTC
#endif
#ifndef SDA_DDR
#error SDAに用いるポートを設定してください。
#define SDA_DDR PORTC
#endif
#ifndef SDA_PORT_BIT
#error SDAに用いるポートのBITを設定してください。
#define SDA_PORT_BIT PORTC4
#endif



#define SCL_H {cbi(SCL_DDR, SCL_PORT_BIT); sbi(SCL_PORT, SCL_PORT_BIT);}
#define SCL_L {cbi(SCL_PORT, SCL_PORT_BIT);sbi(SCL_DDR, SCL_PORT_BIT);}
#define SDA_H {cbi(SDA_DDR, SDA_PORT_BIT); sbi(SDA_PORT, SDA_PORT_BIT);}
#define SDA_L {cbi(SDA_PORT, SDA_PORT_BIT);sbi(SDA_DDR, SDA_PORT_BIT);}






/**
 * usとして利用できる値は0-4000
 */
void delayUS(unsigned int us);
class I2C {
public:
    I2C(){
        SDA_H;
        SCL_H;
    }
    bool start(u8 address,u8 rw){
        SDA_L;
        delayUS(3);
        SCL_L;
        delayUS(3);
        for (int i = 0; i < 7; i++) {
            if((address&(0b01000000)) != 0) {
                SDA_H;
            }
            else {
                SDA_L;
            }
            delayUS(2);
            SCL_H;
            delayUS(5);
            SCL_L;
            delayUS(3);
            address = address *2;
        }
        if (rw == 0) {
            SDA_L;
        }
        else {
            SDA_H;
        }
        delayUS(2);
        SCL_H;
        delayUS(5);
        SCL_L;
        delayUS(3);
        SDA_H;
        delayUS(2);
        SCL_H;
        delayUS(3);
        u8 sda = SDA_PIN&(1 << SDA_PORT_BIT);
        delayUS(2);
        SCL_L;
        delayUS(3);
        if (sda== 0) {
            return true;
        }
        return false;
    }
    bool write(u8 data) {
        for (int i = 0; i < 8; i++) {
            if ((data&(1 << 7)) != 0) {
                SDA_H;
            }
            else {
                SDA_L;
            }
            delayUS(2);
            SCL_H;
            delayUS(5);
            SCL_L;
            delayUS(3);
            data = data *2;
        }
        SDA_H;
        delayUS(2);
        SCL_H;
        delayUS(3);
        u8 sda = SDA_PIN&(1 << SDA_PORT_BIT);
        delayUS(2);
        SCL_L;
        delayUS(3);
        if (sda == 0) {
            return true;
        }
        PORTB=0xFF;

        return false;
    }
    bool stop() {
        SDA_L;
        delayUS(2);
        SCL_H;
        delayUS(3);
        SDA_H;
        delayUS(15);
        return true;

    }
    u8 read(bool last) {
        SDA_H;
        u8 data = 0;
        for (int i = 0; i < 8; i++) {
            data = data * 2;
            delayUS(2);
            SCL_H;
            delayUS(3);
            u8 res = SDA_PIN&(1 << SDA_PORT_BIT);
            if (res != 0) {
                data++;
            }
            delayUS(2);
            SCL_L;

            delayUS(3);
        }
        if(last){
            SDA_H;
        }else{
            SDA_L;
        }
        delayUS(2);
        SCL_H;
        delayUS(3);
        delayUS(2);
        SCL_L;
        delayUS(3);
        return data;


    }


};