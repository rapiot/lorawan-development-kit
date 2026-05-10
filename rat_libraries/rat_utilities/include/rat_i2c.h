// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_I2C
#define RAT_I2C

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "MikroSDK.Driver.I2C.Master"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define RAT_I2C_DEFAULT_SCL_PIN PC3
#define RAT_I2C_DEFAULT_SDA_PIN PC4

#define RAT_I2C_DEFAULT_SPEED 100000
#define RAT_I2C_MINIMUM_SPEED 100000
#define RAT_I2C_MAXIMUM_SPEED 400000

#define RAT_I2C_SLAVES 16

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
bool rat_i2c_init (void);

bool rat_i2c_address (uint8_t   address,
                      uint8_t * slave);

bool rat_i2c_access (uint8_t   slave,
                     uint8_t * tx_data,
                     uint8_t   tx_length,
                     uint8_t * rx_data,
                     uint8_t   rx_length);

bool rat_i2c_write_register (uint8_t slave,
                             uint8_t address,
                             uint8_t value);

bool rat_i2c_read_register (uint8_t   slave,
                            uint8_t   address,
                            uint8_t * value);

bool rat_i2c_write (uint8_t   slave,
                    uint8_t * buffer,
                    uint8_t   length);

bool rat_i2c_read  (uint8_t   slave,
                    uint8_t * buffer,
                    uint8_t   length);

#endif
