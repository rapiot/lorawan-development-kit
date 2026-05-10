// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_TC
#define RAT_TC   

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define RAT_TC_CSN_LFT PA4
#define RAT_TC_CSN_RGT PA1

#define RAT_TC_PWR_LFT PC0
#define RAT_TC_PWR_RGT PC1

#define RAT_TC_TEMPERATURE_MINIMUM -270.0f   // Celsius degrees
#define RAT_TC_TEMPERATURE_MAXIMUM 1800.0f   // Celsius degrees

#define RAT_TC_DELAY 100   // Milliseconds

// -----------------------------------------------------------------------------
// Typedefs
// -----------------------------------------------------------------------------
typedef enum rat_tc_sensors {
    RAT_TC_SENSOR_LFT,
    RAT_TC_SENSOR_RGT
} rat_tc_sensor_e;

typedef enum rat_tc_data_types {
  RAT_THERMOCOUPLE_TEMPERATURE_DATA,
  RAT_INTERNAL_TEMPERATURE_DATA
} rat_thermocouple_type_e;

typedef struct {
    float thermocouple_temperature_lft;
    float internal_temperature_lft;
    
    uint8_t fault_flag_lft;
    uint8_t short_vcc_flag_lft;
    uint8_t short_gnd_flag_lft;
    uint8_t open_circuit_flag_lft;

    float thermocouple_temperature_rgt;
    float internal_temperature_rgt;
    
    uint8_t fault_flag_rgt;
    uint8_t short_vcc_flag_rgt;
    uint8_t short_gnd_flag_rgt;
    uint8_t open_circuit_flag_rgt;
} rat_thermocouple_measurement_t;

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void rat_thermocouple_init (void);

void rat_thermocouple_measure (rat_thermocouple_measurement_t * measurement);

#endif
