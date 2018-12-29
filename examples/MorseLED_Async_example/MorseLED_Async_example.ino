/*
  MorseLED_Async_example.pde v1.0 - example use of MorseLED async calls
  (C) Copyright 2018 (GNU GENERAL PUBLIC LICENSE, Version 3) - Chris Drake <cdrake@cpan.org>
*/

const int led=13;	// what pin is our L.E.D. hooked up to?

#include <MorseLED.h>
MorseLED morse(led); 	// Set the LED pin to 13


void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  morse.say("e2");	// 'e' is a single dot, and numbers are easy in morse-code; '2' is dot dot dash dash dash
  Serial.println(F("Ok!"));
  delay(2000);
} // setup


// To use tsay() in MorseLED, you need this following line:
ISR(TIMER1_COMPA_vect) { morse.delayled(0); } // Timer ISR


int i=0;
void loop() {
  Serial.println(i++);
  if(i==3) morse.tsay(F("SOS 123. This is an example of saying stuff asynchronously using the MorseLED library.  Notice how the LED blinks out the Morse even though this loop is busy printing numbers and doing delay()."));
  delay(1000);
} // loop
