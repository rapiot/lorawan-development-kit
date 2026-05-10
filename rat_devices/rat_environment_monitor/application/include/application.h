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

#define APP_CARBON_DIOXIDE_RESOLUTION 0   // No decimals
#define APP_CARBON_DIOXIDE_SIZE       2   // Two bytes

#define APP_TEMPERATURE_RESOLUTION    2   // Two decimals
#define APP_TEMPERATURE_SIZE          3   // Three bytes

#define APP_HUMIDITY_RESOLUTION       1   // One decimal
#define APP_HUMIDITY_SIZE             2   // Two bytes

#define APP_TX_PAYLOAD_SIZE   14 + 1   // 14 bytes + termination
#define APP_RX_PAYLOAD_SIZE    4 + 1   // Four bytes   + termination

#define APP_MINIMUM_TRANSMISSION_INTERVAL 0x05   // Minimum five minutes
#define APP_MAXIMUM_TRANSMISSION_INTERVAL 0x78   // Maximum 120 minutes, two hours

#define APP_MINIMUM_SAMPLING_INTERVAL     0x01   // Minimum one minutes
#define APP_MAXIMUM_SAMPLING_INTERVAL     0x0A   // Maximum ten minutes, because of ASC

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void application_error (void);
void application_setup (void);
void application_task  (void);
