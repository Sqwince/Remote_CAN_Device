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
    _redlineBlink(redlineBlink), _redlineColor1(redlineColor1), _redlineColor2(redlineColor2),
    redlineBlink(leds, BlinkEffect::flashing, startPos, ledCount, redlineColor1, redlineColor2) {}


//Destructor
RPMsEffect::~RPMsEffect() {}


/*draw RPM effect on LED Strip that resembles SimHub RPMs (0% to 100%) Effect*/
void RPMsEffect::update(uint16_t currentRPM) {

  //currentRPM input value to LED indexes
  uint16_t ledPosition = map(currentRPM, _minRPM, _maxRPM, 0, _ledCount * _dimmingSteps);
  int fullyOnIndex = _startPos + (ledPosition / _dimmingSteps);  // Whole LED index
  int fadeStep = ledPosition % _dimmingSteps;                    // 0-3 step for fading

  bool isAtRedline = (currentRPM >= _maxRPM) ? true : false;

  if (!isAtRedline) {

    // Define the color gradiant from StartColor to EndColor for the full LED strip
    for (int i = _startPos; i < (_startPos + _ledCount); i++) {
      //Fill entire array with calculated colors
      ///// @param leds a pointer to the LED array to fill
      /// @param startpos the starting position in the array
      /// @param startcolor the starting color for the gradient
      /// @param endpos the ending position in the array
      /// @param endcolor the end color for the gradient
      if (_rightToLeft) {
        fill_gradient_RGB(_leds, (_startPos + _ledCount - 1), _startColor, _startPos, _endColor);
      } else {
        fill_gradient_RGB(_leds, _startPos, _startColor, (_startPos + _ledCount - 1), _endColor);
        //_leds[getRTLIndex(i)] = blend(_startColor, _endColor, (i * 255) / (_ledCount - 1));
      }
    }


    // Adjust brightness
    for (int i = _startPos; i < (_startPos + _ledCount); i++) {
      if (i < fullyOnIndex) {
        _leds[getRTLIndex(i)].maximizeBrightness();  // Fully on
      } else if (i == fullyOnIndex) {
        uint8_t brightness = map(fadeStep, 0, _dimmingSteps - 1, 0, 255);  // Start dim, increase to full brightness
        _leds[getRTLIndex(i)].nscale8(brightness);
      } else {
        _leds[getRTLIndex(i)].nscale8(0);  // Off
      }
    }
  } else {
    //Draw Redline Blink Animation
    redlineBlink.update(isAtRedline);
  }
}


/*#####################################################*/
/*####              HELPER FUNCTIONS               ####*/
/*#####################################################*/
/**
    * @brief returns the LED array index based on draw direction
    * @param index of element in LED array
  */
int RPMsEffect::getRTLIndex(int i) {
  int index = i;                                                                //Draw left to right (Default)
  if (_rightToLeft) { index = _startPos + ((_startPos + _ledCount - 1) - i); }  //Draw Right to left
  return index;
}

void RPMsEffect::setBlinkDelay(uint16_t delay) {
  redlineBlink.setBlinkDelay(delay);
}

uint16_t RPMsEffect::getBlinkDelay() {
  return _blinkDelay;
}
