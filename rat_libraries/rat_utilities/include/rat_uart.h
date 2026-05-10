// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_UART
#define RAT_UART

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "MikroSDK.Driver.UART"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define RAT_UART_TX_PIN PC6                   // Default TX pin
#define RAT_UART_RX_PIN PC7                   // Default RX pin

#define RAT_UART_BAUD_RATE 9600               // bps

#define RAT_UART_TX_BUFFER_SIZE 128u          // bytes
#define RAT_UART_RX_BUFFER_SIZE 128u          // bytes

#define RAT_UART_INIT_DELAY 100u              // ms

#define RAT_UART_BM_CLOCK_FREQUENCY 16000000u // Hz

#define RAT_UART_BM_DIVISOR 4.0f              // Default settings

#define RAT_UART_BM_BAUD_RATE_9600     9600u  // bps
#define RAT_UART_BM_BAUD_RATE_19200   19200u  // bps
#define RAT_UART_BM_BAUD_RATE_57600   57600u  // bps
#define RAT_UART_BM_BAUD_RATE_115200 115200u  // bps

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
bool rat_uart_init (void);

void rat_uart_tx (char * buffer, uint8_t length);
void rat_uart_rx (char * buffer, uint8_t length);

void rat_uart_clear (void);

void rat_uart_rx_break (char * buffer, uint8_t length, char * suffix);

#endif
