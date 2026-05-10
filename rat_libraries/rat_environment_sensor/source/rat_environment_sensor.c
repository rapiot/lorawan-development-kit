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
#include "../include/rat_environment_sensor.h"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
static uint8_t i2c_master_identifiers[RAT_ENV_SENSORS];

static uint8_t i2c_master_identifier;

// -----------------------------------------------------------------------------
// Calculate the CRC
// -----------------------------------------------------------------------------
static uint8_t rat_env_crc (uint16_t value,
                            uint8_t  initialisation,
                            uint8_t  polynomial)
{
    uint8_t counter_byte = 0x00;
    uint8_t counter_bit  = 0x00;
    uint8_t checksum     = 0x00;

    uint8_t raw_data [2] = {0x00};

    checksum = initialisation;

    raw_data[0] = value >> 8;
    raw_data[1] = value % 256;

    for (counter_byte = 0;counter_byte < 2;++counter_byte) {
        checksum ^= (raw_data[counter_byte]);

        for (counter_bit = 8;counter_bit > 0;--counter_bit) {
            if ((checksum & 0x80) != 0x00) {
                checksum = (checksum << 1) ^ polynomial;
            } else {
                checksum = (checksum << 1);
            }
        }
    }

    return checksum;
}

// -----------------------------------------------------------------------------
// Init the sensor
// -----------------------------------------------------------------------------
void rat_env_init (void)
{
    i2c_master_identifier = 0;
}

// -----------------------------------------------------------------------------
// Base address
// -----------------------------------------------------------------------------
uint8_t rat_env_base_address (void)
{
    return RAT_ENV_BASE_ADDRESS;
}

// -----------------------------------------------------------------------------
// Add a sensor
// -----------------------------------------------------------------------------
bool rat_env_add (uint8_t   address,
                  uint8_t * identifier)
{
    if (i2c_master_identifier > RAT_ENV_SENSORS - 1) {
        return false;
    }

    bool result = rat_i2c_address(address, &i2c_master_identifiers[i2c_master_identifier]);

    *identifier = i2c_master_identifier;

    i2c_master_identifier++;

    return result;
}

// -----------------------------------------------------------------------------
// Convert raw temperature value to physical temperature
// -----------------------------------------------------------------------------
static float rat_env_convert_temperature (uint16_t temperature)
{
    float result = 0.0f;

    result = ( (float) temperature ) / 65535.0f;
    result = 175.0f * result;
    result = result - 45.0f;

    if (result < RAT_ENV_TEMPERATURE_MINIMUM) {
        result = RAT_ENV_TEMPERATURE_MINIMUM;
    } else if (result > RAT_ENV_TEMPERATURE_MAXIMUM) {
        result = RAT_ENV_TEMPERATURE_MAXIMUM;
    }

    return result;
}

// -----------------------------------------------------------------------------
// Convert raw humidity value to physical humidity
// -----------------------------------------------------------------------------
static float rat_env_convert_humidity (uint16_t humidity)
{
    float result = 0.0f;

    result = ( (float) humidity ) / 65535.0f;
    result = 125.0f * result;
    result = result - 6.0f;

    if (result < RAT_ENV_HUMIDITY_MINIMUM) {
        result = RAT_ENV_HUMIDITY_MINIMUM;
    } else if (result > RAT_ENV_HUMIDITY_MAXIMUM) {
        result = RAT_ENV_HUMIDITY_MAXIMUM;
    }

    return result;
}

// -----------------------------------------------------------------------------
// Convert raw carbon dioxide value to physical carbon dioxide
// -----------------------------------------------------------------------------
static float rat_env_convert_carbon_dioxide (uint16_t carbon_dioxide)
{
    float result = 0.0f;

    result = (float) carbon_dioxide;

    if (result < RAT_ENV_CARBON_DIOXIDE_MINIMUM) {
        result = RAT_ENV_CARBON_DIOXIDE_MINIMUM;
    } else if (result > RAT_ENV_CARBON_DIOXIDE_MAXIMUM) {
        result = RAT_ENV_CARBON_DIOXIDE_MAXIMUM;
    }

    return result;
}

// -----------------------------------------------------------------------------
// Measure temperature, humidity, and carbon dioxide
// -----------------------------------------------------------------------------
bool rat_env_measure (uint8_t master, rat_env_measurement_t * measurement)
{
    // -------------------------------------------------------------------------
    // Auxiliary variables
    // -------------------------------------------------------------------------
    uint8_t request   [2];
    uint8_t response [12];

    uint16_t raw_temperature;
    uint16_t raw_humidity;
    uint16_t raw_carbon_dioxide;
    uint16_t raw_status;

    uint8_t crc [4];

    // -------------------------------------------------------------------------
    // Request
    // -------------------------------------------------------------------------
    request[0] = RAT_ENV_MEASURE_SINGLE_SHOT << 8;
    request[1] = RAT_ENV_MEASURE_SINGLE_SHOT;

    if (!rat_i2c_write(master, request, 2)) {
        return false;
    }

    // -------------------------------------------------------------------------
    // Note! The sensor does NOT support clock stretching!
    //
    //       The sensor sends NACK response if data is not ready.
    // -------------------------------------------------------------------------
    rat_mcu_delay_ms(RAT_ENV_MEASUREMENT_DELAY);

    // -------------------------------------------------------------------------
    // Response
    // -------------------------------------------------------------------------
    if (!rat_i2c_read(master, response, 12)) {
        return false;
    }

    // -------------------------------------------------------------------------
    // Ccheck and calculate the results
    // -------------------------------------------------------------------------
    raw_carbon_dioxide = ( response[0] << 8 ) +
                          response[1];
    raw_temperature    = ( response[3] << 8 ) +
                           response[4];
    raw_humidity       = ( response[6] << 8 ) +
                           response[7];
    raw_status         = ( response[9] << 8 ) +
                           response[10];

    crc[0] = rat_env_crc(raw_carbon_dioxide,
                         RAT_ENV_CRC_INITIALIZATION,
                         RAT_ENV_CRC_POLYNOMIAL);
    crc[1] = rat_env_crc(raw_temperature,
                         RAT_ENV_CRC_INITIALIZATION,
                         RAT_ENV_CRC_POLYNOMIAL);
    crc[2] = rat_env_crc(raw_humidity,
                         RAT_ENV_CRC_INITIALIZATION,
                         RAT_ENV_CRC_POLYNOMIAL);
    crc[3] = rat_env_crc(raw_status,
                         RAT_ENV_CRC_INITIALIZATION,
                         RAT_ENV_CRC_POLYNOMIAL);

    measurement->carbon_dioxide = rat_env_convert_carbon_dioxide(raw_carbon_dioxide);
    measurement->temperature    = rat_env_convert_temperature(raw_temperature);
    measurement->humidity       = rat_env_convert_humidity(raw_humidity);
    
    if ((crc[0] == response[2]) &&
        (crc[1] == response[5]) &&
        (crc[2] == response[8]) &&
        (crc[3] == response[11])) {
        return true;
    } else {
        return false;
    }
}
