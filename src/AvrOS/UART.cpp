
#include "UART.h"

u8 UART::addPos=0;
u8 UART::sendPos=0;
char UART::sendBuf[UART::SEND_BUF_SIZE];
u8 UART::state=0b00000000;
char UART::receiveBuf[UART::SEND_BUF_SIZE];
u8 UART::receivePos=0;
void  (* UART::resCallBack)(char*);

//=================================================================================================
// USART割り込み
//=================================================================================================
ISR(USART_RX_vect){
        char res=UDR0;
        UART::receive(res);
}
ISR(USART_TX_vect){
        UART::send();
}