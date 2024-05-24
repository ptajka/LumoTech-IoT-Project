// в монитор порта вводить 1,1,0,0
// это переменные  move_trig, LM_trig, weather_val, encoder_val
// обрабатываются только move_trig, LM_trig

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif


#define LED_PIN 13
#define LED_COUNT 60
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int LED = 13;
int state = 0;

int button = 4;
String ledVals = "";
int delim = 0;
int move_trig, LM_trig, weather_val, encoder_val;

long serial_resieve_time = millis();
int after_move_time = 1000;

int r = 255;
int g = 255;
int b = 255;


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);

  pinMode(LED, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  strip.begin();
  strip.show();
  strip.setBrightness(75);
  show_default_state();
}


void loop() {

  long current_time = millis();

  if (Serial.available() > 0) {
    serial_resieve_time = millis();

    ledVals = Serial.readString();
    Serial.print(ledVals);
    Serial.print("-");
    delim = 0;

    delim = ledVals.indexOf(',');
    move_trig = ledVals.substring(0, delim).toInt();
    ledVals = ledVals.substring(delim + 1, ledVals.length());

    delim = ledVals.indexOf(',');
    LM_trig = ledVals.substring(0, delim).toInt();
    ledVals = ledVals.substring(delim + 1, ledVals.length());

    delim = ledVals.indexOf(',');
    weather_val = ledVals.substring(0, delim).toInt();
    ledVals = ledVals.substring(delim + 1, ledVals.length());

    delim = ledVals.indexOf(',');
    encoder_val = ledVals.substring(0, delim).toInt();
    ledVals = ledVals.substring(delim + 1, ledVals.length());

    // Serial.print(move_trig);
    // Serial.print(LM_trig);
    // Serial.print(weather_val);
    // Serial.println(encoder_val);

  }
  if (move_trig==1){
    strip.setBrightness(200);
    strip.show();
    if (current_time - serial_resieve_time > after_move_time){
      move_trig = 0;
    }
  } else {
    show_default_state();
  }
  colorWipe(strip.Color(255, 0, 0), 50); // Red
}

void show_strip(int r, int g, int b){
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r, g, b));
      strip.show();
    }
}

void show_default_state(){
    strip.setBrightness(5);
    show_strip(255, 255, 255);
}


// ------------------------------------------
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

