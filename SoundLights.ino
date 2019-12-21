#include <Adafruit_NeoPixel.h>

const byte LED_PIN = 6; //digital 6
const byte NUM_PIXELS = 12;
const byte MIC_PIN = 0; //analog 0
const byte GAIN_POT_PIN = 1; //analog 1
const byte COLOR_POT_PIN = 2; //analog 2
const byte ON_SW_PIN = 3; //digital 3
const byte MODE_BTN_PIN = 12; //digital 12
const int DC_OFFSET = 380;
const int NOISE = 20;
const int MAX_BRIGHTNESS = 255;
const int MIN_TOP = 70;
const int COLOR_THRESHOLD = 200;

int
    lvl = 10,
    localPeak = 0,
    maxLvl = 30;
unsigned long localPeakTick = 0;

byte wheelPos = 0;
uint32_t color;

const byte NUM_MODES = 3;
const byte MODE_WHITE = 0;
const byte MODE_COLOR = 1;
const byte MODE_RAINBOW = 2;

byte mode = 2;

int rainbowPos = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pinMode(ON_SW_PIN, INPUT_PULLUP);
    pinMode(MODE_BTN_PIN, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(MODE_BTN_PIN), changeMode, FALLING);

    pixels.begin();

    // Serial.begin(9600);
}

void loop() {
    if (digitalRead(MODE_BTN_PIN) == LOW) {
        // debounce btn
        delay(100);
        if (digitalRead(MODE_BTN_PIN) == LOW) {
            mode = (mode + 1) % NUM_MODES;
        }
    }
    if (digitalRead(ON_SW_PIN) == LOW) {
        if (mode == MODE_RAINBOW) {

            for (int i = 0; i < NUM_PIXELS; i++) {
                pixels.setPixelColor(i, Wheel(((i * 256 / NUM_PIXELS) + rainbowPos) & 255, 255));
            }

            pixels.show();
            rainbowPos++;
            delay(10);

        } else {
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

            int potVal = analogRead(GAIN_POT_PIN);
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

            if (mode == MODE_WHITE) {
                color = pixels.Color(brightness, brightness, brightness);
            } else if (mode == 1) {
                int colorPotVal = analogRead(COLOR_POT_PIN);
                colorPotVal = map(colorPotVal, 0, 1023, 0, 255);
                if (abs(wheelPos = colorPotVal) > 20) {
                    wheelPos = colorPotVal;
                }
                color = Wheel(wheelPos, brightness);
            }

            for (int i = 0; i < NUM_PIXELS; i++) {
                pixels.setPixelColor(i, color);
            }
            pixels.show();

            // delay(1);
        }
    } else {
        for (int i = 0; i < NUM_PIXELS; i++) {
            pixels.setPixelColor(i, 0, 0, 0);
        }
        pixels.show();
        delay(1000);
    }
}

uint32_t Wheel(byte WheelPos, int brightness) {

    if(WheelPos < 85) {
        return pixels.Color((WheelPos * 3) * brightness/255, (255 - WheelPos * 3) * brightness/255, 0);
    } else if(WheelPos < 170) {
        WheelPos -= 85;
        return pixels.Color((255 - WheelPos * 3) * brightness/255, 0, (WheelPos * 3) * brightness/255);
    } else {
        WheelPos -= 170;
        return pixels.Color(0, (WheelPos * 3) * brightness/255, (255 - WheelPos * 3) * brightness/255);
    }
}
