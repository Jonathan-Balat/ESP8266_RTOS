#ifndef __USER_UART_H__
#define __USER_UART_H__

#include "esp_common.h"
#include "uart.h"

/********** Macros **********/
/* Interrupt macros not defined in uart.h */
#define UART_INTR_RXFIFO_TOUT   (1 << 8)    // RX FIFO timeout interrupt
#define UART_INTR_RXFIFO_FULL   (1 << 0)    // RX FIFO full interrupt
#define UART_INTR_TXFIFO_EMPTY  (1 << 1)    // TX FIFO empty interrupt
#define UART_INTR_LINE_STATUS   (1 << 2)    // Line status interrupt
#define UART_INTR_ALL (UART_INTR_RXFIFO_TOUT | UART_INTR_RXFIFO_FULL | UART_INTR_TXFIFO_EMPTY | UART_INTR_LINE_STATUS)

/********** Prototypes **********/
void init_uart(void);
void uart_rx_handler(void* arg);

#endif /* __USER_UART_H__ */