#include "user_uart.h"


/**
 * @brief Initializes the UART communication interface.
 * @details This function configures UART0 with the specified parameters, sets it as the 
 *      default print port, clears FIFOs, and enables RX FIFO timeout interrupts. 
 *      It also registers a custom interrupt handler for UART RX and enables UART 
 *      interrupts globally.
 *      
 *          Configuration details:
 *          - Baud rate: 115200
 *          - Data bits: 8
 *          - Parity: None
 *          - Stop bits: 1
 *          - Flow control: None
 *          - RX flow threshold: 120
 *          - Line inversion: None
 *
 * @note Ensure that the custom interrupt handler `uart_rx_handler` is defined 
 *       elsewhere in the codebase.
 */
void init_uart(void)
{
    UART_ConfigTypeDef config;

    config.baud_rate = 115200;                          // Set baud rate to 115200
    config.data_bits = UART_WordLength_8b;              // Set data bits to 8
    config.parity = USART_Parity_None;                  // No parity
    config.stop_bits = USART_StopBits_1;                // 1 stop bit
    config.flow_ctrl = USART_HardwareFlowControl_None;  // No flow control
    config.UART_RxFlowThresh = 120;                     // RX flow threshold
    config.UART_InverseMask = UART_None_Inverse;        // No line inversion

    // Apply UART configuration to UART0
    UART_ParamConfig(UART0, &config);

    // Set UART0 as the print port
    UART_SetPrintPort(UART0);

    // Optional: Clear FIFOs
    UART_ResetFifo(UART0);

    UART_ClearIntrStatus(UART0, UART_INTR_ALL); // Clear all interrupt status

    UART_SetIntrEna(UART0, UART_INTR_RXFIFO_TOUT); // Enable RX FIFO timeout interrupt

    // Register a custom interrupt handler for UART RX
    UART_intr_handler_register(uart_rx_handler, NULL);

    // Enable UART interrupts globally
    ETS_UART_INTR_ENABLE();
    
    printf("UART communication initialized!\n");
}


void uart_rx_handler(void* arg)
{
    uint8_t received_char;

    // Read all available data from the RX FIFO
    while (READ_PERI_REG(UART_STATUS(UART0)) & (UART_RXFIFO_CNT << UART_RXFIFO_CNT_S))
    {
        // TODO: Establish a proper command frame protocol for the received data
        // For now, just read the character and echo it back
        received_char = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;

        printf("Received: %c\n", received_char);
    }

    // Clear the RX FIFO timeout and RX FIFO full interrupt flags
    UART_ClearIntrStatus(UART0, UART_INTR_RXFIFO_TOUT | UART_INTR_RXFIFO_FULL);
}
