/// @file    Noise.ino
/// @brief   Demonstrates how to use noise generation on a 2D LED matrix
/// @example Noise.ino

#include <FastLED.h>

#define NUM_LEDS 60
CRGB leds[NUM_LEDS];

static uint16_t x;
static uint16_t y;
static uint16_t z;

uint16_t speed = 2;  // 1-100
uint16_t scale = 1;  // 1-4000
uint16_t noise[60];

uint16_t BRIGHTNESS = 10;

//---------------------энкодер
#define CLK 2
#define DT 3
#define SW 4

int counter = 50;
int encoder_brightness_step = 20;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;
//---------------------

String serialString = "";
int delim = 0;
int weather_num = 0;
int temperature = 0;
int move_trig, LM_state, weather_val, encoder_val = 0;

uint8_t ihue = 0;
uint8_t hue_offset = 0;
uint8_t hue = 0;
uint8_t hue_saturation = 0;
uint8_t hue_value = 0;
uint8_t color_scale = 1;

void setup() {
  Serial.begin(9600);
  delay(3000);
  FastLED.addLeds<WS2812, 13, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  x = random16();
  y = random16();
  z = random16();

  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK);
}

void fillnoise8() {
  for (int i = 0; i < 60; i++) {
    int ioffset = scale * i;
    noise[i] = inoise8(x + ioffset, 0, z);
  }
  z += speed;
}


void loop() {
  get_encoder_value();
  set_brightness_by_encoder();


  if (Serial.available() > 0) {
    sensors_and_weather_by_serial();
  }

  // speed = 1;
  // scale = 10;
  // color_scale = 1;

  set_LM_state();

  fillnoise8();
  for (int i = 0; i < 60; i++) {
    hue = ihue + (noise[i] >> color_scale) + hue_offset;
    hue_value = noise[i];

    if (LM_state == 0) {
      if ((temperature < 3) && (temperature > -3)) {
        hue_saturation = 50;
      } else if ((temperature < 6) && (temperature > -6)) {
        hue_saturation = 100;
      }
      if ((temperature < 10) && (temperature > -10)) {
        hue_saturation = 150;
      } else {
        hue_saturation = 255 - (noise[i] % 50);
      }
    } 
    leds[i] = CHSV(hue, hue_saturation, hue_value);
  }
  
  FastLED.show();
}


void set_LM_state() {
  switch (LM_state) {
    case 2:
    // 
      ihue = 60;
      hue_saturation = 160;
      break;
    case 1:
    // белый
      ihue = 0;
      hue_saturation = 0;
      break;
    case 0:
    // цвета зависят от погоды
      set_weather_num();
      set_color_by_temperature();
      break;
    default:
      set_weather_num();
      set_color_by_temperature();
      break;
  }
}

void set_weather_num() {
  switch (weather_num) {
    case 2:
      // Rain / Snow
      speed = 4;
      scale = 150;
      color_scale = 5;
      hue_offset = 0;
      break;

    case 1:
      // Clouds
      speed = 3;
      scale = 50;
      color_scale = 2;
      hue_offset = -20;
      break;

    case 0:
      // Clear
      speed = 1;
      scale = 10;
      color_scale = 1;
      hue_offset = -50;
      break;

    default:
      speed = 4;
      scale = 150;
      color_scale = 5;
      hue_offset = 0;
      break;
  }
}

void set_color_by_temperature() {
  // 0 желтый
  // 40 красный
  // 50 розовый
  // 60 фиолетовый
  // 110 синий
  // 130 голубой
  // 220 зеленый
  if (temperature >= 0) {
    // для теплой погоды - теплые цвета
    ihue = temperature + 60;
  } else {
    // для холодной погоды - холодные цвета
    ihue = temperature * 2 + 230;
  }
}

void sensors_and_weather_by_serial() {
  // на вход строка из LM_state, temperature, weather_num
  char separator = ',';
  serialString = Serial.readString();
  delim = 0;

  delim = serialString.indexOf(',');
  LM_state = serialString.substring(0, delim).toInt();
  serialString = serialString.substring(delim + 1, serialString.length());

  delim = serialString.indexOf(separator);
  temperature = serialString.substring(0, delim).toFloat();
  serialString = serialString.substring(delim + 1, serialString.length());

  delim = serialString.indexOf(separator);
  weather_num = serialString.substring(0, delim).toInt();
  serialString = serialString.substring(delim + 1, serialString.length());

  Serial.print(LM_state);
  Serial.print(separator);
  Serial.print(temperature);
  Serial.print(separator);
  Serial.println(weather_num);
}

void get_encoder_value() {
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      counter = counter - encoder_brightness_step;
    } else {
      counter = counter + encoder_brightness_step;
    }
    // Serial.print(" | Counter: ");
    // Serial.println(counter);
  }
  lastStateCLK = currentStateCLK;
  int btnState = digitalRead(SW);

  if (btnState == LOW) {
    if (millis() - lastButtonPress > 50) {
      // Serial.println("Button pressed!");
    }
    lastButtonPress = millis();
  }
}

void set_brightness_by_encoder() {
  if (counter > 255) {
    BRIGHTNESS = 255;
  } else if (counter < 0) {
    BRIGHTNESS = 0;
  } else {
    BRIGHTNESS = counter;
  }
  FastLED.setBrightness(BRIGHTNESS);
}
