#ifndef BLINKEFFECT_H
#define BLINKEFFECT_H

#include <FastLED.h>

class BlinkEffect {
private:
  CRGB* _leds;           //pointer to LED strip
  int _pattern;          //animation pattern
  uint16_t _startPos;    //Starting LED
  uint16_t _ledCount;    //Number of LEDs to include in effect
  CRGB _color1;          //Color in CRGB format
  CRGB _color2;          //Color in CRGB format
  CRGB _color3;          //Color in CRGB format
  uint16_t _blinkDelay;  //blink delay for redline blink animation

  //State Variables
  unsigned long _lastBlinkTime = 0;  // Last recorded blink time
  bool _blinkState = false;          // State of blinking
  bool _isActive = false;           //effect enabled by SimHub





public:
  /* CONSTRUCTOR - TWO ALTERNATING PATTERNS */
  BlinkEffect(CRGB* leds,            //pointer to LED strip
              uint16_t pat,               //animation pattern
              uint16_t startPos,     //Starting LED (BASE 0)
              uint16_t ledCount,     //Number of LEDs to include in effect
              CRGB color1,           //Color in CRGB format
              CRGB color2,           //Color in CRGB format
              CRGB color3 = CRGB::Black,           //Color in CRGB format
              uint16_t blinkDelay = 100);  //blink delay for redline blink animation (Default = 100ms)

  /** Destructor */
  ~BlinkEffect();

  /**
    * @brief Animate LED effect. Enabled by input state
    * @param enabled enable effect to be drawn
  */
  void update(bool enabled);

  /*#####################################################*/
  /*####              HELPER FUNCTIONS               ####*/
  /*#####################################################*/
  void setBlinkDelay(uint16_t delay); 

  uint16_t getBlinkDelay();

};



#endif  //END BLINKEFFECTS_H
