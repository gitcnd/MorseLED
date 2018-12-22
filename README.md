# MorseLED
MorseLED is a *very* lightweight library for flashing International Morse Code messages on an arduino LED

This uses 1 byte of RAM, 300 bytes (<1%) of flash storage, and temporarily uses no more than 6 bytes of stack (10bytes if using PROGMEM calls).


# SYNOPSIS

```C
  #include <MorseLED.h>
  MorseLED morse(13); // Set the pin the LED is hooked up to

  void setup() {                
    pinMode(13, OUTPUT);     
  }

  void loop() {
    morse.say("Hello 123");  
  }
```

# FUNCTIONS

```C
  say("some string");	// flashes out the given message
  die("fatal message");	// same as say(), but does this in a loop forever
  say(F("some string"));// understands PROGMEM
  dot(); // flashes a dot
  dash();// flashes a dash
```
