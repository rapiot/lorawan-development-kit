// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2026 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

#include "MikroSDK.Driver.GPIO.In"
#include "MikroSDK.Driver.GPIO.Out"
#include "MikroSDK.Driver.GPIO.Port"

#include "../include/rat_microcontroller.h"

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
static port_t port_a;
static port_t port_b;
static port_t port_c;
static port_t port_e;

static uint16_t sleep_cycles; 

static uint8_t measurement_interval;
static uint8_t transmission_interval;
  
static uint32_t timestamp;

static uint8_t sleep_minute  = 0x00;
static uint8_t sleep_cycle   = 0x00;

static uint8_t sleep_minutes = 0x00;
static uint8_t sleep_cycles_limit  = 0x00;

// -----------------------------------------------------------------------------
// Init the pins to avoid floating pins
// -----------------------------------------------------------------------------
static void rat_mcu_init_pins (void)
{
    // Init all pins as analog inputs
    /*
    ANSELA = 0xFF;
    ANSELB = 0xFF;
    ANSELC = 0xFF;

    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    */
    
    // Init all pins as outputs
    port_init(&port_a, PORT_A, 0xFF, PIN_DIRECTION_DIGITAL_OUTPUT);
    port_init(&port_b, PORT_B, 0xFF, PIN_DIRECTION_DIGITAL_OUTPUT);
    port_init(&port_c, PORT_C, 0xFF, PIN_DIRECTION_DIGITAL_OUTPUT);
    port_init(&port_e, PORT_E, 0xFF, PIN_DIRECTION_DIGITAL_OUTPUT);

    // Set all pins to digital low
    port_write(&port_a, 0x00);
    port_write(&port_b, 0x00);
    port_write(&port_c, 0x00);
    port_write(&port_e, 0x00);
} 

// -----------------------------------------------------------------------------
// Write port
// -----------------------------------------------------------------------------
void rat_mcu_write_port (rat_mcu_port_e port, uint8_t value)
{
    switch(port) {
        case RAT_MCU_PORT_A:
            port_write(&port_a, value);
            break;
        case RAT_MCU_PORT_B:
            port_write(&port_b, value);
            break;
        case RAT_MCU_PORT_C:
            port_write(&port_c, value);
            break;
        case RAT_MCU_PORT_E:
            port_write(&port_e, value);
            break;
    }
}

// -----------------------------------------------------------------------------
// Init the 16 bit timer that is needed for all timing related operations
// -----------------------------------------------------------------------------
static void rat_mcu_init_timers (void)
{
    // Set the clock source to secondary oscillator
    T1CON.TMR1CS1  = 0b1;
    T1CON.TMR1CS0  = 0b0;

    // Enable the secondary oscillator
    T1CON.T1SOSCEN = 0b1;

    // Set the prescaler to two (four seconds)
    T1CON.T1CKPS1  = 0b0;
    T1CON.T1CKPS0  = 0b1;
  
    // Do not synchronize the external clock input
    T1CON.T1SYNC   = 0b1;

    // Enable the 16-bit mode
    T1CON.T1RD16   = 0b1;

    // Disable the gate enable
    T1GCON.TMR1GE  = 0b0;

    // Turn on the timer
    T1CON.TMR1ON   = 0b1;
}

// -----------------------------------------------------------------------------
// Clear the 16 bit timer
// -----------------------------------------------------------------------------
static void rat_mcu_clear_timer (void)
{
    TMR1H = 0x00;
    TMR1L = 0x00;
}

// -----------------------------------------------------------------------------
// Enable the interrupt of the 16 bit timer
// -----------------------------------------------------------------------------
static void rat_mcu_init_interrupts (void)  
{
    // Disable the interrupt priority
    INTCON.IPEN = 0b0;

    // Enable the peripheral interrupts
    INTCON.PEIE = 0b1;

    // Enable Timer 1 interrupt
    PIE1.TMR1IE = 0b1;

    // Clear the interrupt flag of timer 1
    PIR1.TMR1IF = 0b0;

    // Enable the global interrupts
    INTCON.GIE  = 0b1;
}

// -----------------------------------------------------------------------------
// Init the internal oscillator
//
// Note that the idle mode must be enabled,
// because otherwise the microcontroller does not wake up from sleep
// after timer 1 has an overflow.
//
// Note that the settings of the mikroSDK and
// the internal oscillator must match.
//
// '111' = 16 MHz
// '110' =  8 MHz
// '101' =  4 MHz
// '100' =  2 MHz
// '011' =  1 MHz
// -----------------------------------------------------------------------------
static void rat_mcu_init_internal_oscillator (void)
{
    OSCCON.IDLEN = 1;

    OSCCON.IRCF2 = 1;
    OSCCON.IRCF1 = 1;
    OSCCON.IRCF0 = 1;
}

// -----------------------------------------------------------------------------
// Init the sleep cycle to 15 minutes
// -----------------------------------------------------------------------------
static void rat_mcu_init_sleep (void)
{
    timestamp = 0;

    sleep_cycles = RAT_MCU_SLEEP_CYCLES_DEFAULT * 60;

    sleep_cycles = sleep_cycles / RAT_MCU_SLEEP_CYCLE_LENGTH;
}

// -----------------------------------------------------------------------------
// Delay in cycles using the 16 bit timer
// -----------------------------------------------------------------------------
static void rat_mcu_delay (uint16_t limit)
{
    uint16_t cycles;

    // Set the init state of the timer
    TMR1H = 0x00;
    TMR1L = 0x00;

    // Wait until the amount of cycles has passed
    //
    // Note that the frequency is 32,768 KHz and the prescaler is two
    while (true) {
        cycles = TMR1H;
        cycles = cycles << 8;
        cycles += TMR1L;

        if (cycles > limit) {
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Delay in cycles using the 16 bit timer
// -----------------------------------------------------------------------------
void rat_mcu_delay_ms (uint16_t ms)
{
    uint32_t cycles = (uint32_t)ms;

    cycles = ((cycles * RAT_MCU_CYCLES_PER_SECOND) / RAT_MCU_MICROSECONDS_IN_SECOND) + 1;

    while (true) {
        if (cycles > RAT_MCU_CYCLES_PER_SECOND) {
            rat_mcu_delay(RAT_MCU_CYCLES_PER_SECOND);

            cycles = cycles - RAT_MCU_CYCLES_PER_SECOND;    
        } else {
            rat_mcu_delay(cycles);

            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Init the microcontroller
// -----------------------------------------------------------------------------
void rat_mcu_init (void)  
{
    rat_mcu_init_pins();
    rat_mcu_init_internal_oscillator();
    rat_mcu_init_timers();
    rat_mcu_init_interrupts();
    rat_mcu_init_sleep();

    rat_mcu_delay_ms(RAT_MCU_INIT_DELAY);
}

// -----------------------------------------------------------------------------
// Sleep one cycle
// -----------------------------------------------------------------------------
void rat_mcu_sleep_cycle (void)
{
    PIR1.TMR1IF = 0;   // Clear pending interupt flag
    PIE1.TMR1IE = 1;   // Enable timer 1 interrupt
    INTCON.PEIE = 1;   // Enable peripheral interrupts
    INTCON.GIE  = 1;   // Eanble Global interrupts

    asm SLEEP;

    PIR1.TMR1IF = 0;   // Clear pending interupt flag
} 

// -----------------------------------------------------------------------------
// Deep sleep
// -----------------------------------------------------------------------------
void rat_mcu_deep_sleep (uint8_t minutes)
{
    rat_mcu_clear_timer();

    for (uint8_t minute = 0; minute < minutes; minute++) {
        for (uint8_t cycle = 0; cycle < RAT_MCU_SLEEP_CYCLES_DEFAULT; cycle++) {
            rat_mcu_sleep_cycle();
        }
    }
}

// -----------------------------------------------------------------------------
// Set the measurement interval
// -----------------------------------------------------------------------------
void rat_mcu_measurement_interval (uint8_t interval)
{
    if (measurement_interval != interval) {
        timestamp = 0;

        measurement_interval = interval;
    }
}

// -----------------------------------------------------------------------------
// Set the transmission interval
// -----------------------------------------------------------------------------
void rat_mcu_transmission_interval (uint8_t interval)
{
    if (transmission_interval != interval) {
        timestamp = 0;

        transmission_interval = interval;
    }
}

// -----------------------------------------------------------------------------
// Check if a measurement should be performed
// -----------------------------------------------------------------------------
bool rat_mcu_measurement (void)
{
    if (timestamp % measurement_interval == 0) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Check if a transmission should be performed
// -----------------------------------------------------------------------------
bool rat_mcu_transmission (void)
{
    if (timestamp % transmission_interval == 0) {
        return true;
    } else {
        return false;
    }
}
