#include "RPMsEffect.h"


//Constructor
RPMsEffect::RPMsEffect(
  CRGB* leds,          //pointer to LED strip
  bool rightToLeft,    //animation direction
  uint16_t startPos,   //Starting LED
  uint16_t ledCount,   //Number of LEDs to include in effect
  CRGB startColor,     //Starting Color in CRGB format
  CRGB endColor,       //Ending color in CRGB format
  uint16_t minRPM,     //minimum RPM value (Default: 0%)
  uint16_t maxRPM,     //maximum RPM value (Default: 100%)
  bool redlineBlink,   //blink Color 1 & 2 when RPM = 100%
  CRGB redlineColor1,  //Color 1 of RPM Redline animation blink
  CRGB redlineColor2)  //Color 2 of PRM redline animation blink
  : _leds(leds), _rightToLeft(rightToLeft), _startPos(startPos), _ledCount(ledCount),
    _startColor(startColor), _endColor(endColor), _minRPM(minRPM), _maxRPM(maxRPM),
    _redlineBlink(redlineBlink), _redlineColor1(redlineColor1), _redlineColor2(redlineColor2) {}


//Destructor
RPMsEffect::~RPMsEffect() {}


/*draw RPM effect on LED Strip that resembles SimHub RPMs (0% to 100%) Effect*/
void RPMsEffect::update(uint16_t currentRPM) {

  int half_LED_Num = _ledCount / 2;

  //currentRPM input value to LED indexes
  uint16_t ledPosition = map(currentRPM, _minRPM, _maxRPM, 0, _ledCount * _dimmingSteps);
  int fullyOnIndex = ledPosition / _dimmingSteps;  // Whole LED index
  int fadeStep = ledPosition % _dimmingSteps;      // 0-3 step for fading

  // Define the color gradient from StartColor to EndColor
  for (int i = 0; i < half_LED_Num; i++) {
    _leds[i] = blend(_startColor, _endColor, (i * 255) / (half_LED_Num - 1));
  }

  // Adjust brightness
  for (int i = 0; i < half_LED_Num; i++) {
    if (i < fullyOnIndex) {
      _leds[i].maximizeBrightness();  // Fully on
    } else if (i == fullyOnIndex) {
      uint8_t brightness = map(fadeStep, 0, _dimmingSteps - 1, 0, 255);  // Start dim, increase to full brightness
      _leds[i].nscale8(brightness);
    } else {
      _leds[i].nscale8(0);  // Off
    }
  }

  //split mirror hack
  for (int i = 0; i < half_LED_Num + 1; i++) {
    _leds[(_ledCount - 1) - i] = _leds[i];
  }

  FastLED.show();
}