// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef APPLICATION
#define APPLICATION

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define APP_SLEEP_LENGTH                   1u   // One minute

#define APP_DEFAULT_MEASUREMENT_INTERVAL  10u   // Ten minutes
#define APP_DEFAULT_TRANSMISSION_INTERVAL 10u   // Ten minutes

#define APP_TEMPERATURE_RESOLUTION         2u   // Two decimals
#define APP_TEMPERATURE_SIZE               3u   // Three bytes

#define APP_HUMIDITY_RESOLUTION            1u   // One decimal
#define APP_HUMIDITY_SIZE                  2u   // Two bytes

#define APP_TX_PAYLOAD_SIZE               11u   // Ten bytes + termination
#define APP_RX_PAYLOAD_SIZE                5u   // Four bytes + termination

#define APP_CLOCK_FREQUENCY         16000000u   // Hz
#define APP_UART_BAUD_RATE              9600u   // bps

#define APP_MINIMUM_MEASUREMENT_INTERVAL  0x05  //   5 minutes
#define APP_MAXIMUM_MEASUREMENT_INTERVAL  0x78  // 120 minutes

#define APP_MINIMUM_TRANSMISSION_INTERVAL 0x05  //   5 minutes
#define APP_MAXIMUM_TRANSMISSION_INTERVAL 0x78  // 120 minutes

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void application_setup (void);
void application_task  (void);
void application_error (void);

#endif
