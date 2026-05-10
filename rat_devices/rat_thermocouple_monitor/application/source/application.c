// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Testing with the development board
//
// Hardware (Rapiot PIC development board):
//
//     - Use PIC18(L)F26K22 microcontroller and the 28 pins socket.
//     - Add Thermostart 3 Clicks (MIKROE-3724) to mikroBUS 5 and 6.
//       + Chip selects are RA4 and RA1.
//     - Add LR 14 Click (MIKROE-6303) to mikroBUS 2.
//
// Software (Necto Studio setup):
//
//     - Select the internal primary oscillator.
//     - Set the clock frequency to 16 MHz.
//     - Enable the watchdog.
//     - Set the watchdog prescaler to 1:32,768.
//
// Note that the application does not work with EasyPIC v8,
// because it does not support a secondary oscillator.
//
// Remember to create a header file (lorawan.h)
// which includes the necessary parameters for LoRaWAN network:
//
//     #define DEVICE_EUI  ".. 16 hex digits ..."
//     #define DEVICE_ADDR "... 8 hex digits ..."
//     #define APPSKEY     ".. 32 hex digits ..."
//     #define NWKSKEY     ".. 32 hex digits ..."
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "../../../../rat_libraries/rat_microcontroller/include/rat_microcontroller.h"
#include "../../../../rat_libraries/rat_utilities/include/rat_spi.h"
#include "../../../../rat_libraries/rat_utilities/include/rat_uart.h"
#include "../../../../rat_libraries/rat_thermocouple_sensor/include/rat_thermocouple_sensor.h"
#include "../../../../rat_libraries/rat_radio_module/include/rat_radio_module.h"
#include "../../../../rat_libraries/rat_utilities/include/rat_utils.h"

#include "../../../../rat_common/include/lorawan.h"

#include "../include/application.h"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
static rat_thermocouple_measurement_t measurement;

// -----------------------------------------------------------------------------
// Application error
// -----------------------------------------------------------------------------
void application_error (void)
{
    //asm RESET;
}

// -----------------------------------------------------------------------------
// Application setup
// -----------------------------------------------------------------------------
void application_setup (void)
{
    // Init all the components
    rat_mcu_init();
    rat_spi_init();
    rat_uart_init();

    // Init all the components
    rat_thermocouple_init();

    if (!rat_radio_module_init()) {
        application_error();
    }

    // Setup the radio module
    char device_eui  [] = DEVICE_EUI;
    char device_addr [] = DEVICE_ADDR;

    char appskey [] = APPSKEY;
    char nwkskey [] = NWKSKEY;

    if (!rat_radio_module_set_device_eui (device_eui)) {
        application_error();
    }

    if (!rat_radio_module_set_device_addr(device_addr)) {
        application_error();
    }

    if (!rat_radio_module_set_appskey(appskey)) {
        application_error();
    }

    if (!rat_radio_module_set_nwkskey(nwkskey)) {
        application_error();
    }

    // Set the default measurement and transmission interval
    rat_mcu_measurement_interval(APP_DEFAULT_MEASUREMENT_INTERVAL);
    rat_mcu_transmission_interval(APP_DEFAULT_TRANSMISSION_INTERVAL);
}

// -----------------------------------------------------------------------------
// Application measurement
// -----------------------------------------------------------------------------
void application_measurement (void)
{
    rat_thermocouple_measure(&measurement);
}

// -----------------------------------------------------------------------------
// Application transmission
// -----------------------------------------------------------------------------
void application_transmission(void)
{
    // Convert the measurements to twos complements
    uint32_t thermocouple_temperature_lft;
    uint32_t thermocouple_temperature_rgt;

    uint32_t internal_temperature_lft;
    uint32_t internal_temperature_rgt;

    thermocouple_temperature_lft = rat_convert_twos_complement(measurement.thermocouple_temperature_lft,
                                                               APP_TEMPERATURE_RESOLUTION,
                                                               APP_TEMPERATURE_SIZE);

    thermocouple_temperature_rgt = rat_convert_twos_complement(measurement.thermocouple_temperature_rgt,
                                                               APP_TEMPERATURE_RESOLUTION,
                                                               APP_TEMPERATURE_SIZE);

    internal_temperature_lft     = rat_convert_twos_complement(measurement.internal_temperature_lft,
                                                               APP_TEMPERATURE_RESOLUTION,
                                                               APP_TEMPERATURE_SIZE);

    internal_temperature_rgt     = rat_convert_twos_complement(measurement.internal_temperature_rgt,
                                                               APP_TEMPERATURE_RESOLUTION,
                                                               APP_TEMPERATURE_SIZE);

    // Create the payload
    uint8_t tx_payload [(APP_TX_PAYLOAD_SIZE - 1) /2];
    uint8_t rx_payload [(APP_RX_PAYLOAD_SIZE - 1) /2];

    tx_payload[ 0] = ((measurement.fault_flag_lft        & 0x01) << 7) |
                     ((measurement.short_vcc_flag_lft    & 0x01) << 6) |
                     ((measurement.short_gnd_flag_lft    & 0x01) << 5) |
                     ((measurement.open_circuit_flag_lft & 0x01) << 4) |
                     ((measurement.fault_flag_rgt        & 0x01) << 3) |
                     ((measurement.short_vcc_flag_rgt    & 0x01) << 2) |
                     ((measurement.short_gnd_flag_rgt    & 0x01) << 1) |
                     ((measurement.open_circuit_flag_rgt & 0x01) << 0);

    tx_payload[ 1] = (thermocouple_temperature_lft >> 16) & 0xFF;
    tx_payload[ 2] = (thermocouple_temperature_lft >>  8) & 0xFF;
    tx_payload[ 3] = (thermocouple_temperature_lft >>  0) & 0xFF;

    tx_payload[ 4] = (thermocouple_temperature_rgt >> 16) & 0xFF;
    tx_payload[ 5] = (thermocouple_temperature_rgt >>  8) & 0xFF;
    tx_payload[ 6] = (thermocouple_temperature_rgt >>  0) & 0xFF;

    tx_payload[ 7] = (internal_temperature_lft     >> 16) & 0xFF;
    tx_payload[ 8] = (internal_temperature_lft     >>  8) & 0xFF;
    tx_payload[ 9] = (internal_temperature_lft     >>  0) & 0xFF;

    tx_payload[10] = (internal_temperature_rgt     >> 16) & 0xFF;
    tx_payload[11] = (internal_temperature_rgt     >>  8) & 0xFF;
    tx_payload[12] = (internal_temperature_rgt     >>  0) & 0xFF;

    char tx_payload_message [APP_TX_PAYLOAD_SIZE];
    char rx_payload_message [APP_RX_PAYLOAD_SIZE];

    memset(tx_payload_message, '\0', APP_TX_PAYLOAD_SIZE);
    memset(rx_payload_message, '\0', APP_RX_PAYLOAD_SIZE);

    rat_convert_uint8_to_char_array(tx_payload, (APP_TX_PAYLOAD_SIZE - 1) / 2, tx_payload_message);

    // Send and receive data
    if (!rat_radio_module_message(tx_payload_message,
                                  rx_payload_message)) {
        application_error();
    }

    // Updata the sleep period
    rat_convert_char_to_uint8_array(rx_payload_message, APP_RX_PAYLOAD_SIZE - 1, rx_payload);

    uint8_t measurement_interval  = rx_payload[0];
    uint8_t transmission_interval = rx_payload[1];

    if (measurement_interval >= APP_MINIMUM_MEASUREMENT_INTERVAL &&
        measurement_interval <= APP_MAXIMUM_MEASUREMENT_INTERVAL) {
        rat_mcu_measurement_interval(measurement_interval);
    }

    if (transmission_interval >= APP_MINIMUM_TRANSMISSION_INTERVAL &&
        transmission_interval <= APP_MAXIMUM_TRANSMISSION_INTERVAL) {
        rat_mcu_transmission_interval(transmission_interval);
    }
}

// -----------------------------------------------------------------------------
// Application task
// -----------------------------------------------------------------------------
void application_task (void)
{
    if (rat_mcu_measurement()) {
        application_measurement();
    }

    if (rat_mcu_transmission()) {
        application_transmission();
    }

    rat_mcu_deep_sleep(APP_SLEEP_LENGTH);
}
