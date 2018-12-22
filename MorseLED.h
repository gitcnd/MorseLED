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
    void say(char *);
    void die(char *);
    void say(const __FlashStringHelper*);
    void die(const __FlashStringHelper*);
  private:
    byte _pin;
};

#endif

