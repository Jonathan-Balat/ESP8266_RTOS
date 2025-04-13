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
    UART_ConfigTypeDef config =
    {
        .baud_rate = USER_BAUD_RATE,
        .data_bits = USER_DATA_BITS,
        .parity = USER_PARITY,
        .stop_bits = USER_STOP_BITS,
        .flow_ctrl = USER_FLOW_CTRL,
        .UART_RxFlowThresh = USER_RX_FLOW_THRESH,
        .UART_InverseMask = USER_LINE_INVERSION
    };

    // Apply UART configuration to UART0
    UART_ParamConfig(USER_UART_NUM, &config);

    // Set UART0 as the print port
    UART_SetPrintPort(USER_UART_NUM);

    // Optional: Clear FIFOs
    UART_ResetFifo(USER_UART_NUM);

    UART_ClearIntrStatus(USER_UART_NUM, UART_INTR_ALL); // Clear all interrupt status

    UART_SetIntrEna(USER_UART_NUM, UART_INTR_RXFIFO_TOUT); // Enable RX FIFO timeout interrupt

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
    while (READ_PERI_REG(UART_STATUS(USER_UART_NUM)) & (UART_RXFIFO_CNT << UART_RXFIFO_CNT_S))
    {
        // TODO: Establish a proper command frame protocol for the received data
        // For now, just read the character and echo it back
        received_char = READ_PERI_REG(UART_FIFO(USER_UART_NUM)) & 0xFF;

        printf("Received: %c\n", received_char);
    }

    // Clear the RX FIFO timeout and RX FIFO full interrupt flags
    UART_ClearIntrStatus(USER_UART_NUM, UART_INTR_RXFIFO_TOUT | UART_INTR_RXFIFO_FULL);
}
