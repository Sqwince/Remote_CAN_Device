#ifndef SCROLLINGEFFECT_H
#define SCROLLINGEFFECT_H

#include <FastLED.h>

/// @file ScrollingEffect.h
/// Utility functions for color fill, palettes, blending, and more

class ScrollEffect {
public:
  /* ANIMATION PATTERNS */
  enum ScrollPattern {
    chase,          //LEDs chassing color1 & color2 -> 1|2|2, 2|1|2, 2|2|1
    invertedChase,  //LEDs chassing color1 & color2 -> 2|1|1, 1|2|1, 1|1|2
    chaseDimmed     //LEDs chassing color1 & color2 with dimming blur -> 2|1|1, 1|2|1, 1|1|2

  };


  /* CONSTRUCTOR */
  ScrollEffect(CRGB* leds,                   //pointer to LED strip
               bool rightToLeft,             //animation direction
               ScrollPattern pattern,        //selected animation for effect
               uint16_t startPos,            //Starting LED
               uint16_t ledCount,            //Number of LEDs to include in effect
               uint16_t ledSpacing,          //distance between lit LEDs (between 1 and ledCount)
               CRGB color1,                  //Color in CRGB format
               CRGB color2,                  //Color in CRGB format
               uint16_t scrollSpeed = 100);  //how fast to scroll the effect


  /** Destructor
    * @brief Destroys an RPMs effect object and frees up memory
     */
  ~ScrollEffect();  //Destructor

  /** 
    * @brief updates scrolling effect 
    * @param enabled turns effect on/off
    */
  void update(bool enabled);

  /** 
    * @brief updates scrolling effect 
    * @param delay delay in ms between scroll timer updates
    */
  void setScrollSpeed(uint16_t delay);



private:
  CRGB* _leds;             //pointer to LED strip
  bool _rightToLeft;       //animation direction
  ScrollPattern _pattern;  //selected animation for effect
  uint16_t _startPos;      //Starting LED
  uint16_t _ledCount;      //Number of LEDs to include in effect
  uint16_t _ledSpacing;    //distance between lit LEDs
  CRGB _color1;            ///Color in CRGB format
  CRGB _color2;            //Color in CRGB format
  uint16_t _scrollSpeed;   //how fast to scroll the effect
  //TODO: Define rate for scrolling


  //Animation State variables
  //bool _useDimming = true;            //fade last LED
  unsigned long _previousMillis = 0;  //scroll timer
  uint8_t _dimmingSteps = 2;          //Number of steps to use for dimming function
  uint16_t _currentScrollStep = 0;  // keeps track of where the scrolling led is
  uint16_t _totalSteps;
};

#endif  //END SCROLLINGEFFECT_H
