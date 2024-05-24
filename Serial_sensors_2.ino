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


int current_brightness = 10;


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);

  pinMode(LED, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  strip.begin();
  strip.show();
  show_default_state();
}


// void loop() {

//   long current_time = millis();

//   if (Serial.available() > 0) {
//     serial_resieve_time = millis();

//     ledVals = Serial.readString();
//     Serial.print(ledVals);
//     Serial.print("-");
//     delim = 0;

//     delim = ledVals.indexOf(',');
//     move_trig = ledVals.substring(0, delim).toInt();
//     ledVals = ledVals.substring(delim + 1, ledVals.length());

//     delim = ledVals.indexOf(',');
//     LM_trig = ledVals.substring(0, delim).toInt();
//     ledVals = ledVals.substring(delim + 1, ledVals.length());

//     delim = ledVals.indexOf(',');
//     weather_val = ledVals.substring(0, delim).toInt();
//     ledVals = ledVals.substring(delim + 1, ledVals.length());

//     delim = ledVals.indexOf(',');
//     encoder_val = ledVals.substring(0, delim).toInt();
//     ledVals = ledVals.substring(delim + 1, ledVals.length());

//     // Serial.print(move_trig);
//     // Serial.print(LM_trig);
//     // Serial.print(weather_val);
//     // Serial.println(encoder_val);

//   }
//   if (move_trig==1){
//     strip.setBrightness(200);
//     strip.show();
//     if (current_time - serial_resieve_time > after_move_time){
//       move_trig = 0;
//     }
//   } else {
//     show_default_state();
//   }
//   colorWipe(strip.Color(255, 0, 0), 50); // Red
// }

void show_strip(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
    strip.show();
  }
}

void show_default_state() {
  current_brightness = 5;
  show_strip(255, 255, 255);
}


// ------------------------------------------
unsigned long pixelPrevious = 0;    // Previous Pixel Millis
unsigned long patternPrevious = 0;  // Previous Pattern Millis
int patternCurrent = 0;             // Current Pattern Number
int patternInterval = 5000;         // Pattern Interval (ms)
bool patternComplete = false;

int pixelInterval = 50;            // Pixel Interval (ms)
int pixelQueue = 0;                // Pattern Pixel Queue
int pixelCycle = 0;                // Pattern Pixel Cycle
uint16_t pixelNumber = LED_COUNT;  // Total Number of Pixe

unsigned long currentMillis = millis();  

int r = 200;
int g = 250;
int b = 50;


void loop() {
  set_color(10, 0, 200);

  strip.setBrightness(current_brightness);
  currentMillis = millis();
  if (patternComplete || (currentMillis - patternPrevious) >= patternInterval) {
    patternComplete = false;
    patternPrevious = currentMillis;
    // patternCurrent++;
  }


  if (Serial.available() > 0) {
  serial_resieve_time = millis();
  parse_sensors_data();
    // patternCurrent = Serial.parseInt();
    if ((patternCurrent > 3) || (patternCurrent <= 0)) {
      patternCurrent = 0;
    }
    patternPrevious = currentMillis;
    strip.clear();
  }
  if (move_trig==1){
      current_brightness = 200;
      if (currentMillis - serial_resieve_time > after_move_time){
        current_brightness = 10;
        move_trig = 0;
      }
  }
  if (LM_trig==1){
      patternCurrent++;
      LM_trig=0;
  }
  if (encoder_val==1){
      set_color(255, 0, 155);
      encoder_val=0;
  }


    switch_states();

}




void switch_states() {// code below assumes 0 <= h < 360. Otherwise wrap the value before

  if (currentMillis - pixelPrevious >= pixelInterval) {  //  Check for expired time
    pixelPrevious = currentMillis;                       //  Run current frame
    switch (patternCurrent) {
      case 3:
        theaterChaseRainbow(50);  // Rainbow-enhanced theaterChase variant
        break;
      case 2:
        rainbow(10);  // Flowing rainbow cycle along the whole strip
        break;
      case 1:
        theaterChase(strip.Color(r,g,b), 50);
        break;
      default:
        show_strip(r,g,b);
        break;
    }
  }
}

void set_color(int red, int green, int blue) {
  if ( r != red || g != green || b != blue ) {
    if ( r < red ) r += 1;
    if ( r > red ) r -= 1;

    if ( g < green ) g += 1;
    if ( g > green ) g -= 1;

    if ( b < blue ) b += 1;
    if ( b > blue ) b -= 1;
  }
}






void parse_sensors_data() {
  if (Serial.available() > 0) {

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
    // Serial.print(encoder_val);
    Serial.println();
  }
}



















// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  static uint32_t loop_count = 0;
  static uint16_t current_pixel = 0;

  pixelInterval = wait;  //  Update delay time

  strip.clear();

  for (int c = current_pixel; c < pixelNumber; c += 3) {
    strip.setPixelColor(c, color);
  }
  strip.show();

  current_pixel++;
  if (current_pixel >= 3) {
    current_pixel = 0;
    loop_count++;
  }

  if (loop_count >= 10) {
    current_pixel = 0;
    loop_count = 0;
    patternComplete = true;
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(uint8_t wait) {
  if (pixelInterval != wait)
    pixelInterval = wait;
  for (uint16_t i = 0; i < pixelNumber; i++) {
    strip.setPixelColor(i, Wheel((i + pixelCycle) & 255));  //  Update delay time
  }
  strip.show();  //  Update strip to match
  pixelCycle++;  //  Advance current cycle
  if (pixelCycle >= 256)
    pixelCycle = 0;  //  Loop the cycle back to the begining
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  if (pixelInterval != wait)
    pixelInterval = wait;  //  Update delay time
  for (int i = 0; i < pixelNumber; i += 3) {
    strip.setPixelColor(i + pixelQueue, Wheel((i + pixelCycle) % 255));  //  Update delay time
  }
  strip.show();
  for (int i = 0; i < pixelNumber; i += 3) {
    strip.setPixelColor(i + pixelQueue, strip.Color(0, 0, 0));  //  Update delay time
  }
  pixelQueue++;  //  Advance current queue
  pixelCycle++;  //  Advance current cycle
  if (pixelQueue >= 3)
    pixelQueue = 0;  //  Loop
  if (pixelCycle >= 256)
    pixelCycle = 0;  //  Loop
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
