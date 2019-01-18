#include <Adafruit_NeoPixel.h>

const int LED_PIN = 6;
const int NUM_PIXELS = 12;
const int MIC_PIN = 0;
const int POT_PIN = 1;
const int DC_OFFSET = 330;
const int NOISE = 10;
const int MAX_BRIGHTNESS = 255;
const int MIN_TOP = 70;

int
    lvl = 10,
    localPeak = 0,
    localPeakTick = 0,
    maxLvl = 30;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pixels.begin();

    Serial.begin(9600);
}

void loop() {
    int val, brightness;

    val = analogRead(MIC_PIN);
    Serial.println(val);
    val = abs(val - DC_OFFSET); // center on 0
    val = (val <= NOISE) ? 0 : (val - NOISE);
    lvl = ((lvl * 7) + val) >> 3; // dampen reading

    // brightness = MAX_BRIGHTNESS * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

    // store local peak value (minimum every 5ms)
    if (lvl > localPeak || millis() - localPeakTick > 5) {
        localPeak = ((localPeak * 3) + lvl) >> 2;
        localPeakTick = millis();
    }

    // int pot_val = analogRead(POT_PIN);
    // if (abs(maxLvl - pot_val) > 10) {
    //     maxLvl = pot_val;
    // }
    // if (maxLvl < MIN_TOP) {
    //     maxLvl = MIN_TOP;
    // }

    brightness = map(localPeak, 0, 70, 0, 255);

    // brightness = val;
    // if (brightness > 255) {
    //     brightness = 255;
    // }

    // clipping
    if (brightness < 0L) {
        brightness = 0;
    } else if (brightness > MAX_BRIGHTNESS) {
        brightness = MAX_BRIGHTNESS;
    }


    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, brightness, brightness, brightness);
    }
    pixels.show();

    delay(1);
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
