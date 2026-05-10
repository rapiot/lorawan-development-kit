// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_SPI
#define RAT_SPI

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "MikroSDK.Driver.SPI.Master"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define RAT_SPI_SPEED 100000

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void rat_spi_init (void);

bool rat_spi_write (uint8_t * buffer,
                    uint8_t   length);

void rat_spi_read (uint8_t * buffer,  
                   uint8_t   length);

#endif   
