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
#include "../include/rat_utils.h"

// -----------------------------------------------------------------------------
// Convert a float to twos complement
// -----------------------------------------------------------------------------
uint32_t rat_convert_twos_complement (float   value,
                                      uint8_t decimals,
                                      uint8_t bytes)
{
    float scaled = value;

    uint32_t mask   = 0xFFFFFFFF;
    uint32_t result = 0x00000000;

    uint8_t cnt;

    for (cnt = 0; cnt < decimals; cnt++) {
        scaled = scaled * 10.0f;
    }

    if (value < 0) {
        result = -scaled;
    } else {
        result =  scaled;
    }

    if (value < 0) {
        result = result ^ mask;

        result++;
    }

    if (bytes == 1) {
        result = result & 0x000000FF;
    } else if (bytes == 2) {
        result = result & 0x0000FFFF;
    } else if (bytes == 3) {
        result = result & 0x00FFFFFF;
    } else if (bytes == 4) {
        result = result & 0xFFFFFFFF;
    } else {
        result = 0xFFFFFFFF;   // Error
    }

    return result;
}

static char rat_convert_uint8_to_char (uint8_t value)
{
    char character;

    if (value >= 0 && value <= 9) {
        character = 0x30 + value;
    } else {
        character = 0x41 + value - 0x0A;
    }

    return character;
}

static uint8_t rat_convert_char_to_uint8 (char character)
{
    uint8_t value;

    if (character >= 0x30 && character <= 0x39) {
        value = character - 0x30;
    } else {
        value = character - 0x41 + +0x0A;
    }

    return value;
}

// -----------------------------------------------------------------------------
// Convert an unsigned integer array to a char array
//
// ASCII codes:
//
// 0 - 9 : 48 - 57 (0x30 - 0x39)
// A - F : 65 - 70 (0x41 - 0x46)
// -----------------------------------------------------------------------------
void rat_convert_uint8_to_char_array (uint8_t * source,
                                      uint8_t   length,
                                      char    * destination)
{
    for (uint8_t index = 0; index < length; index++) {
        destination[index * 2]     = rat_convert_uint8_to_char((source[index] >> 4) & 0x0F);
        destination[index * 2 + 1] = rat_convert_uint8_to_char((source[index] >> 0) & 0x0F);
    }
}

// -----------------------------------------------------------------------------
// Convert a char array to an unsigned integer array
//
// ASCII codes:
//
// 0 - 9 : 48 - 57
// A - F : 65 - 70
// -----------------------------------------------------------------------------
void rat_convert_char_to_uint8_array (char    * source,
                                      uint8_t   length,
                                      uint8_t * destination)
{
    for (uint8_t index = 0; index < length; index++) {
        if (index % 2 == 0) {
            destination[index / 2]  = rat_convert_char_to_uint8(source[index]) << 4;
        } else {
            destination[index / 2] += rat_convert_char_to_uint8(source[index]) << 0;
        }
    }
}
