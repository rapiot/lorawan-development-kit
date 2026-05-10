// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_ENV     
#define RAT_ENV

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_microcontroller/include/rat_microcontroller.h"
#include "../../rat_utilities//include/rat_i2c.h"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define RAT_ENV_SENSORS 1   // No different variants

#define RAT_ENV_BASE_ADDRESS 0x64

#define RAT_ENV_MEASURE_SINGLE_SHOT 0x219D
#define RAT_ENV_READ_MEASUREMENT    0xEC05

#define RAT_ENV_CRC_INITIALIZATION 0xFF
#define RAT_ENV_CRC_POLYNOMIAL     0x31

#define RAT_ENV_MEASUREMENT_DELAY 1000
#define RAT_ENV_READ_DELAY          10

#define RAT_ENV_TEMPERATURE_MINIMUM     -40.0f
#define RAT_ENV_TEMPERATURE_MAXIMUM      85.0f

#define RAT_ENV_HUMIDITY_MINIMUM          0.0f
#define RAT_ENV_HUMIDITY_MAXIMUM        100.0f

#define RAT_ENV_CARBON_DIOXIDE_MINIMUM  400.0f
#define RAT_ENV_CARBON_DIOXIDE_MAXIMUM 2000.0f

// -----------------------------------------------------------------------------
// Measurement
// -----------------------------------------------------------------------------
typedef struct {
    float temperature;
    float humidity;
    float carbon_dioxide;
} rat_env_measurement_t;

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void rat_env_init (void);

uint8_t rat_env_base_address (void);

bool rat_env_add (uint8_t   address,
                  uint8_t * identifier);

bool rat_env_measure (uint8_t master, rat_env_measurement_t * measurement);

#endif
