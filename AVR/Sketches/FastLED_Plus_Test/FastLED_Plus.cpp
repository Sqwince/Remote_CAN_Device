#include "FastLED_Plus.h"

FastLED_Plus::FastLED_Plus() {
    _leds = leds;
    _numLeds = numLeds;
}


//Un-tested
void FastLED_Plus::breathe(CRGB* leds, uint16_t numLeds, uint8_t speed) {
    static uint8_t brightness = 0;
    static int8_t direction = 1;

    brightness += direction * speed;
    if (brightness == 0 || brightness == 255) {
        direction = -direction;
    }

    for (uint16_t i = 0; i < numLeds; i++) {
        leds[i].fadeToBlackBy(255 - brightness);
    }
    FastLED.show();
}

//Un-tested
void FastLED_Plus::chase(CRGB* leds, uint16_t numLeds, uint8_t color, uint8_t speed) {
    static uint16_t index = 0;
    index = (index + speed) % numLeds;

    fill_solid(leds, numLeds, CRGB::Black);
    leds[index] = CHSV(color, 255, 255);

    FastLED.show();
}
