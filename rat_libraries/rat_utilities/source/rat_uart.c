// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include "../include/rat_uart.h"

#include "../../rat_microcontroller/include/rat_microcontroller.h"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
static uart_t        uart;
static uart_config_t uart_cfg;

static uint8_t uart_rx_buffer [RAT_UART_TX_BUFFER_SIZE];
static uint8_t uart_tx_buffer [RAT_UART_RX_BUFFER_SIZE];

// -----------------------------------------------------------------------------
// Init UART
// -----------------------------------------------------------------------------
bool rat_uart_init (void)
{
    // Set the pins
    uart_cfg.tx_pin = RAT_UART_TX_PIN;
    uart_cfg.rx_pin = RAT_UART_RX_PIN;
    
    // Set the buffers
    uart.tx_ring_buffer = uart_tx_buffer;
    uart.rx_ring_buffer = uart_rx_buffer;
    
    uart_cfg.tx_ring_size = sizeof(uart_tx_buffer);
    uart_cfg.rx_ring_size = sizeof(uart_rx_buffer);

    // Open the UART port
    if (uart_open(&uart, &uart_cfg) == ACQUIRE_FAIL) {
        return false;
    }

    // Set the baud rate
    if (uart_set_baud(&uart, RAT_UART_BAUD_RATE)      != UART_SUCCESS) {
        return false;
    }

    // Set the parity bit
    if (uart_set_parity(&uart, UART_PARITY_NONE)      != UART_SUCCESS) {
        return false;
    }

    // Set the stop bit
    if (uart_set_stop_bits(&uart, UART_STOP_BITS_ONE) != UART_SUCCESS) {
        return false;
    }

    // Set the data bits
    if (uart_set_data_bits(&uart, UART_DATA_BITS_8)   != UART_SUCCESS) {
        return false;
    }

    // Set the blocking mode
    uart_set_blocking(&uart, false);

    // Clear the buffers
    uart_clear(&uart);

    // Wait for stabilization
    rat_mcu_delay_ms(RAT_UART_INIT_DELAY);
    
    return true;
}

void rat_uart_clear (void)
{
    uart_clear(&uart);
}

// -----------------------------------------------------------------------------
// UART TX
// -----------------------------------------------------------------------------
void rat_uart_tx (uint8_t * buffer, uint8_t length)
{
    (void)uart_write(&uart, buffer, length);    
}

// -----------------------------------------------------------------------------
// UART RX
// -----------------------------------------------------------------------------
void rat_uart_rx (char * buffer, uint8_t length)
{
    char character;

    uint8_t index = 0;

    while (true) {
        if (uart_read(&uart, &character, 1) > 0) {
            if (character != '\r' &&
                character != '\n' && 
                character != '\0') {
                buffer[index] = character;

                index++;
            }

            if (index == length)  {
                break;
            }
            
        }            
    } 
}

// -----------------------------------------------------------------------------
// Check if the string ends with another string
// -----------------------------------------------------------------------------
static bool rat_string_suffix (char *buffer, char *suffix)
{
    if (!buffer || !suffix) {
        return false;
    }

    size_t buffer_length = strlen(buffer);
    size_t suffix_length = strlen(suffix);

    if (suffix_length > buffer_length) {
        return false;
    } else if (strncmp(buffer + buffer_length - suffix_length, suffix, suffix_length) == 0) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------
// UART RX
//
// Note that this function returns after receiving "OK" even if
// the length of the response is not sufficient.
// -----------------------------------------------------------------------------
void rat_uart_rx_break (char * buffer, uint8_t length, char * suffix)
{
    char character;

    uint8_t index = 0;

    while (true) {
        if (uart_read(&uart, &character, 1)) {
            if (character != '\r' &&
                character != '\n' && 
                character != '\0') {
                strncat(buffer, &character, 1);

                index++;
            }

            if (index == length)  {
                break;
            } else if (rat_string_suffix(buffer, suffix)) {
                break;
            }
        }            
    }     
}
