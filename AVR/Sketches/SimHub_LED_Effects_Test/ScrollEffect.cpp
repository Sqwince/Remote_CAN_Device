#include "ScrollEffect.h"

/* CONSTRUCTOR */
ScrollEffect::ScrollEffect(
  CRGB* leds,             //pointer to LED strip
  bool rightToLeft,       //animation direction
  ScrollPattern pattern,  //selected animation for effect
  uint16_t startPos,      //Starting LED
  uint16_t ledCount,      //Number of LEDs to include in effect
  uint16_t ledSpacing,    //distance between lit LEDs
  CRGB color1,            //Color in CRGB format
  CRGB color2,            //Color in CRGB format
  uint16_t scrollSpeed)   //how fast to scroll the effect
  :                       //Ending color in CRGB format
    _leds(leds), _rightToLeft(rightToLeft), _pattern(pattern),
    _startPos(startPos), _ledCount(ledCount), _ledSpacing(ledSpacing),
    _color1(color1), _color2(color2), _scrollSpeed(scrollSpeed) {

  //uint16_t _totalSteps = ledCount * _dimmingSteps;
}

//destructor
ScrollEffect::~ScrollEffect() {}


//Scrolling Effect
//TODO: idea maybe want to have the scrolling rate as an input argument,
//might be useful to show various effects tied to game data
void ScrollEffect::update(bool enabled) {

  /*Timer*/
  unsigned long currentMillis = millis();

  /*timer*/
  if (currentMillis - _previousMillis >= _scrollSpeed) {
    _previousMillis = currentMillis;  //update tick


    /*##############################################*/
    /*####              ANIMATIONS              ####*/
    /*##############################################*/
    switch (_pattern) {

        /*##########################################################*/
        // single LEDs CHASING with set spacing  >>2|1|2|2|2|1|2>> */
        /*##########################################################*/
      case chase:
        if (enabled) {
          //.oO|Oo.//
          //total steps = number of fadesteps + fullyOn + fadestep number
          for (uint16_t i = _startPos; i < (_startPos + _ledCount); i++) {

            //Draw direction
            uint16_t index = i;                                                           //Draw left to right (Default)
            if (_rightToLeft) { index = _startPos + ((_startPos + _ledCount - 1) - i); }  //Draw Right to left

            //update LED strip
            _leds[index] = ((i + _currentScrollStep) % _ledSpacing == 0) ? _color1 : _color2;
          }
          _currentScrollStep++;  //Next LED step
          break;
        }  //break included in enabled loop to fall to default when !enabled


        /*##########################################################*/
        // single LEDs CHASING with set spacing  >>2|1|2|2|2|1|2>> */
        /*##########################################################*/
      case chaseDimmed:
        if (enabled) {

          fill_gradient_RGB(_leds, _startPos, _color1, (_startPos + _ledCount), _color2);

        };  //Next LED step
        break;
    }  //break included in enabled loop to fall to default when !enabled

        /*##########################################################*/
        // single LEDs CHASING with set spacing  >>2|1|2|2|2|1|2>> */
        /*##########################################################*/
      case chaseDimmed:
        if (enabled) {

          fill_gradient_RGB(_leds, _startPos, _color1, (_startPos + _ledCount), _color2);

        };  //Next LED step
        break;
    }  //break included in enabled loop to fall to default when !enabled


      /*###############################################################################*/
      /*          DEFAULT STATE - ALL SET TO COLOR3                                    */
      /*###############################################################################*/
    default:
      //Restore LEDs to default state
      for (uint16_t i = _startPos; i < (_startPos + _ledCount); i++) {
        _leds[i] = _color2;  //restore to default
      }
      break;
  }
}
}


/*
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
