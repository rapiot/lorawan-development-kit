// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../include/application.h"

// -----------------------------------------------------------------------------
// Main application
// -----------------------------------------------------------------------------
void main (void)
{
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    application_setup();

    while (true) {
        application_task();
    }
}
