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
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define APP_SLEEP_LENGTH                   1   // One minute

#define APP_DEFAULT_MEASUREMENT_INTERVAL  10   // Ten minutes
#define APP_DEFAULT_TRANSMISSION_INTERVAL 10   // Ten minutes

#define APP_TEMPERATURE_RESOLUTION 2   // Two decimals
#define APP_TEMPERATURE_SIZE       3   // Three bytes

#define APP_TX_PAYLOAD_SIZE        26 + 1  // 13 bytes + termination
#define APP_RX_PAYLOAD_SIZE         2 + 1  // One byte   + termination

#define APP_INVALID_PERIOD 0x00   // No data received
#define APP_MINIMUM_PERIOD 0x05   // Minimum five minutes

#define APP_MINIMUM_MEASUREMENT_INTERVAL  0x05
#define APP_MAXIMUM_MEASUREMENT_INTERVAL  0x78

#define APP_MINIMUM_TRANSMISSION_INTERVAL 0x05
#define APP_MAXIMUM_TRANSMISSION_INTERVAL 0x78

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void application_error (void);
void application_setup (void);
void application_task  (void);
