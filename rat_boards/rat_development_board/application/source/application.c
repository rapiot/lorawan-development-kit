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
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../../rat_libraries/rat_microcontroller/include/rat_microcontroller.h"

#include "../include/application.h"

#include "MikroSDK.Driver.GPIO.Port"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
static port_t port_a;
static port_t port_b;
static port_t port_c;

// -----------------------------------------------------------------------------
// Application
// -----------------------------------------------------------------------------
void application_error (void)
{
    //asm RESET;
}

// -----------------------------------------------------------------------------
// Application
// -----------------------------------------------------------------------------
void application_setup (void)
{
    rat_mcu_init();

    port_init(&port_a, PORT_A, 0xFF, PIN_DIRECTION_DIGITAL_OUTPUT);
    port_init(&port_b, PORT_B, 0xFF, PIN_DIRECTION_DIGITAL_OUTPUT);
    port_init(&port_c, PORT_C, 0xFF, PIN_DIRECTION_DIGITAL_OUTPUT);
}

// -----------------------------------------------------------------------------
// Application
// -----------------------------------------------------------------------------
void application_task (void)
{
    for (uint8_t counter = 0; counter < 24; counter++) {
        if (counter > 16 - 1) {
            port_write(&port_c, 0x01 << (counter - 16));
        } else if (counter > 8 - 1) {
            port_write(&port_b, 0x01 << (counter - 8));
        } else {
            port_write(&port_a, 0x01 << (counter));
        }

        if (counter == 24 - 1) {
            counter = 0;
        } else {
            counter++;
        }
        
        rat_mcu_delay_ms(APP_REFRESH_DELAY);
    }
}
