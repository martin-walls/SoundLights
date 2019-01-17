#include <Adafruit_NeoPixel.h>


const int LED_PIN = 6;
const int NUMPIXELS = 12;
const int DC_OFFSET = 0;
const int NOISE = 100;
const int SAMPLES = 60;
const int TOP = NUMPIXELS + 1;
const int DETECTOR_PIN = 0;

byte
    peak = 0,
    dotCount = 0,
    volCount = 0;

int
    vol[SAMPLES],
    lvl = 10,
    minLvlAvg = 0,
    maxLvlAvg = 512;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
    // initialise NeoPixel library
    memset(vol, 0, sizeof(int)*SAMPLES);
    pixels.begin();
    // Serial.begin(9600);
}

void loop() {
    


    // int val = analogRead(DETECTOR_PIN);
    // if (abs(val - lastval) > 20) {
    //     lastval = val;
    //     // setBrightness(map(val, 0, 1023, 0, 255));
    //     Serial.println(map(val, 0, 1023, 0, 255));
    // }
    // delay(5);
}

// void setBrightness(int b) {
//     for (int p=0; p<NUMPIXELS; p++) {
//         pixels.setPixelColor(p, pixels.Color(b, b, b));
//     }
//     pixels.show();
// }


uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
