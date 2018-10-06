/**************************************************************************
 *                                                                        *    
 *   NeoPixel Ring Clock                                                  *
 *   by Andy Doro (mail@andydoro.com)                                     *
 *   http://andydoro.com/ringclock/                                       *
 *                                                                        *
 **************************************************************************
 *
 *
 * Revision History
 *
 * Date    By    What
 * 20140320   AFD   First draft
 *
 * 20160602   updated by Ralf Hager
 */

// include the library code:
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

// define pins
#define NEOPIN 9

#define BRIGHTNESS 255 // set max brightness

RTC_DS3231 RTC; // Establish clock object
DateTime Clock; // Holds current clock time

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEOPIN, NEO_GRB + NEO_KHZ800); // strip object

byte hourval, minuteval, secondval; // holds the time

byte pixelColorRed, pixelColorGreen, pixelColorBlue; // holds color values


void setup () {
  Wire.begin();         // Begin I2C

  // set pinmodes
  pinMode(NEOPIN, OUTPUT);

  // set Serial Speed to 9600 Baud
  Serial.begin(9600);
  
  delay(3000); // wait for console opening

  if (RTC.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(BRIGHTNESS); // set brightness

  // startup sequence
  delay(500);
  colorWipe(strip.Color(255, 0, 0), 20); // Red
  colorWipe(strip.Color(0, 255, 0), 20); // Green
  colorWipe(strip.Color(0, 0, 255), 20); // Blue
  delay(500);

}

void loop () {

  // get time
  Clock = RTC.now(); // get the RTC time

  secondval = Clock.second();  // get seconds  
  minuteval = Clock.minute();  // get minutes
  hourval = Clock.hour();   // get hours
  if(hourval > 11) hourval -= 12; // This clock is 12 hour, if 13-23, convert to 0-11

  hourval = (hourval*60 + minuteval) / 12;  //each red dot represent 24 minutes.

  // arc mode
  for(int i=0; i<strip.numPixels(); i++) {
  
    if (i == ((secondval+59) % 60)) {
      pixelColorBlue = 31;
    }
    else
    if (i == ((secondval+1) % 60)) {
      pixelColorBlue = 31;
    }
    else
    if (i == secondval) {
      pixelColorBlue = 255;
    }
    else {
      pixelColorBlue = 0;
    }
  
    if (i == ((minuteval+59) % 60)) {
      pixelColorGreen = 31;  
    }
    else
    if (i == ((minuteval+1) % 60)) {
      pixelColorGreen = 31;  
    }
    else
    if (i == minuteval) {
      pixelColorGreen = 255;  
    }
    else {
      pixelColorGreen = 0;
    }
  
    if (i == ((hourval+59) % 60)) {
      pixelColorRed = 31;   
    }
    else
    if (i == ((hourval+1) % 60)) {
      pixelColorRed = 31;   
    }
    else
    if (i == hourval) {
      pixelColorRed = 255;   
    }
    else {
      pixelColorRed = 0;
    }
  
    strip.setPixelColor(i, strip.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
    
  }

  /*
  // for serial debugging
   Serial.print(Clock.hour(), DEC);
   Serial.print(':');
   Serial.print(Clock.minute(), DEC);
   Serial.print(':');
   Serial.println(Clock.second(), DEC);
  */
  
  //display
  strip.show();
 
  // wait
  delay(100);

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
