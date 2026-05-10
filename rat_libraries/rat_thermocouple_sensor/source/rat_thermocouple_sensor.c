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
#include "../../rat_utilities/include/rat_spi.h"

#include "../include/rat_thermocouple_sensor.h"

#include "MikroSDK.Driver.GPIO.Out"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------

// Power pins
static digital_out_t rat_tc_lft_pwr_pin;
static digital_out_t rat_tc_rgt_pwr_pin;

// Chip select pins
static digital_out_t rat_tc_lft_csn_pin;
static digital_out_t rat_tc_rgt_csn_pin;

// -----------------------------------------------------------------------------
// Init the sensor
// -----------------------------------------------------------------------------
void rat_thermocouple_init (void)
{
    // Power pins
    digital_out_init(&rat_tc_lft_pwr_pin, RAT_TC_PWR_LFT);
    digital_out_init(&rat_tc_rgt_pwr_pin, RAT_TC_PWR_RGT);
    
    digital_out_low(&rat_tc_lft_pwr_pin);
    digital_out_low(&rat_tc_rgt_pwr_pin);

    // Chip select pins
    digital_out_init(&rat_tc_lft_csn_pin, RAT_TC_CSN_LFT);
    digital_out_init(&rat_tc_rgt_csn_pin, RAT_TC_CSN_RGT);

    digital_out_high(&rat_tc_lft_csn_pin);
    digital_out_high(&rat_tc_rgt_csn_pin);
}

// -----------------------------------------------------------------------------
// Convert rawtemperature value to physical temperature
// -----------------------------------------------------------------------------
static float rat_convert_temperature (uint32_t temperature,
                                      rat_thermocouple_type_e data_type)
{
    // -------------------------------------------------------------------------
    // Auxiliary variables
    // -------------------------------------------------------------------------
    int8_t index    = 0;
  
    uint8_t length  = 0;
    uint8_t counter = 0;
  
    uint32_t value  = 0;

    float result = 0.0;

    // -------------------------------------------------------------------------
    // Determine the start index and the length of the value
    // -------------------------------------------------------------------------
    if (data_type == RAT_THERMOCOUPLE_TEMPERATURE_DATA) {
        index = -2;
        length = 13;
    } else {
        index = -4;
        length = 11;
    }
  
    // -------------------------------------------------------------------------
    // Convert the temperature
    // -------------------------------------------------------------------------
    value = temperature;
  
    result = 0;
  
    for (counter = 0;counter < length;++counter) {
        if ((value % 2) == 1) {
            result += pow(2, index);
        }
    
        value = value >> 1;
  
        index += 1;
    }
  
    // -------------------------------------------------------------------------
    // Check the sign bit
    // -------------------------------------------------------------------------
    if ((value % 2) == 1) {
        result = -result;
    }

    // -------------------------------------------------------------------------
    // Check that the result is between the minimum and the maximum
    // -------------------------------------------------------------------------
    if (result < RAT_TC_TEMPERATURE_MINIMUM) {
        result = RAT_TC_TEMPERATURE_MINIMUM;
    } else if (result > RAT_TC_TEMPERATURE_MAXIMUM) {
        result = RAT_TC_TEMPERATURE_MAXIMUM;
    }

    // -------------------------------------------------------------------------
    // Return
    // -------------------------------------------------------------------------
    return result;
}

// -----------------------------------------------------------------------------
// Enable the sensor
// -----------------------------------------------------------------------------
void rat_thermocouple_enable (rat_tc_sensor_e tc_sensor)
{
    if (tc_sensor == RAT_TC_SENSOR_LFT) {
        digital_out_high(&rat_tc_lft_pwr_pin);

        rat_mcu_delay_ms(RAT_TC_DELAY);

        digital_out_low(&rat_tc_lft_csn_pin);
    } else {
        digital_out_high(&rat_tc_rgt_pwr_pin);

        rat_mcu_delay_ms(RAT_TC_DELAY);

        digital_out_low(&rat_tc_rgt_csn_pin);
    }

    rat_mcu_delay_ms(RAT_TC_DELAY);
}

// -----------------------------------------------------------------------------
// Disable the sensor
// -----------------------------------------------------------------------------
void rat_thermocouple_disable (rat_tc_sensor_e tc_sensor)
{
    rat_mcu_delay_ms(RAT_TC_DELAY);

    if (tc_sensor == RAT_TC_SENSOR_LFT) {
        digital_out_high(&rat_tc_lft_csn_pin);

        rat_mcu_delay_ms(RAT_TC_DELAY);

        digital_out_low(&rat_tc_lft_pwr_pin);
    } else {
        digital_out_high(&rat_tc_rgt_csn_pin);

        rat_mcu_delay_ms(RAT_TC_DELAY);

        digital_out_low(&rat_tc_rgt_pwr_pin);
    }
}

// -----------------------------------------------------------------------------
// Measure the temperature
// -----------------------------------------------------------------------------
void rat_thermocouple_measure (rat_thermocouple_measurement_t * measurement)
{
    // -------------------------------------------------------------------------
    // Auxiliary variables
    // -------------------------------------------------------------------------
    uint8_t sensor_index = 0;

    uint8_t response [4] = {0x00u, 0x00u, 0x00u, 0x00u};

    uint32_t raw_thermocouple_temperature_data = 0x00000000u;
    uint32_t raw_internal_temperature_data     = 0x00000000u;
  
    float temperature = 0.0f;
  
    // -------------------------------------------------------------------------
    //
    // -------------------------------------------------------------------------
    for (sensor_index = 0; sensor_index < 2; ++sensor_index) {  
        if (sensor_index == 0) {
            rat_thermocouple_enable(RAT_TC_SENSOR_LFT);
        } else {
            rat_thermocouple_enable(RAT_TC_SENSOR_RGT);
        }

        rat_mcu_delay_ms(RAT_TC_DELAY);

        rat_spi_read(response, 4);
  
        // ---------------------------------------------------------------------
        // Parse the raw data
        // ---------------------------------------------------------------------
  
        // ---------------------------------------------------------------------
        // Thermocouple temperature data
        // ---------------------------------------------------------------------
        raw_thermocouple_temperature_data = response[0];
  
        raw_thermocouple_temperature_data = raw_thermocouple_temperature_data << 6;
  
        raw_thermocouple_temperature_data += ( response[1] >> 2 );
  
        // ---------------------------------------------------------------------
        // Fault flag
        // ---------------------------------------------------------------------
        if (sensor_index == 0) {
            measurement->fault_flag_lft = response[1] % 2;
        } else {
            measurement->fault_flag_rgt = response[1] % 2;
        }
  
        // ---------------------------------------------------------------------
        // Internal temperature data
        // ---------------------------------------------------------------------
        raw_internal_temperature_data = response[2];

        raw_internal_temperature_data = raw_internal_temperature_data << 5;

        raw_internal_temperature_data += ( response[3] >> 3 );
  
        // ---------------------------------------------------------------------
        // Short to VCC flag
        // ---------------------------------------------------------------------
        if (sensor_index == 0) {
            measurement->short_vcc_flag_lft = ( response[3] >> 2 ) % 2;
        } else {
            measurement->short_vcc_flag_rgt = ( response[3] >> 2 ) % 2;
        }
    
        // -----------------------------------------------------------------------
        // Short to GND flag
        // -----------------------------------------------------------------------
        if (sensor_index == 0) {
            measurement->short_gnd_flag_lft = ( response[3] >> 1 ) % 2;
        } else {
            measurement->short_gnd_flag_rgt = ( response[3] >> 1 ) % 2;
        }
  
        // -----------------------------------------------------------------------
        // Open circuit flag
        // -----------------------------------------------------------------------
        if (sensor_index == 0) {
            measurement->open_circuit_flag_lft = response[3] % 2;
        } else {
            measurement->open_circuit_flag_rgt = response[3] % 2;
        }

        // -----------------------------------------------------------------------
        // Convert
        // -----------------------------------------------------------------------
        temperature = rat_convert_temperature(raw_thermocouple_temperature_data,
                                              RAT_THERMOCOUPLE_TEMPERATURE_DATA);
                             
        if (sensor_index == 0) {
            measurement->thermocouple_temperature_lft = temperature;
        } else {
            measurement->thermocouple_temperature_rgt = temperature;
        }
  
        temperature = rat_convert_temperature(raw_internal_temperature_data,
                                              RAT_INTERNAL_TEMPERATURE_DATA);
    
        if (sensor_index == 0) {
            measurement->internal_temperature_lft = temperature;
        } else {
            measurement->internal_temperature_rgt = temperature;
        }

        if (sensor_index == 0) {
            rat_thermocouple_disable(RAT_TC_SENSOR_LFT);
        } else {
            rat_thermocouple_disable(RAT_TC_SENSOR_RGT);
        }

        rat_mcu_delay_ms(RAT_TC_DELAY);
    }
}
