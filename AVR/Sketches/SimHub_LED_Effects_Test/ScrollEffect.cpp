/*
#include "ScrollEffect.h"

//Constructor
ScrollingEffect::ScrollingEffect(
  CRGB* leds,            //pointer to LED strip
  bool rightToLeft,      //animation direction
  uint16_t scrollSpeed,  //how fast to scroll the effect
  uint16_t startPos,     //Starting LED
  uint16_t ledCount,     //Number of LEDs to include in effect
  CRGB scrollColor,      //Starting Color in CRGB format
  CRGB bgColor)
  :  //Ending color in CRGB format
    _leds(leds), _rightToLeft(rightToLeft), _scrollSpeed(scrollSpeed),
    _startPos(startPos), _ledCount(ledCount), _scrollColor(scrollColor), _bgColor(bgColor) {}

//destructor
ScrollingEffect::~ScrollingEffect() {}


//Scrolling Effect
//TODO: idea maybe want to have the scrolling rate as an input argument,
//might be useful to show various effects tied to game data
ScrollingEffect::update() {


  //currentRPM input value to LED indexes
  //uint16_t ledPosition = map(_currentScrollstep, _minRPM, _maxRPM, _startPos, _ledCount * _dimmingSteps);


  //.oO|Oo.//
  //total steps = number of fadesteps + fullyOn + fadestep number
  int totalSteps = ((2 * _dimmingSteps) + 1);
  _currentScrollStep = (_currentScrollingStep + 1) % totalSteps;  //0 to totalSteps restart back to 0
  int fullyOnIndex = ledPosition / _dimmingSteps;                 // Whole LED index
  int fadeStep = ledPosition % _dimmingSteps;                     // 0-8 step for fading

  //Fill the led array with calculated colors
  for (int i = _startPos; i < _ledCount; i++) {

    //TODO: replace with helper function from RPMEffects
    int index = i;                                         //Draw left to right (Default)
    if (!_rightToLeft) { index = ((_ledCount - 1) - i); }  //Draw Right to left

    //LEFT OFF HERE

    //Fill entire array with calculated colors
    _leds[index] = blend(_startColor, CRGB::Black, (i * 255) / (_ledCount - 1));
  }


  // Adjust brightness
  for (int i = _startPos; i < _ledCount; i++) {
    if (i < fullyOnIndex) {
      _leds[getRTLIndex(i)].maximizeBrightness();  // Fully on
    } else if (i == fullyOnIndex) {
      uint8_t brightness = map(fadeStep, 0, _dimmingSteps - 1, 0, 255);  // Start dim, increase to full brightness
      _leds[getRTLIndex(i)].nscale8(brightness);
    } else {
      _leds[getRTLIndex(i)].nscale8(0);  // Off
    }
  }
}
else {
  //Draw Redline Blink Animation
  //if (rpmPercentage >= maxRPM && millis() - lastBlinkTime >= redlineBlinkMs) {
  unsigned long currentBlinkMillis = millis();

  if (currentBlinkMillis - _lastBlinkTime >= _blinkDelay) {
    _blinkState = !_blinkState;
    _lastBlinkTime = currentBlinkMillis;

    for (int i = _startPos; i < _ledCount; i++) {
      _leds[getRTLIndex(i)] = _blinkState ? _redlineColor1 : _redlineColor2;  //alternate between RLcolor 1 & 2
    }


    // //split mirror hack
    // for (int i = 0; i < half_LED_Num + 1; i++) {
    //   _leds[(_ledCount - 1) - i] = _leds[i];
    // }

    FastLED.show();
  }
}
}
}*/