# AVR AtMega328p Garage Door Driver
It was tested on my 12vdc battery powered with 220vca charger garage door system

##### Designed and tested by Mateo Proruk. (30/11/2025) See me later!.
</div>


## Current Status
- [x] Program Ready to Upload
- [x] Tested and Working with arduino uno board and relay module (1 day)
- [ ] Tested and Working with arduino uno board and relay module (1 month)
- [ ] Board layout design
- [ ] Tested and Working with standalone board (1 day)
- [ ] Tested and Working with standalone board (1 month)

## Documentation
-

## Description
It starts of a remote control or push switch input (PORTD2) wich starts moving the garage door motor clockiwse (PORTB1, PORTB2). after the open endstop input (PORTD3) is reached, The same behaviour occurs when closing the garage door after the close endstop input (PORTD4) is reached, it also has two auxiliary outputs for battery charger bypass while the motor is running (PORTB3, PORTB4).

## How to use
Upload main.elf to the Microcontroller (Atmega328P @ 16MHz) you can use an arduino uno board.


Inputs:
    PD2 (Digital Input 2): Garage door start switch (use a 500ohm pull-up resistor to 5v), 
    PD3 (Digital Input 3): Garage door open endstop switch (use a 500ohm pull-up resistor to 5v), 
    PD4 (Digital Input 4): Garage door close endstop (use a 500ohm pull-up resistor to 5v), 

Outputs:
    PB1 (Digital Output 9) Motor Relay: Open Garage Door Motor (Make sure to have a flyback diode 1n4007 or the coil backward pulse will trash the microcontroller), 
    PB2 (Digital Output 10) Motor Relay: Close Garage Door Motor (Make sure to have a flyback diode 1n4007 or the coil backward pulse will trash the microcontroller), 
    PB3 (Digital Output 11) Motor running for an external power supply (Make sure to have a flyback diode 1n4007 or the coil backward pulse will trash the microcontroller), 
    PB4 (Digital Output 12) Motor running for an external power supply (Make sure to have a flyback diode 1n4007 or the coil backward pulse will trash the microcontroller), 
    PB5 (Digital Output 13) Led Builtin, flickers every second to signal that the mcu is running. (Make sure to current limit the LED with a 220ohm resisor)

## IMPORTANT:
Make sure to drive relays with a suitable transistor circuit and to use flyback diodes (1n4007) in relays and (10a10) for the 12v garage door motor. Failing to protect against coil backwards voltage will result in the death of the mcu and other components of the board.

## For Devs
VSCode: I use it for writing code.
Makefile: I use it for compiling automation.
avr-gcc: Standard avrc/c++ compiler.
avrdude: To upload main.ELF to the MCU using (in my case the Arduino bootloader)
avrlibs: They come with avr-gcc, I use the api to handle register adresses and make things simple.

## Dependencys:
Makefile, avr-gcc, avrdude, avr libs