/*
  MorseLED.cpp v1.0 - A lightweight (uses 1 byte ram and 6 bytes stack) library for flashing international Morse Code messages on an arduino LED
  (C) Copyright 2018 (GNU GENERAL PUBLIC LICENSE, Version 3) - Chris Drake <cdrake@cpan.org>
*/


#ifndef MorseLED_h
#define MorseLED_h

#include "Arduino.h"

class MorseLED
{
  public:
    MorseLED(byte pin);
    void dot();
    void dash();
    void say(char *);				// Halts your program while it sends the morse
    void say(const __FlashStringHelper*);
    // void osay(char *);
    void die(char *);				// Sends morse error code forever (with 3s gap between repeats)
    void die(const __FlashStringHelper*);
    unsigned long delayled(unsigned long);	// does  delay() [while taking care of LED flashing]. Returns the number of ms before the next LED state needs to happen.
    void asay(char *);				// Asynchronous say - doesn't halt your program, but you need to call delayled() as often as you can
    void asay(const __FlashStringHelper*);
    void tsay(char *);				// Timer say - Same as asay, except this sets up a timer interrupt to handle the LED states without you doing anything
    void tsay(const __FlashStringHelper*);	// remember to include the ISR (see below)
    void timer1Hook();				// future - cater for a different way to hook timers
  private:
    void t1Init(void);
    void startISR();
    byte _pin;
};

/* Add this to your sketch if you're usng tsay:-

ISR(TIMER1_COMPA_vect) { // Interrupt service: runs 50 times per second to keep the Morse LED flashing 
  morse.delayled(0);
} // Timer ISR


*/

#endif

