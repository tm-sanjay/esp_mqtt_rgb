#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 5
#define DATA_PIN D2

int brightness = 0;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(9600);
  Serial.print("WA2812 LED");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
//  FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() { 
  
}

void setLedBrightness() {
  while(Serial.available()>0) {
    char inString = (char)Serial.read();
    int inInt = 0;
//    String inString = Serial.readString();
//    int inInt = inString.toInt();
    Serial.print(inString);
    
    if((inInt > 0) && (inInt < 255)) {
      brightness = inInt;
    }
    else if(inString == '+') {
      brightness = brightness + 10;
      if(brightness >= 255) {
        brightness = 255;
      }
    }
    else if(inString == '-') {
      brightness = brightness - 10;
      if(brightness <= 0) {
        brightness = 0;
      }
    }
    
    Serial.println("Brightness:" + String(brightness));
  }
  analogWrite(NUM_LEDS, brightness);
  FastLED.setBrightness(brightness);
  setColor();
}
//set color individually
void setColor(){
  leds[0] = CRGB(255,0,0);
  leds[1] = CRGB(0,255,0);
  leds[2] = CRGB(0,0,255);
  leds[3] = CRGB(100,100,0);
  leds[4] = CRGB(0,100,100);
  FastLED.show();
//  delay(1000);
}

//blink led one by one
void ledBlink() {
  // Turn the LED on, then pause
  for(int i=0; i<5; i++) {
    leds[i] = CRGB::Red;
    leds[i+1] = CRGB::Blue;
    leds[i+2] = CRGB::Green;
    FastLED.show();
    delay(500);
  }
  // Now turn the LED off, then pause
  for(int i=0; i<5; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(500);
  }
}
