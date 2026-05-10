// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifndef RAT_MCU
#define RAT_MCU

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Enumerated types
// -----------------------------------------------------------------------------
typedef enum
{
    RAT_MCU_PORT_A,
    RAT_MCU_PORT_B,
    RAT_MCU_PORT_C,
    RAT_MCU_PORT_E
} rat_mcu_port_e;


// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------
#define RAT_MCU_INIT_DELAY               100u   // 100 milliseconds

#define RAT_MCU_CYCLES_PER_SECOND      16384u   // 16-bit timer with 1:2 prescaling
#define RAT_MCU_MICROSECONDS_IN_SECOND  1000u   // 1,000 ms = 1 s

#define RAT_MCU_SLEEP_CYCLES_DEFAULT      15u   // 15 cycles in one minute
#define RAT_MCU_SLEEP_CYCLE_LENGTH         4u   //  4 seconds

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------
void rat_mcu_init  (void);

void rat_mcu_delay_ms (uint16_t ms);

void rat_mcu_measurement_interval  (uint8_t interval);
void rat_mcu_transmission_interval (uint8_t interval);

void rat_mcu_write_port (rat_mcu_port_e port, uint8_t value);

bool rat_mcu_measurement  (void);
bool rat_mcu_transmission (void);

void rat_mcu_sleep_cycle (void);

void rat_mcu_deep_sleep (uint8_t minutes);

#endif
