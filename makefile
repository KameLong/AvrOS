# 変数宣言
## ソース情報
TARGET = sample
HEDS =src/AvrOS/def.h src/AvrOS/Exception.h src/AvrOS/TaskManager.h src/AvrOS/UART.h src/AvrOS/I2C.h
SRCS = src/AvrOS/TaskManager.cpp src/main.cpp src/AvrOS/UART.cpp src/AvrOS/I2C.cpp
OBJS = $(SRCS:.c=.o)
## AVR デバイス情報
AVR_CHIP = atmega328p
AVR_WRIT = arduino
AVR_PORT = /dev/cu.usbmodem1101
F_CPU=16000000

## コンパイラ情報
CC = avr-gcc
CFLAGS = -Wall -O2 -mmcu=$(AVR_CHIP) -DF_CPU=$(F_CPU)
## 他
RM = -rm

# 実行ファイル及び HEX ファイル作成 (各 Cソースのオブジェクトファイルが必要)
$(TARGET).hex: $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o build/$(TARGET)
	avr-objcopy -F ihex build/$(TARGET) build/$(TARGET).hex

# 各 Cソースのオブジェクトファイル作成
%.o: %.c $(HEDS)
	$(CC) $(CFLAGS) -c $< -o $@

# HEX ファイル書き込み (Blink.hex が必要)
write: $(TARGET).hex
	avrdude -c $(AVR_WRIT) -P $(AVR_PORT) -p $(AVR_CHIP) -b 115200 -U build/$(TARGET).hex

asm:$(OBJS)
	$(CC) $(CFLAGS) -S $(OBJS)
# オブジェクトファイルと実行ファイル及び HEX ファイル削除
.PHONY: clean
clean:
	$(RM) build/*.*


