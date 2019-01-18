#include <Adafruit_NeoPixel.h>

const int LED_PIN = 6;
const int NUM_PIXELS = 12;
const int MIC_PIN = 0;
const int DC_OFFSET = 330;
const int NOISE = 10;
const int SAMPLES = 5;
const int MAX_BRIGHTNESS = 255;

byte volCount = 0;
int
    vol[SAMPLES],
    lvl = 10,
    minLvlAvg = 0,
    maxLvlAvg = 0,
    localPeak = 0,
    localPeakTick = 0,
    maxVal = 0,
    maxValTick = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

    for (int i = 0; i < SAMPLES; i++) {
        vol[i] = 0;
    }
    pixels.begin();

    Serial.begin(9600);
}

void loop() {
    uint8_t i;
    uint16_t minLvl, maxLvl;
    int val, brightness;

    val = analogRead(MIC_PIN);
    val = abs(val - DC_OFFSET); // center on 0
    val = (val <= NOISE) ? 0 : (val - NOISE);
    lvl = ((lvl * 7) + val) >> 3; // dampen reading

    // brightness = MAX_BRIGHTNESS * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

    if (lvl > localPeak || millis() - localPeakTick > 5) {
        localPeak = ((localPeak * 3) + lvl) >> 2;
        localPeakTick = millis();
    }


    brightness = map(localPeak, 0, 100, 0, 255);

    Serial.println(localPeak);
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


    if (val > maxVal || millis() - maxValTick > 1000) {
        maxVal = val;
        maxValTick = millis();
    }
    if (maxVal < 10) {
        maxVal = 10;
    }

    // add small delay to help smooth output and make it less jittery
    // delay(5);

    // vol[volCount] = val; // save sample for dynamic leveling
    // if (++volCount >= SAMPLES) {
    //     volCount = 0;
    // }
    //
    // // maxLvl = vol[0];
    // // for (int i = 0; i < SAMPLES; i++) {
    // //     if (vol[i] > maxLvl) {
    // //         maxLvl = vol[i];
    // //     }
    // // }
    //
    //
    // // get volume range of previous frames
    // minLvl = maxLvl = vol[0];
    // for (i = 1; i < SAMPLES; i++) {
    //     if (vol[i] < minLvl) {
    //         minLvl = vol[i];
    //     } else if (vol[i] > maxLvl) {
    //         maxLvl = vol[i];
    //     }
    // }
    //
    // if ((maxLvl - minLvl) < MAX_BRIGHTNESS) {
    //     maxLvl = minLvl + MAX_BRIGHTNESS;
    // }
    //
    // minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6;
    // maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6;
}
