// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_RADIO_MODULE
#define RAT_RADIO_MODULE

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define RAT_RADIO_MODULE_NRST PA0   // mikroBUS 2 RST pin

#define RAT_RADIO_MODULE_TX_BUFFER_SIZE  256   // 256 chars
#define RAT_RADIO_MODULE_RX_BUFFER_SIZE  256   // 256 chars

#define RAT_RADIO_MODULE_RESET_DELAY     100   //   100 milliseconds
#define RAT_RADIO_MODULE_INIT_DELAY     3000   // 3,000 milliseconds
#define RAT_RADIO_MODULE_RESPONSE_DELAY  100   //   100 milliseconds
#define RAT_RADIO_MODULE_RX_DELAY       3000   // 3,000 milliseconds

#define RAT_DEVICE_EUI_LENGTH  16   // 16 chars without null termination
#define RAT_DEVICE_ADDR_LENGTH  8   //  8 chars without null termination
#define RAT_APPSKEY_LENGTH     32   // 32 chars without null termination
#define RAT_NWKSKEY_LENGTH     32   // 32 chars without null termination

#define RAT_AT_MAX_TX_LENGTH  16  // Max uplink payload length (eight bytes)
#define RAT_AT_MAX_RX_LENGTH   2  // Max downlink payload length (two bytes)

#define RAT_AT_TERMINATION "\r\n"
#define RAT_AT_SEPARATOR   "="
#define RAT_AT_QUERY       "?"
#define RAT_AT_SUCCESS     "OK"

#define RAT_AT_PORT           "1"
#define RAT_AT_PORT_SEPARATOR ":"
    
#define RAT_AT_DATA_NOT_AVAILABLE "00"

#define RAT_AT_NETWORK_MODE_COMMAND "AT+NWM"
#define RAT_AT_REGION_COMMAND       "AT+BAND"
#define RAT_AT_JOIN_MODE_COMMAND    "AT+NJM"
#define RAT_AT_DEVEUI_COMMAND       "AT+DEVEUI"
#define RAT_AT_DEVADDR_COMMAND      "AT+DEVADDR"
#define RAT_AT_APPSKEY_COMMAND      "AT+APPSKEY"
#define RAT_AT_NWKSKEY_COMMAND      "AT+NWKSKEY"
#define RAT_AT_SEND_COMMAND         "AT+SEND"
#define RAT_AT_RECEIVE_COMMAND      "AT+RECV"

#define RAT_AT_NETWORK_MODE_LORAWAN "1"   // 0 = P2P LoRa, 1 = LoRaWAN
#define RAT_AT_REGION_EU868         "4"   // 0 = EU 433 MHz, 4 = EU 868 MHz
#define RAT_AT_JOIN_MODE_ABP        "0"   // 0 = ABP, 1 = OTAA

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
bool rat_radio_module_init  (void);

bool rat_radio_module_set_device_eui (char * device_eui);
bool rat_radio_module_get_device_eui (char * device_eui);

bool rat_radio_module_set_device_addr (char * device_addr);
bool rat_radio_module_get_device_addr (char * device_addr);

bool rat_radio_module_set_appskey (char * key);
bool rat_radio_module_set_nwkskey (char * key);

bool rat_radio_module_message (char * tx_payload,
                               char * rx_payload);
#endif
