#include "BlinkEffect.h"

/* CONSTRUCTOR - TWO ALTERNATING PATTERNS */
BlinkEffect::BlinkEffect(
  CRGB* leds,            //pointer to LED strip
  blinkPattern pattern,  //animation pattern
  uint16_t startPos,     //Starting LED (BASE 0)
  uint16_t ledCount,     //Number of LEDs to include in effect
  CRGB color1,           //Color in CRGB format
  CRGB color2,           //Color in CRGB format
  CRGB color3,           //Color in CRGB format
  uint16_t blinkDelay)   //blink delay for redline blink animation (Default = 100ms)
  : _leds(leds), _pattern(pattern), _startPos(startPos), _ledCount(ledCount),
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


    /*##############################################*/
    /*####              ANIMATIONS              ####*/
    /*##############################################*/
    switch (_pattern) {


      /*###############################################################################*/
      //Alternating flashing all LEDs between color1 & color2 -> 1|1|1, 2|2|2, 1|1|1 */
      /*###############################################################################*/
      case flashing:
        if (enabled) {
          //Calculate LED colors for effect
          for (uint16_t i = _startPos; i < (_startPos + _ledCount); i++) {
            _leds[i] = _blinkState ? _color1 : _color2;  //alternate between color 1 & 2
          }
          break;
        }  //break included in enabled loop to fall to default when !enabled


      /*###############################################################################*/
      //Even/Odd flip flop animation between color1 & color2  -> 1|2|1, 2|1|2, 1|2|1 */
      /*###############################################################################*/
      case alternateTwo:
        if (enabled) {
          //Calculate LED colors for effect
          for (uint16_t i = _startPos; i < (_startPos + _ledCount); i++) {
            if (_blinkState) {
              _leds[i] = (i % 2 == 0) ? _color1 : _color2;  // Even index → color1, Odd index → color2
            } else {
              _leds[i] = (i % 2 == 0) ? _color2 : _color1;  // Even index → color1, Odd index → color2
            }
          }
          break;
        }  //break included in enabled loop to fall to default when !enabled


        /*###############################################################################*/
        //Even/Odd flip-flop animation between 3 colors         -> 1|3|1, 3|2|3, 1|3|1 */
        /*###############################################################################*/
      case alternateThree:
        if (enabled) {

          //Calculate LED colors for effect
          for (uint16_t i = _startPos; i < (_startPos + _ledCount); i++) {
            if (_blinkState) {
              _leds[i] = (i % 2 == 0) ? _color3 : _color1;        // BlinkState1: Even index → color3, Odd index → color1
            } else {
              _leds[i] = (i % 2 == 0) ? _color2 : _color3;        // BlinkState2: Even index → color2, Odd index → color3
            }
          }

          break;
        }  //break included in enabled loop to fall to default when !enabled

        /*###############################################################################*/
        /*          DEFAULT STATE - ALL SET TO COLOR3                                    */
        /*###############################################################################*/
      default:
        //Restore LEDs to default state
        for (uint16_t i = _startPos; i < (_startPos + _ledCount); i++) {
          _leds[i] = _color3;  //restore to default
        }
        break;
    }
  }
}


/*#####################################################*/
/*####              HELPER FUNCTIONS               ####*/
/*#####################################################*/
void BlinkEffect::setBlinkDelay(uint16_t delay) {
  _blinkDelay = delay;
}

uint16_t BlinkEffect::getBlinkDelay() {
  return _blinkDelay;
}
