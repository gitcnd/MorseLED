/*
  MorseLED.cpp v1.0 - A lightweight (uses 1 byte ram and 6 bytes stack) library for flashing international Morse Code messages on an arduino LED
  (C) Copyright 2018 (GNU GENERAL PUBLIC LICENSE, Version 3) - Chris Drake <cdrake@cpan.org>
*/

#include "Arduino.h"
#include "MorseLED.h"

#define UNITLEN 250		// Milliseconds per dot
#define DOTLEN UNITLEN
#define DASHLEN (UNITLEN * 3)	// International standard
#define SPACELEN UNITLEN	// International standard
#define LETTERLEN (UNITLEN * 3)	// International standard
#define WORDLEN (UNITLEN * 7)	// International standard

const byte MorseTable[] PROGMEM = {	// 1=dot, 0=dash.  ignore all leading 0 and the first leading 1
	0b00100000,	// 0	- - - - -
	0b00110000,	// 1	. - - - -
	0b00111000,	// 2	. . - - -
	0b00111100,	// 3	. . . - -
	0b00111110,	// 4	. . . . -
	0b00111111,	// 5	. . . . .
	0b00101111,	// 6	- . . . .
	0b00100111,	// 7	- - . . . 
	0b00100011,	// 8	- - - . .
	0b00100001,	// 9	- - - - .
	0b00000001,	// word space		10
	0b00000110,	// A	. -		11
	0b00010111,	// B	- . . .		12
	0b00010101,	// C	- . - .		13
	0b00001011,	// D	- . .		14
	0b00000011,	// E	.		15
	0b00011101,	// F	. . - .		16
	0b00001001,	// G	- - .		17
	0b00011111,	// H	. . . .		18
	0b00000111,	// I	. .		19
	0b00011000,	// J	. - - -		20
	0b00001010,	// K	- . -		21
	0b00011011,	// L	. - . .		22
	0b00000100,	// M	- -		23
	0b00000101,	// N	- .		24
	0b00001000,	// O	- - -		25
	0b00011001,	// P	. - - .		26
	0b00010010,	// Q	- - . -		27
	0b00001101,	// R	. - .		28
	0b00001111,	// S	. . .		29
	0b00000010,	// T	-		30
	0b00001110,	// U	. . -		31
	0b00011110,	// V	. . . -		32
	0b00001100,	// W	. - -		33
	0b00010110,	// X	- . . -		34
	0b00010100,	// Y	- . - -		35
	0b00010011,	// Z	- - . .		36
}; // MorseTable


MorseLED::MorseLED(byte pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void MorseLED::dot()
{
  digitalWrite(_pin, HIGH);
  delay(DOTLEN);
  digitalWrite(_pin, LOW);
  delay(SPACELEN);  
}

void MorseLED::dash()
{
  digitalWrite(_pin, HIGH);
  delay(DASHLEN);
  digitalWrite(_pin, LOW);
  delay(SPACELEN);
}


void MorseLED::say(char *msg)
{
  for(unsigned int i=0;i<strlen(msg);i++) {
    byte c=msg[i] & 0b11011111; // a => A

    // Convert ASCII into an index into MorseTable
    if(c>'Z') c=10;		// All non A-Z0-9 are considered spaces
    else if(c<16) c=10;		// "0"(48) wen to 16 from the & 0b11011111;
    else if(c<26) c-=16;	// Convert ascii digits into binary
    else if(c<'A') c=10;
    else c=c-('A'-11);		// Convert letters into an index starting at item 11
    c=pgm_read_byte_near(MorseTable+c); // Load the morse-code symbols for this character
    byte b;	// this will be used to step through the bits in the symbol
    for(b=0b10000000;(b>0)&&((b&c)==0);b=b>>1); // find the start of the morse code lookup table
    b=b>>1;	// Skip the leading 1 (this indicates the start of the morse dots and dashes, which are variablly-lengthed sequences)
    if(!b)delay(WORDLEN-LETTERLEN);	// A new word (we already delayed LETTERLEN earlier)
    for(;(b>0);b=b>>1) {	// Go through all the dots and dashes
      if(b&c) dot(); else dash();
    }
    delay(LETTERLEN-SPACELEN);
  }
} // say

void MorseLED::say(const __FlashStringHelper* msg) {
  byte c[2];c[1]=0; const char *s=(const char*)msg;
  while(c[0]=pgm_read_byte_near(s++)) say((char*)c);
} // say from PROGMEM


void MorseLED::die(char *msg)
{
  while(1) {
    say(msg);
    delay(3000);
  }
}
void MorseLED::die(const __FlashStringHelper* msg) {
  while(1) {
    say(msg);
    delay(3000);
  }
}
