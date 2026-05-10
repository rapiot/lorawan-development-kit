// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_TRH     
#define RAT_TRH

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
#define RAT_TRH_SENSORS 3   // Three different variants

#define RAT_TRH_BASE_ADDRESS       0x44

#define RAT_TRH_MEASURE_COMMAND    0xFD

#define RAT_TRH_CRC_INITIALIZATION 0xFF
#define RAT_TRH_CRC_POLYNOMIAL     0x31

#define RAT_TRH_DELAY 100   // 100 milliseconds

#define RAT_TRH_TEMPERATURE_MINIMUM -40.0f
#define RAT_TRH_TEMPERATURE_MAXIMUM  85.0f

#define RAT_TRH_HUMIDITY_MINIMUM      0.0f
#define RAT_TRH_HUMIDITY_MAXIMUM    100.0f

// -----------------------------------------------------------------------------
// Measurement
// -----------------------------------------------------------------------------
typedef struct {
    float temperature;
    float humidity;
} rat_trh_measurement_t;

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void rat_trh_init (void);

uint8_t rat_trh_base_address (void);

bool rat_trh_add (uint8_t   address,
                  uint8_t * identifier);

bool rat_trh_measure (uint8_t master, rat_trh_measurement_t * measurement);

#endif
