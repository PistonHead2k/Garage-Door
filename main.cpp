#ifndef __AVR__ATmega328P__
    #define __AVR__ATmega328P__
#endif

#define F_CPU 16000000UL
#define DEBUG



//FOR MathF.h Header (For Some Reason It Only Works Here)
#include <math.h>

#include "std.h"

#include "USART.h"
#include "Timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <avr/delay.h>

#include "IO.h"



IO::Debounce Debounce1;


/* Timer Subroutine */
void TS1(void)
{
    USART::Debug::SendString(ToStringBinary(PIND));
}

//Pin 2 (PCINT18 capture input interrupt)
uint8_t PCIN2;
ISR (PCINT2_vect)
{
    PCIN2 = PIND;
}

/* Executes Every 0.001024 seconds */

void FixedLoop(void)
{

}

int main(void)
{   
    sei();

    USART::Init(250000); //USART Init
    
    Timer::Start(); //Starts The Program Timer

    Timer::TIMER0_OVF_INT = &FixedLoop; //Starts FixedLoop

    
   //Ses PORTB bit 1 - 4 as Output
    Bit::Out(&DDRB, 1, OUTPUT);
    Bit::Out(&DDRB, 2, OUTPUT);
    Bit::Out(&DDRB, 3, OUTPUT);
    Bit::Out(&DDRB, 4, OUTPUT);

    //Sets PORTD2 as input w pullup
    Bit::Out(&DDRD, 2, INPUT);
    P.D2 = PULLUP;

    //Enables Pin Change Interrupts For PCIE2 (PCINT18);
    Bit::Set(&PCICR, PCIE2);

    //These registers specify which individual pins within the enabled port 
    //will trigger an interrupt. Each bit corresponds to a specific pin.
    Bit::Set(&PCMSK2, PCINT18);

    while(true)
    {
        #define bit bool
        #define flag bit

        static Timer::Subroutine T1;
        T1.Wait(0.5f, &TS1);

        bit MotorOpen;
        bit MotorClose;

        bit RemoteInput = Debounce1.InputT(~PCIN2, 2);

        static Electric::Pulse Pulse;
        MotorOpen = Pulse.Rising(RemoteInput);

        P.B1 = MotorOpen;
        P.B2 = MotorClose;


        //Timer::DeltaT(Timer::Micros()); //Calculates Loop DT
        
        P.BitsToRegister();
    }
}