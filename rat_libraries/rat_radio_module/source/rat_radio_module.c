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
#include "../../rat_microcontroller/include/rat_microcontroller.h"
#include "../../rat_utilities/include/rat_uart.h"

#include "../include/rat_radio_module.h"

#include "MikroSDK.Driver.GPIO.Out"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
static digital_out_t rat_radio_nrst_pin;

static char request  [RAT_RADIO_MODULE_TX_BUFFER_SIZE];
static char response [RAT_RADIO_MODULE_RX_BUFFER_SIZE];

// -----------------------------------------------------------------------------
// Init pins
// -----------------------------------------------------------------------------
static void rat_radio_module_init_pins (void)
{
    digital_out_init(&rat_radio_nrst_pin, RAT_RADIO_MODULE_NRST);

    digital_out_high(&rat_radio_nrst_pin);
}

// -----------------------------------------------------------------------------
// Reset
// -----------------------------------------------------------------------------
static void rat_radio_module_reset (void)
{
    rat_mcu_delay_ms(RAT_RADIO_MODULE_RESET_DELAY);

    digital_out_low(&rat_radio_nrst_pin);

    rat_mcu_delay_ms(RAT_RADIO_MODULE_RESET_DELAY);

    digital_out_high(&rat_radio_nrst_pin);
}

// -----------------------------------------------------------------------------
// Init the request and response buffers
// -----------------------------------------------------------------------------
static void rat_radio_module_init_buffers (char * command,
                                           char * parameters)
{
    char separator   [] = RAT_AT_SEPARATOR;
    char termination [] = RAT_AT_TERMINATION;

    memset(request , '\0', RAT_RADIO_MODULE_TX_BUFFER_SIZE);
    memset(response, '\0', RAT_RADIO_MODULE_RX_BUFFER_SIZE);

    strcat(request, command);
    strcat(request, separator);
    strcat(request, parameters);
    strcat(request, termination);
}

// -----------------------------------------------------------------------------
// Check the response
//
// For example:
//
// AT+DEVEUI=?\r\n
// AT+DEVEUI=1122334455667788\r\n
// OK\r\n
//
// AT+DEVEUI=1122334455667788\r\n
// AT+DEVEUI=1122334455667788\r\n
// OK\r\n
// -----------------------------------------------------------------------------
static bool rat_check_response (char * request,
                                char * response)
{
    // Check that the command of the response matches
    if (strncmp(response, request, strlen(request) - strlen(RAT_AT_TERMINATION)) != 0) {
        return false;
    }
        
    // Check the return value of the response matches
    if (strncmp(&response[strlen(response) - strlen(RAT_AT_SUCCESS)], RAT_AT_SUCCESS, strlen(RAT_AT_SUCCESS)) != 0) {
        return false;
    } else {
        return true;
    }
}

// -----------------------------------------------------------------------------
// Check the port
//
// For example:
//
// AT+RECV=?\r\n
// AT+RECV=1:12\r\n
// OK\r\n
// -----------------------------------------------------------------------------
static bool rat_check_port (char * command,
                            char * response)
{
    uint8_t index = strlen(command)          +
                    strlen(RAT_AT_SEPARATOR) +
                    strlen(RAT_AT_QUERY)     +
                    strlen(command)          +
                    strlen(RAT_AT_SEPARATOR);

    if (strncmp(&response[index], RAT_AT_PORT, strlen(RAT_AT_PORT)) == 0) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Write a request and ignore a response
//
// Note that some requests might lead to a reboot and
// checking the response in such cases is not meaningul.
//
// Since the response is ignored, the function does not return a value.
// -----------------------------------------------------------------------------
static void rat_radio_module_write_and_ignore_response (char * command,
                                                        char * parameters)
{
    // Init all necessary variables
    rat_radio_module_init_buffers(command, parameters);

    // Send request
    rat_uart_tx(request, strlen(request));
}

// -----------------------------------------------------------------------------
// Write a request and check a response (note the echo)
//
// For example:
//
// AT+DEVEUI=1122334455667788\r\n
// AT+DEVEUI=1122334455667788\r\n
// OK\r\n
// -----------------------------------------------------------------------------
static bool rat_radio_module_write_and_check_response (char * command,
                                                       char * parameters)
{
    // Init all necessary variables
    rat_radio_module_init_buffers(command, parameters);

    // Clear the UART buffers
    rat_uart_clear();

    // Send request
    rat_uart_tx(request, strlen(request));
    
    // Receive response
    rat_uart_rx(response, strlen(request)            -
                          strlen(RAT_AT_TERMINATION) +
                          strlen(RAT_AT_SUCCESS));
    
    // Check the response
    if (rat_check_response(request, response)) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Write a request and check and parse a response
//
// For example:
//
// AT+DEVEUI=?\r\n
// AT+DEVEUI=?\r\n
// AT+DEVEUI=1122334455667788\r\n
// OK\r\n
// -----------------------------------------------------------------------------
static bool rat_radio_module_write_and_parse_response (char * command,
                                                       char * parameters,
                                                       char * value,
                                                       uint8_t length)
{
    // Init all necessary variables
    rat_radio_module_init_buffers(command, parameters);

    // Send request
    rat_uart_tx(request, strlen(request));
    
    // Receive response
    rat_uart_rx(response, strlen(request)     +    // Echo
                          strlen(request) - 1 +    // Indicator
                          length              +    // Data
                          strlen(RAT_AT_SUCCESS)); // Response
    
    // Check and parse the response
    if (rat_check_response(request, response)) {
        strncpy(value, &response[strlen(command)], length);

        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Set the network mode to LoRaWAN
//
// For example:
//
//   TX : AT+NWM=1\r\n
//   RX : AT+NWM=1\n\r
//   RX : OK
//
// Note that the response is aborted, because the network mode is changed!
// -----------------------------------------------------------------------------
static void rat_radio_module_set_network_mode (void)
{
    char command    [] = RAT_AT_NETWORK_MODE_COMMAND;
    char parameters [] = RAT_AT_NETWORK_MODE_LORAWAN;

    rat_radio_module_write_and_ignore_response(command, parameters);
}

// -----------------------------------------------------------------------------
// Set the network region
//
// For example:
// 
//   TX : AT+BAND=4\r\n
//   RX : AT+BAND=4\n\r
//   RX : OK\r\n
// -----------------------------------------------------------------------------
static bool rat_radio_module_set_region (void)
{
    char command    [] = RAT_AT_REGION_COMMAND;
    char parameters [] = RAT_AT_REGION_EU868;
    
    return rat_radio_module_write_and_check_response(command, parameters);
}

// -----------------------------------------------------------------------------
// Set the join mode
//
// For example:
//
//   TX : AT+NJM=0\r\n
//   RX : AT+NJM=0\n\r
//   RX : OK\r\n
// -----------------------------------------------------------------------------
static bool rat_radio_module_set_join_mode (void)
{
    char command    [] = RAT_AT_JOIN_MODE_COMMAND;
    char parameters [] = RAT_AT_JOIN_MODE_ABP;
    
    return rat_radio_module_write_and_check_response(command, parameters);
}

// -----------------------------------------------------------------------------
// Init the radio module
// -----------------------------------------------------------------------------
bool rat_radio_module_init (void)
{
    rat_radio_module_init_pins();

    rat_radio_module_reset();

    rat_mcu_delay_ms(RAT_RADIO_MODULE_INIT_DELAY);

    rat_radio_module_set_network_mode();

    rat_mcu_delay_ms(RAT_RADIO_MODULE_INIT_DELAY);

    if (!rat_radio_module_set_region()) {
        return false;
    }

    if (!rat_radio_module_set_join_mode()) {
        return false;
    }

    return true;
}

// -----------------------------------------------------------------------------
// Set the device EUI
//
// For example:
//
//   TX : AT+DEVEUI=1122334455667788\r\n
//   RX : AT+DEVEUI=1122334455667788\n\r
//   RX : OK\r\n
// -----------------------------------------------------------------------------
bool rat_radio_module_set_device_eui (char * device_eui)
{
    char command [] = RAT_AT_DEVEUI_COMMAND;
    
    return rat_radio_module_write_and_check_response(command, device_eui);
}

// -----------------------------------------------------------------------------
// Get the device EUI
//
// For example:
//
//   TX : AT+DEVEUI=?\r\n
//   RX : AT+DEVEUI=?\n\r
//   RX : AT+DEVEUI=1122334455667788\r\n
//   RX : OK\r\n
// -----------------------------------------------------------------------------
bool rat_radio_module_get_device_eui (char * device_eui)
{
    char command    [] = RAT_AT_DEVEUI_COMMAND;
    char parameters [] = RAT_AT_QUERY;

    return rat_radio_module_write_and_parse_response(command,
                                                     parameters,
                                                     device_eui,
                                                     RAT_DEVICE_EUI_LENGTH);
}

// -----------------------------------------------------------------------------
// Set the device address
//
// For example:
//
//   TX : AT+DEVADDR=11223344\r\n
//   RX : AT+DEVADDR=11223344\n\r
//   RX : OK\r\n
// -----------------------------------------------------------------------------
bool rat_radio_module_set_device_addr (char * device_addr)
{
    char command [] = RAT_AT_DEVADDR_COMMAND;
    
    return rat_radio_module_write_and_check_response(command, device_addr);
}

// -----------------------------------------------------------------------------
// Get the device address
//
// For example:
//
//   TX : AT+DEVADDR=?\r\n
//   RX : AT+DEVADDR=?\n\r
//   RX : AT+DEVADDR=11223344\r\n
//   RX : OK\r\n
// -----------------------------------------------------------------------------
bool rat_radio_module_get_device_addr (char * device_addr)
{
   char command    [] = RAT_AT_DEVADDR_COMMAND;
   char parameters [] = RAT_AT_QUERY;

   return rat_radio_module_write_and_parse_response(command,
                                                     parameters,
                                                     device_addr,
                                                     RAT_DEVICE_ADDR_LENGTH);
}


// -----------------------------------------------------------------------------
// Set the application session key
//
// For example:
//
//   TX : AT+APPSKEY=112233445566778899AABBCCDDEEFF\r\n
//   RX : AT+APPSKEY=112233445566778899AABBCCDDEEFF\n\r
//   RX : OK\r\n
// -----------------------------------------------------------------------------
bool rat_radio_module_set_appskey (char * key)
{
    char command [] = RAT_AT_APPSKEY_COMMAND;

    return rat_radio_module_write_and_check_response(command, key);
}


// -----------------------------------------------------------------------------
// Set the network session key
//
// For example:
//
//   TX : AT+NWKSKEY=112233445566778899AABBCCDDEEFF\r\n
//   RX : AT+NWKSKEY=112233445566778899AABBCCDDEEFF\n\r
//   RX : OK\r\n
// -----------------------------------------------------------------------------
bool rat_radio_module_set_nwkskey (char * key)
{
    char command [] = RAT_AT_NWKSKEY_COMMAND;
    
    return rat_radio_module_write_and_check_response(command, key);
}

// -----------------------------------------------------------------------------
// Transmit data
//
// For example:
//
//   TX : AT+SEND=1:11223344\r\n
//   RX : AT+SEND=1:11223344\n\r
//   RX : OK\r\n
// -----------------------------------------------------------------------------
static bool rat_radio_module_tx (char * payload)
{
    char command [] = RAT_AT_SEND_COMMAND;

    char parameters [RAT_AT_MAX_TX_LENGTH + 2 + 1];

    memset(parameters, '\0', RAT_AT_MAX_TX_LENGTH + 2 + 1);

    strcat(parameters, RAT_AT_PORT);
    strcat(parameters, RAT_AT_PORT_SEPARATOR);
    strcat(parameters, payload);

    // Init all necessary variables
    rat_radio_module_init_buffers(command, parameters);

    // Send request
    rat_uart_tx(request, strlen(request));
    
    // Receive response
    rat_uart_rx(response, strlen(request));
    
    // Check and parse the response
    if (rat_check_response(request, response)) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Receive data
//
// For example:
//
//   TX : AT+RECV=?\r\n
//
//   Response without data:
//
//   RX : AT+RECV=?\n\r
//   RX : AT+RECV=0:\r\n
//   RX : OK\r\n
//
//   Response with data:
//
//   RX : AT+RECV=?\n\r
//   RX : AT+RECV=1:FF\r\n
//   RX : OK\r\n
// -----------------------------------------------------------------------------
static bool rat_radio_module_rx (char * payload)
{
    char command    [] = RAT_AT_RECEIVE_COMMAND;
    char parameters [] = RAT_AT_QUERY;
    char suffix     [] = RAT_AT_SUCCESS;

    uint8_t response_length;

    response_length = strlen(command)               +
                      strlen(RAT_AT_SEPARATOR)      +
                      strlen(RAT_AT_QUERY)          +
                      strlen(command)               +
                      strlen(RAT_AT_SEPARATOR)      +
                      strlen(RAT_AT_PORT)           +
                      strlen(RAT_AT_PORT_SEPARATOR) +
                      RAT_AT_MAX_RX_LENGTH          +
                      strlen(RAT_AT_SUCCESS);

    // Init all necessary variables
    rat_radio_module_init_buffers(command, parameters);

    // Send request
    rat_uart_tx(request, strlen(request));
    
    // Receive response
    rat_uart_rx_break(response, response_length, suffix);

    rat_uart_tx(response, strlen(response));
    
    // Check the response
    if (!rat_check_response(request, response)) {
        return false;
    }

    // Check if there is downlink payload
    if (strlen(response) < response_length) {
        strncpy(payload, RAT_AT_DATA_NOT_AVAILABLE, strlen(RAT_AT_DATA_NOT_AVAILABLE));

        return true;
    }

    // Check the port
    if (!rat_check_port(command, response)) {
        return false;
    }

    // Copy the payload
    strncpy(payload, &response[response_length - RAT_AT_MAX_RX_LENGTH], RAT_AT_MAX_RX_LENGTH);

    return true;
}

// -----------------------------------------------------------------------------
// Send and receive a message
// -----------------------------------------------------------------------------
bool rat_radio_module_message (char * tx_payload,
                               char * rx_payload)
{
    if (!rat_radio_module_tx(tx_payload)) {
        return false;
    }

    rat_mcu_delay_ms(RAT_RADIO_MODULE_RX_DELAY);

    if (!rat_radio_module_rx(rx_payload)) {
        return false;
    }

    return true;
}
