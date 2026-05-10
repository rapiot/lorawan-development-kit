// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#include "../include/rat_i2c.h"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------

static i2c_master_t        i2c_master;
static i2c_master_config_t i2c_master_config;

static i2c_slave_index;

static i2c_slave_addresses [RAT_I2C_SLAVES];

// -----------------------------------------------------------------------------
// Init the I2C bus
// -----------------------------------------------------------------------------
bool rat_i2c_init (void) 
{
    i2c_master_configure_default(&i2c_master_config);

    i2c_master_config.scl = RAT_I2C_DEFAULT_SCL_PIN;
    i2c_master_config.sda = RAT_I2C_DEFAULT_SDA_PIN;

    if (i2c_master_open(&i2c_master, &i2c_master_config) == I2C_MASTER_ERROR) {
        return false;
    }

    if (i2c_master_set_speed(&i2c_master, I2C_MASTER_SPEED_STANDARD) == I2C_MASTER_ERROR) {
        return false;
    }

    i2c_slave_index = 0;

    return true;
}

// -----------------------------------------------------------------------------
// Add a slave to the I2C bus
// -----------------------------------------------------------------------------
bool rat_i2c_address (uint8_t   address,
                      uint8_t * slave)
{
    if (i2c_slave_index > RAT_I2C_SLAVES - 1) {
        return false;
    }

    i2c_slave_addresses[i2c_slave_index] = address;

    *slave = i2c_slave_index;

    i2c_slave_index++;

    return true;
}

// -----------------------------------------------------------------------------
// Write and read
// -----------------------------------------------------------------------------
bool rat_i2c_access (uint8_t   slave,
                     uint8_t * tx_buffer,
                     uint8_t   tx_length,
                     uint8_t * rx_buffer,
                     uint8_t   rx_length)
{
    if (i2c_master_set_slave_address(&i2c_master, i2c_slave_addresses[slave]) == I2C_MASTER_ERROR) {
        return false;
    }

    if (i2c_master_write_then_read(&i2c_master,
                                   tx_buffer,
                                   tx_length,
                                   rx_buffer,
                                   rx_length) == I2C_MASTER_ERROR) {
        return false;
    } else {
        return true;
    }
}	

// -----------------------------------------------------------------------------
// Write register
// -----------------------------------------------------------------------------
bool rat_i2c_write_register (uint8_t slave,
                             uint8_t address,
                             uint8_t value)
{
    uint8_t buffer [2] = {address, value};

    if (i2c_master_set_slave_address(&i2c_master, i2c_slave_addresses[slave]) == I2C_MASTER_ERROR) {
        return false;
    }

    if (i2c_master_write(&i2c_master, buffer, 2) == I2C_MASTER_ERROR) {
        return false;
    } else {
        return true;
    }
}                             

// -----------------------------------------------------------------------------
// Read register
// -----------------------------------------------------------------------------
bool rat_i2c_read_register (uint8_t   slave,
                            uint8_t   address,
                            uint8_t * value)
{
    uint8_t buffer [1] = {address};

    if (i2c_master_set_slave_address(&i2c_master, i2c_slave_addresses[slave]) == I2C_MASTER_ERROR) {
        return false;
    }

    if (i2c_master_write_then_read(&i2c_master,
                                   buffer,
                                   1,
                                   value,
                                   1) == I2C_MASTER_ERROR) {
        return false;
    } else {
        return true;
    }
}

// -----------------------------------------------------------------------------
// Write bytes
// -----------------------------------------------------------------------------
bool rat_i2c_write (uint8_t   slave,
                    uint8_t * buffer,
                    uint8_t   length)
{
    if (i2c_master_set_slave_address(&i2c_master, i2c_slave_addresses[slave]) == I2C_MASTER_ERROR) {
        return false;
    }

    if (i2c_master_write(&i2c_master, buffer, length) == I2C_MASTER_ERROR) {
        return false;
    } else {
        return true;
    }
}

// -----------------------------------------------------------------------------
// Read bytes
// -----------------------------------------------------------------------------
bool rat_i2c_read (uint8_t   slave,
                   uint8_t * buffer,
                   uint8_t   length)
{
    if (i2c_master_set_slave_address(&i2c_master, i2c_slave_addresses[slave]) == I2C_MASTER_ERROR) {
        return false;
    }

    if (i2c_master_read(&i2c_master, buffer, length) == I2C_MASTER_ERROR) {
        return false;
    } else {
        return true;
    }
}
