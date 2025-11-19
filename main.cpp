#ifndef __AVR__ATmega328P__
    #define __AVR__ATmega328P__
#endif

#define F_CPU 16000000UL
#define DEBUG

#include <math.h> //for fabs for some reason only works here
#include <avr/io.h>
#include <avr/interrupt.h>

#include "std.h"

#include "USART.h"
#include "Timer.h"

#include "IO.h"

IO::Debounce Debounce1, Debounce2, Debounce3;

#if defined(DEBUG) 
/* Timer Subroutine */
void TS1(void)
{
    USART::Debug::SendString(ToStringBinary(PIND));
}
#endif

//Pin 2 (PCINT18 capture input interrupt)
uint8_t PCIN2;
ISR (PCINT2_vect)
{
    PCIN2 = PIND;
}

/* Executes Every 0.001024 seconds */
void FixedLoop(void)
{
//Unused. Stuff Here Executes Slower
}

int main(void)
{   
    //Enables Interrupt Handler to Interrupt CPU
    sei();

    USART::Init(250000); //USART Init
    
    Timer::Start(); //Starts The Program Timer

    Timer::TIMER0_OVF_INT = &FixedLoop; //Starts FixedLoop

    
   //Ses PORTB bit 1 - 4 as Output
    Bit::Out(&DDRB, 1, OUTPUT); //Motor Relay 1
    Bit::Out(&DDRB, 2, OUTPUT); //Motor Relay 2
    Bit::Out(&DDRB, 3, OUTPUT); //Aux
    Bit::Out(&DDRB, 4, OUTPUT); //Aux

    //Sets PORTB bit 1 - 4 HIGH to make relays LOW on the first cycle;
    Bit::Out(&PORTB, 1, HIGH);
    Bit::Out(&PORTB, 2, HIGH);
    Bit::Out(&PORTB, 3, HIGH);
    Bit::Out(&PORTB, 4, HIGH);

    //Sets PORTD2-4 as input w pullup
    Bit::Out(&DDRD, 2, INPUT); //Controller Input
    P.D2 = PULLUP;
    Bit::Out(&DDRD, 3, INPUT); //Open Endstop Input
    P.D3 = PULLUP;
    Bit::Out(&DDRD, 4, INPUT); //Close Endstop Input
    P.D4 = PULLUP;
    

    //Enables Pin Change Interrupts For PCIE2 (PCINT18-20);
    Bit::Set(&PCICR, PCIE2);

    //These registers specify which individual pins within the enabled port 
    //will trigger an interrupt. Each bit corresponds to a specific pin.
    Bit::Set(&PCMSK2, PCINT18);
    Bit::Set(&PCMSK2, PCINT19);
    Bit::Set(&PCMSK2, PCINT20);

    while(true)
    {
        //1 bit definitions
        #define bit bool
        #define flag bit

        //Debug Timed Subroutine
        #if defined(DEBUG) 
        static Timer::Subroutine T1;
        T1.Wait(0.5f, &TS1);
        #endif

        /* Remote Control Pulse Input */
        bit RemoteInput = Debounce1.Input(~PCIN2, 2);
        /* Endstop Open Pulse Input */
        bit EndOpenInput = Debounce2.InputR(~PCIN2, 3);
        /* Endstop Close Pulse Input */
        bit EndCloseInput = Debounce3.InputR(~PCIN2, 4);
        
        //Debug USART
        #if defined(DEBUG) 
        if (RemoteInput) USART::Debug::SendString("REMOTE IN \n");
        if (EndOpenInput) USART::Debug::SendString("OPEN ENDSTOP \n");
        if (EndCloseInput) USART::Debug::SendString("CLOSE ENDSTOP \n");
        #endif


        //Start Moving Garage Door Flip Flop
        static flag Start;
        static Electric::Pulse Pulse0;
        Start ^= Pulse0.Falling(RemoteInput);

        // Motor Clockwise Bit
        bit MotorOpen;
        // Motor CounterClockwise Bit
        bit MotorClose;

        //Endstop XOR to Start Moving
        Start ^= MotorOpen & EndOpenInput;
        Start ^= MotorClose & EndCloseInput;

        //Spin Direction Flip Flop
        static Electric::Pulse Pulse1;
        flag Dir;
        Dir ^= Pulse1.Falling(Start);

        //Start Moving and Spin Direction
        MotorOpen = Dir & Start;
        MotorClose = !Dir & Start;
        
        //Motor Relay Control
        P.B1 = !MotorOpen;
        P.B2 = !MotorClose;

        //Power Supply Bridge Output for charging battery while the motor runs
        P.B3 = !(MotorOpen | MotorClose);
        P.B4 = P.B3;

        //Timer::DeltaT(Timer::Micros()); //Calculates Loop DT

        //OUT GPIO
        P.BitsToRegister();
    }
}