#include <Adafruit_NeoPixel.h>

const int LED_PIN = 6;
const int NUM_PIXELS = 12;
const int MIC_PIN = 0;
const int POT_PIN = 1;
const int DC_OFFSET = 330;
const int NOISE = 10;
const int NUM_SAMPLES = 100;
const int MAX_BRIGHTNESS = 255;
const int MIN_TOP = 70;

byte samplePos = 0;
int
    samples[NUM_SAMPLES],
    lvl = 10,
    minLvlAvg = 0,
    localPeak = 0,
    localPeakTick = 0,
    maxLvl = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

    for (int i = 0; i < NUM_SAMPLES; i++) {
        samples[i] = 0;
    }
    pixels.begin();

    Serial.begin(9600);
}

void loop() {
    int val, brightness;

    val = analogRead(MIC_PIN);
    val = abs(val - DC_OFFSET); // center on 0
    val = (val <= NOISE) ? 0 : (val - NOISE);
    lvl = ((lvl * 7) + val) >> 3; // dampen reading

    // brightness = MAX_BRIGHTNESS * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

    if (lvl > localPeak) {
        localPeak = ((localPeak * 3) + lvl) >> 2;
        localPeakTick = millis();
    } else if (millis() - localPeakTick > 5) {
        samples[samplePos] = localPeak;
        if (++samplePos > NUM_SAMPLES) {
            samplePos = 0;
        }
        localPeak = ((localPeak * 3) + lvl) >> 2;
        localPeakTick = millis();
    }

    int pot_val = analogRead(POT_PIN);
    if (abs(maxLvl - pot_val) > 10) {
        maxLvl = pot_val;
    }
    if (maxLvl < MIN_TOP) {
        maxLvl = MIN_TOP;
    }


    brightness = map(localPeak, 0, maxLvl, 0, 255);

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
    Serial.println(maxLvl);

    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, brightness, brightness, brightness);
    }
    pixels.show();


    // maxLvl = samples[0];
    // for (int i = 1; i < NUM_SAMPLES; i++) {
    //     if (samples[i] > maxLvl) {
    //         maxLvl = samples[i];
    //     }
    // }

    // if (lvl > maxVal || millis() - maxValTick > 2000) {
    //     maxVal = lvl;
    //     maxValTick = millis();
    // }
    // if (maxLvl < 10) {
    //     maxLvl = 10;
    // }
    // maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6;

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
}
