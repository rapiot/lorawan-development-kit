# lorawan-sdk

## Introduction

The purpose of the LoRaWAN Development Kit is to provide the basic development software components for the implementation of ultra low power wireless sensor nodes for private and public LoRaWAN networks.

## Components

The default microcontroller is Microchip PIC18(L)F26K22 and the default LoRaWAN radio module is RAK Wireless RAK3172. In the future, more microcontrollers and radio modules will be added.
The focus of the development is on microcontrollers and radio modules which use minimal amount of power to enable as long battery life as possible.

The following sensors are supported at this moment:

- Sensirion SHT40 temperature and humidity sensor
- Sensirion STCC4 temperature, humidity, and carbon dioxide sensor
- K-type thermocouple sensors

## Integrated Development Environment

The default IDE is Necto Studio from Mikroelektronika and the default compiler is mikroC for PIC. The latest versions are supported unless otherwise stated.

## Reference Designs

The following reference designs are available at this moment:

- Indoor monitor (one temperature and humidity sensor)
- Environment monitor (one temperature and humidity sensor and one carbon dioxide sensor)
- Thermocouple monitor (two analog front ends for K type thermocouple sensors)

The following hardware development tools are available:

- Hardware development board
    - Headers for input and outputs
    - Connector for laboratory power supply
    - 28 and 40 pin sockets
    - Two secondary oscillators
    - Six mikroBUS connectors
    - 36 leds
    - Reset button

## License

The LoRaWAN SDK is published under Creative Commons Attributions ShareAlike 4.0 license unless otherwise stated (CC-BY-SA 4.0). For further details, please see the license:

https://creativecommons.org/licenses/by-sa/4.0/legalcode
