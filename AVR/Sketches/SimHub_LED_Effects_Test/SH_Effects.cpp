#include "SH_Effects.h"

//Constructor
SH_Effects::SH_Effects() {
  //_leds = leds;
  // _numLeds = numLeds;
}

//returns RPMGauge struct which holds settings for the RPMGauge effect
s_RPMs SH_Effects::createRPMsEffect(
  CRGB* _leds,             //pointer to LED strip
  uint16_t _startPos,      //Starting LED
  uint16_t _ledCount,      //Number of LEDs to include in effect
  CRGB _startColor,        //Starting Color in CRGB format
  CRGB _endColor,          //Ending color in CRGB format
  uint16_t _minRPM,        //minimum RPM value (Default: 0%)
  uint16_t _maxRPM,        //maximum RPM value (Default: 100%)
  bool _useDimming,        //fade last LED
  bool _rightToLeft,       //animation direction
  CRGB _redlineColor1,     //Color 1 of RPM Redline animation blink
  CRGB _redlineColor2,     //Color 2 of PRM redline animation blink
  uint16_t _blinkDelay) {  //blink delay for redline blink animation

  s_RPMs rpmEffect;  //new instance RPMGauge to return

  rpmEffect.leds = _leds;                    //pointer to LED strip
  rpmEffect.startPos = _startPos;            //Starting LED
  rpmEffect.ledCount = _ledCount;            //Number of LEDs to include in effect
  rpmEffect.startColor = _startColor;        //Starting Color in CRGB format
  rpmEffect.endColor = _endColor;            //Ending color in CRGB format
  rpmEffect.minRPM = _minRPM;                //minimum RPM value (Default: 0%)
  rpmEffect.maxRPM = _maxRPM;                //maximum RPM value (Default: 100%)
  rpmEffect.useDimming = _useDimming;        //fade last LED
  rpmEffect.rightToLeft = _rightToLeft;      //animation direction
  rpmEffect.redlineColor1 = _redlineColor1;  //Color 1 of RPM Redline animation blink
  rpmEffect.redlineColor2 = _redlineColor2;  //Color 2 of PRM redline animation blink
  rpmEffect.blinkDelay = _blinkDelay;        //blink delay for redline blink animation

  return rpmEffect;
}

/*draw RPM effect on LED Strip that resembles SimHub RPMs (0% to 100%) Effect*/
void FastLED_Plus::updateRPMs_Effect(uint16_t currentRPM, s_RPMs rpms) {

  //int ledPosition = map(potValue, 0, 1023, 0, half_LED_Num * DIMMING_STEPS);  // 4 steps per LED

  uint16_t ledPosition = map(currentRPM, rpmEffect.minRPM, rpmEffect.maxRPM, rpmEffect.ledCount * _dimmingSteps);
  int fullyOnIndex = ledPosition / _dimmingSteps;  // Whole LED index
  int fadeStep = ledPosition % _dimmingSteps;      // 0-3 step for fading

  // Define the color gradient from Blue to Red
  for (int i = 0; i < half_LED_Num; i++) {
    leds[i] = blend(CRGB::Green, CRGB::Red, (i * 255) / (half_LED_Num - 1));
  }

  // Adjust brightness
  for (int i = 0; i < half_LED_Num; i++) {
    if (i < fullyOnIndex) {
      leds[i].maximizeBrightness();  // Fully on
    } else if (i == fullyOnIndex) {
      uint8_t brightness = map(fadeStep, 0, DIMMING_STEPS - 1, 0, 255);  // Start dim, increase brightness
      leds[i].nscale8(brightness);
    } else {
      leds[i].nscale8(0);  // Off
    }
  }

  for (int i = 0; i < half_LED_Num + 1; i++) {
    leds[(NUM_LEDS - 1) - i] = leds[i];
  }
  FastLED.show();
}
}
}
;
