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
  rpmEffect.endPos = _ledCount;              //Number of LEDs to include in effect
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
void FastLED_Plus::updateRPMs_Effect(uint16_t currentRPM, s_RPMs rpms){


  
};








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
