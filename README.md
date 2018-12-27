# MorseLED
MorseLED is a lightweight library for flashing International Morse Code messages on an arduino LED

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

  asay("hi");		// async version of say; ensure you don't overwrite the string before its finished being said
  asay(F("hi"));	
  delayled(1000);	// use this instead of delay() if you need to delay.  use delayled(0); in your loops even if you don't need delay

  tsay("hi");           // timer-interrupt driver version of say; ensure you include the ISR line (below) of code in your sketch to make this work.
  tsay(F("hi"));	// tsay sets up the timer interrupt for you (at 50hz)

  ISR(TIMER1_COMPA_vect) { morse.delayled(0); } // Timer ISR

```


# HOW TO INSTALL

(Method 1)

1. Open a terminal (commandline, bash shell, whatever)
2. Change into your Arduino folder
```bash
   cd /Applications/Arduino.app/Contents/Java/libraries/
   (or)
   cd ~/Arduino/libraries/
```
3. grab and install this code
```bash
   git clone https://github.com/gitcnd/MorseLED.git
```
4. restart your arduino IDE
5. Choose File => Examples => MorseLED => hello
6. Hit the "build" button and enjoy!

(Method 2) - see https://www.arduino.cc/en/Guide/Libraries

1. Download the ZIP of this repo: https://github.com/gitcnd/MorseLED/archive/master.zip
2. In your IDE, select Sketch -> Include Library -> Add .ZIP Library
3. Choose File => Examples => MorseLED => hello
4. Hit the "build" button and enjoy!
