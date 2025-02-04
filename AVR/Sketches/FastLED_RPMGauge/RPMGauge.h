#ifndef RPMGAUGE_H
#define RPMGAUGE_H

#include <FastLED.h>

class RPMGauge {
public:
    RPMGauge(int startPos, int ledCount, CRGB color, CRGB startColor, CRGB endColor,
             int minRPM, int maxRPM, int redlineBlinkMs, bool useDimming, bool rightToLeft)
        : startPos(startPos), ledCount(ledCount), color(color), startColor(startColor), endColor(endColor),
          minRPM(minRPM), maxRPM(maxRPM), redlineBlinkMs(redlineBlinkMs), useDimming(useDimming), rightToLeft(rightToLeft),
          lastBlinkTime(0), blinkState(false) {
        leds = new CRGB[ledCount];
        FastLED.addLeds<WS2812B, 6, GRB>(leds, ledCount); // Assume data pin 6
    }

    ~RPMGauge() {
        delete[] leds;
    }

    void update(int rpmPercentage) {
        int ledPosition = map(rpmPercentage, minRPM, maxRPM, 0, ledCount - 1);
        int fullyOnIndex = ledPosition;
        int fadeStep = ledPosition % 8;
        
        for (int i = 0; i < ledCount; i++) {
            if (i < fullyOnIndex) {
                leds[i] = blend(startColor, endColor, (i * 255) / (ledCount - 1));
            } else if (i == fullyOnIndex) {
                uint8_t brightness = map(fadeStep, 0, 7, 0, 255);
                leds[i] = blend(startColor, endColor, (i * 255) / (ledCount - 1));
                leds[i].nscale8(brightness);
            } else {
                leds[i] = CRGB::Black;
            }
        }

        if (rpmPercentage >= 100 && millis() - lastBlinkTime >= redlineBlinkMs) {
            blinkState = !blinkState;
            lastBlinkTime = millis();
            for (int i = 0; i < ledCount; i++) {
                leds[i] = blinkState ? CRGB::Red : CRGB::Black;
            }
        }
    }

    void show() {
        FastLED.show();
    }

private:
    int startPos;
    int ledCount;
    CRGB color;
    CRGB startColor;
    CRGB endColor;
    int minRPM;
    int maxRPM;
    int redlineBlinkMs;
    bool useDimming;
    bool rightToLeft;
    unsigned long lastBlinkTime;
    bool blinkState;
    CRGB* leds;
};

#endif