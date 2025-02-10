#include "BlinkEffect.h"


/* CONSTRUCTOR - TWO ALTERNATING PATTERNS */
BlinkEffect::BlinkEffect(
  CRGB* leds,           //pointer to LED strip
  uint16_t pat,         //animation pattern
  uint16_t startPos,    //Starting LED (BASE 0)
  uint16_t ledCount,    //Number of LEDs to include in effect
  CRGB color1,          //Color in CRGB format
  CRGB color2,          //Color in CRGB format
  CRGB color3,          //Color in CRGB format
  uint16_t blinkDelay)  //blink delay for redline blink animation (Default = 100ms)
  : _leds(leds), _pattern(pat), _startPos(startPos), _ledCount(ledCount),
    _color1(color1), _color2(color2), _color3(color3), _blinkDelay(blinkDelay) {}

//Destructor
BlinkEffect::~BlinkEffect() {}


//animates LEDs flasshing effect
void BlinkEffect::update(bool enabled) {
  unsigned long currentBlinkMillis = millis();
  /*Timer*/
  if (currentBlinkMillis - _lastBlinkTime >= _blinkDelay) {
    _blinkState = !_blinkState;
    _lastBlinkTime = currentBlinkMillis;

    FastLED.clear();

    if (enabled) {
      // Define the color gradiant from StartColor to EndColor for the full LED strip
      for (uint16_t i = _startPos; i < (_startPos + _ledCount); i++) {




        for (uint16_t i = _startPos; i < (_startPos + _ledCount); i++) {
          _leds[i] = _blinkState ? _color1 : _color2;  //alternate between color 1 & 2
        }
      }
    }
  }
}
