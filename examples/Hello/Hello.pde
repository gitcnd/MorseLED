#include <MorseLED.h>

MorseLED morse(13);

const char VERSION[] PROGMEM="\n#\tv2.00 " __FILE__ "\t" __DATE__ " " __TIME__;

void setup() {
  Serial.begin(115200);while((!Serial)&&(micros()<2000000)); Serial.println((__FlashStringHelper*)VERSION);
#ifdef TEENSYDUINO
  delay(5000); // wait 5s for PC to re-connect serial
  Serial.println((__FlashStringHelper*)VERSION);
#endif

}

void loop()
{
  morse.say("Hello 123");
  morse.say(F(" SOS");
  delay(3000);
}

