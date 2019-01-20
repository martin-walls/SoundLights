#include <Adafruit_NeoPixel.h>

const int LED_PIN = 6;
const int NUM_PIXELS = 12;
const int MIC_PIN = 0;
const int POT_PIN = 1;
const int SWITCH_PIN = 11;
const int DC_OFFSET = 330;
const int NOISE = 10;
const int MAX_BRIGHTNESS = 255;
const int MIN_TOP = 70;
const int COLOR_THRESHOLD = 200;

int
    lvl = 10,
    localPeak = 0,
    localPeakTick = 0,
    maxLvl = 30;

// byte wheelPos = 0;

// bool colorChanged = false;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    pixels.begin();

    // Serial.begin(9600);
}

void loop() {
    if (digitalRead(SWITCH_PIN) == LOW) {
        int val, brightness;

        val = analogRead(MIC_PIN);
        val = abs(val - DC_OFFSET); // center on 0
        val = (val <= NOISE) ? 0 : (val - NOISE);
        lvl = ((lvl * 7) + val) >> 3; // dampen reading

        // brightness = MAX_BRIGHTNESS * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

        // store local peak value (minimum every 5ms)
        if (lvl > localPeak || millis() - localPeakTick > 5) {
            localPeak = ((localPeak * 3) + lvl) >> 2;
            localPeakTick = millis();
        }

        int potVal = analogRead(POT_PIN);
        if (abs(maxLvl - potVal) > 10) {
            maxLvl = potVal;
        }
        if (maxLvl < MIN_TOP) {
            maxLvl = MIN_TOP;
        }

        brightness = map(localPeak, 0, maxLvl, 0, 255);

        // Serial.println(lvl);
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

        // if (brightness == 0 && !colorChanged) {
        //     wheelPos = (wheelPos + 30) % 255;
        //     colorChanged = true;
        // } else if (brightness > 0 && colorChanged) {
        //     colorChanged = false;
        // }


        for (int i = 0; i < NUM_PIXELS; i++) {
            pixels.setPixelColor(i, brightness, brightness, brightness);
        }
        pixels.show();

        // delay(1);
    } else {
        for (int i = 0; i < NUM_PIXELS; i++) {
            pixels.setPixelColor(i, 0, 0, 0);
        }
        pixels.show();
    }
}

// uint32_t Wheel(byte WheelPos, int brightness) {
//
//     if(WheelPos < 85) {
//         return pixels.Color((WheelPos * 3) * brightness/255, (255 - WheelPos * 3) * brightness/255, 0);
//     } else if(WheelPos < 170) {
//         WheelPos -= 85;
//         return pixels.Color((255 - WheelPos * 3) * brightness/255, 0, (WheelPos * 3) * brightness/255);
//     } else {
//         WheelPos -= 170;
//         return pixels.Color(0, (WheelPos * 3) * brightness/255, (255 - WheelPos * 3) * brightness/255);
//     }
// }
