#pragma once
/**
 * 使用整数型はbit数がわかるようにします
 * RUSTライクな型定義にします
 */
typedef char i8;
typedef unsigned char u8;
typedef int i16;
typedef unsigned int u16;
typedef long i32;
typedef unsigned long u32;
typedef long long i64;
typedef unsigned long long u64;

typedef float f16;
typedef double f32;

/**
 * bit操作を定義します
 * sbiはbit立ち上げ、cbiはbit足し下げ
 */
#define sbi(PORT,BIT) PORT|=_BV(BIT)
#define cbi(PORT,BIT) PORT&=~_BV(BIT)


/**
 * CPU周波数
 */
#ifndef F_CPU
#error F_CPUを指定してください。
#define F_CPU 16000000uL
#endif
#include <avr/iom328p.h>
#include <avr/io.h>
#include <stdarg.h>


#define SDA_PORT PORTC
#define SDA_DDR DDRC
#define SDA_PIN PINC
#define SDA_PORT_BIT PORTC4
#define SCL_PORT PORTC
#define SCL_DDR DDRC
#define SCL_PIN PINC
#define SCL_PORT_BIT PORTC5

