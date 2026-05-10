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
#include "../include/rat_spi.h"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
static spi_master_t spi;
static spi_master_config_t spi_master_cfg;

// -----------------------------------------------------------------------------
// Init the SPI bus
//
// Note that the I2C and SPI buses use the same pins by default.
// -----------------------------------------------------------------------------
void rat_spi_init (void)
{
    spi_master_configure_default(&spi_master_cfg);

    spi_master_cfg.sck  = PC3;
    spi_master_cfg.miso = PC4;
    spi_master_cfg.mosi = PC5;

    spi_master_open(&spi, &spi_master_cfg);

    spi_master_set_speed(&spi, RAT_SPI_SPEED); 
}

// -----------------------------------------------------------------------------
// Write to the SPI bus
// -----------------------------------------------------------------------------
bool rat_spi_write (uint8_t * buffer,
                    uint8_t   length)
{
    bool response = false;
    
    if (spi_master_write(&spi, buffer, length) == SPI_MASTER_SUCCESS) {
        response = true; 
    } else {
        response = false;
    }

    return response;
}

// -----------------------------------------------------------------------------
// Read from the SPI bus
// -----------------------------------------------------------------------------
void rat_spi_read (uint8_t * buffer,  
                   uint8_t   length)
{
    spi_master_read(&spi, buffer, length);
}
