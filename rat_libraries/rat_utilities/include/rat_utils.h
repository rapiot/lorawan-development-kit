// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_UTILS
#define RAT_UTILS

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
uint32_t rat_convert_twos_complement (float   value,
                                      uint8_t decimals,
                                      uint8_t bytes);

void rat_convert_uint8_to_char_array (uint8_t * source,
                                      uint8_t   length,
                                      char    * destination);

void rat_convert_char_to_uint8_array (char    * source,
                                      uint8_t   length,
                                      uint8_t * destination);

#endif
