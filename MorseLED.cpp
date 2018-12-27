// works

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

byte _abit=0x8F;		// Which bit in the morse are we up to (0x8F means idle)
char *_aptr=0;			// Which byte in the output string are we upto
//String *_sptr=0;		// Same as above, for PROGMEM strings
//const __FlashStringHelper *_sptr=0;		// Same as above, for PROGMEM strings
char *_sptr=0;		// Same as above, for PROGMEM strings
unsigned long nextmilli=0;	// When to advance to the next async stage

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
  //_abit = pin;
}

void MorseLED::dot()
{
  digitalWrite(_pin&0x3F, HIGH);
  delay(DOTLEN);
  digitalWrite(_pin&0x3F, LOW);
  delay(SPACELEN);  
}

void MorseLED::dash()
{
  digitalWrite(_pin&0x3F, HIGH);
  delay(DASHLEN);
  digitalWrite(_pin&0x3F, LOW);
  delay(SPACELEN);
}

byte char_to_morse(char c) { // Convert ASCII into an index into MorseTable, then to morse bits
  if(c==0) return 0;
  c=c & 0b11011111; // a => A
  if(c>'Z') c=10;		// All non A-Z0-9 are considered spaces
  else if(c<16) c=10;		// "0"(48) wen to 16 from the & 0b11011111;
  else if(c<26) c-=16;		// Convert ascii digits into binary
  else if(c<'A') c=10;
  else c=c-('A'-11);		// Convert letters into an index starting at item 11
  return pgm_read_byte_near(MorseTable+c); // Load the morse-code symbols for this character
} // char_to_morse

void MorseLED::say(char *msg) {
  asay(msg); while(delayled(delayled(UNITLEN))); // delayled returns 0 at the end, otherwise, returns the # of millis to wait
}

/* Old simplistic delay code - works:

void MorseLED::say(char *msg)
{
  // replace with asay(msg); followed by: while(delayled(delayled(0)); // delayled returns 0 at the end, otherwise, returns the # of millis to wait

  for(unsigned int i=0;i<strlen(msg);i++) {
    byte c=msg[i] & 0b11011111; // a => A

    // Convert ASCII into an index into MorseTable
    if(c>'Z') c=10;		// All non A-Z0-9 are considered spaces
    else if(c<16) c=10;		// "0"(48) went to 16 from the & 0b11011111;
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
*/

void MorseLED::say(const __FlashStringHelper* msg) {
  byte c[2];c[1]=0; const char *s=(const char*)msg;
  while(c[0]=pgm_read_byte_near(s++)) say((char*)c);
} // say from PROGMEM


void MorseLED::die(char *msg) {
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



unsigned long MorseLED::delayled(unsigned long tdelay) { // call this as often as you can - does the LED blinking states when the times are right
  unsigned long now=micros()/1000; // millis();
  unsigned long started_loop=now;

  do {
    if(now>=nextmilli) {	// Time to change the LED state
      if(_abit&0b10000000) {	// LED is already on, so we now need to turn it off, and wait for a space
	nextmilli=now+SPACELEN;
	if(_abit!=0x8F) {	// not 0x8F indicates we are busy (not idle)
	  _abit=_abit&0b01111111; // remember the LED is now off
	  digitalWrite(_pin&0x3F, LOW);
	} else {		// idle
	  nextmilli=now-1; if(tdelay>0)delay(tdelay); // wait, if we were told, and tell caller it's all over now too (nextmilli will casue a return 0 at the end)
	}
      } else {			// LED is off, time to turn it on, and decide if we're waiting for a dot or dash length of time
        byte m=0; if(_aptr!=0)m=char_to_morse(*_aptr); else if(_sptr!=0) m=char_to_morse(pgm_read_byte_near((const __FlashStringHelper *)_sptr));  // Get the morse we're upto (0=end)
	byte b=1<<(_abit-1);    // this will be used to step through the bits in the symbol
	unsigned long wordspace=0;
	if(_abit==8){		// Find the start of the morse code
	  for(b=0b10000000;(b>0)&&((b&m)==0);b=b>>1)_abit--; // find the start of the morse code lookup table
	  b=b>>1; _abit--;	// Skip the leading 1 (this indicates the start of the morse dots and dashes, which are variably-lengthed sequences)
	  wordspace=WORDLEN-LETTERLEN; // remember how much extra to wait in between words
	}
	if((b==0)||(_abit==0)) { // end of a letter
	  if(m!=0){_abit=8;if(_aptr!=0)_aptr++; if(_sptr!=0)_sptr++;} else {_abit=0x8F; } // advance to next char, unless we're at the end, in which case just repeatedly do this delay step now
	  nextmilli=now+(LETTERLEN-SPACELEN)+wordspace; // work out when the next letter needs to start (we already waited SPACELEN just before)
	} else {
	  digitalWrite(_pin&0x3F, HIGH);
	  if(b&m) nextmilli=now+DOTLEN; else nextmilli=now+DASHLEN;
	  _abit--;
	  _abit=_abit|0b10000000;	// remember it's on now
	}
      }
    }

    if(tdelay>0) { // don't do any delay() calls in ISR or when caller didn't ask
      if(((now+tdelay) <= nextmilli)) delay(tdelay); // wait whichever is the shortest amount of time
      else if(nextmilli>now) delay(nextmilli-now);
    }

    now=micros()/1000-1; // millis()-1;
  } while((started_loop+tdelay)>now);


  if(now>=nextmilli) if(_abit==0x8F)return 0;else return 1; // only return 0 when we've reached the end
  return nextmilli-now; // Tell caller how long until the next LED state change needs to happen
} // delayled


void MorseLED::asay(char *msg) {
  _abit=0x88;			// Which bit in the morse are we up to
  _aptr=msg;
  _sptr=0;
  nextmilli=micros()/1000; // millis();	// When to advance to the next async stage
}
void MorseLED::asay(const __FlashStringHelper* msg) {
  _abit=0x88;			// Which bit in the morse are we up to
  _aptr=0;
  _sptr=(char *)msg;
  nextmilli=micros()/1000; //millis();	// When to advance to the next async stage
}


//void MorseLED::timer1Hook(void (*function)(void)) {
//  __timer1Hook = function;
//}


void MorseLED::startISR() {
  if((_pin&0b10000000)==0) {
    _pin|=0b10000000; // remember we did this
    // initialize timer1 so that it calls our TIMER1_COMPA_vect() ISR 50 times per second.
    noInterrupts();           // disable all interrupts
    TCCR1A = 0; 
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A = 1250;		// compare match register 16MHz/256/1250 = 50hz
    TCCR1B |= (1 << WGM12);   // CTC mode
    TCCR1B |= (1 << CS12);    // 256 prescaler 
    TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
    interrupts();             // enable all interrupts
  }
} //startISR

void MorseLED::tsay(char *msg) {	// timer-say (asynchronous)
  MorseLED::startISR();
  asay(msg);
}
void MorseLED::tsay(const __FlashStringHelper* msg) {
  MorseLED::startISR();
  asay(msg);
}

/* Timer version:



https://www.instructables.com/id/Arduino-Timer-Interrupts/

so if you wanted an interrupt every second (frequency of 1Hz):
compare match register = [16,000,000 / (prescaler * 1) ] -1
with a prescaler of 1024 you get:
compare match register = [16,000,000 / (1024 * 1) ] -1
= 15,624
since 256 < 15,624 < 65,536, you must use timer1 for this interrupt.






  // initialize timer1 so that it calls our TIMER1_COMPA_vect() ISR 1008 times per second. Note that our settings do not interfere with millis() or delay();
  noInterrupts();           // disable all interrupts
  TCCR1A = 0; 
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 61;               // compare match register 16MHz/256/2Hz.  312 = 200 per second. 61=1008 per second
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  configADC(); // setup to read fast from A0..An using interrupts
  interrupts();             // enable all interrupts




// *   Timer ISR.   *
ISR(TIMER1_COMPA_vect) // Interrupt service: runs when Timer/Counter1 reaches OCR1A (your own top value). OCR1A = 61; means 1008 interrupts per second
{
  if(adc_run)ADCSRA |= B01100000;	// tap the bit needed to commence the next ADC conversion
  serviceLED();				// short blinks on the LED occasionally so we know we are alive - NB - Do not remove - we need the above bit-tap to be separated from the below un-tap
  ADCSRA &= B10011111;			// un-tap the ADC voncersion bit - we only want a single sample, not continuous
} // Timer ISR


*/
