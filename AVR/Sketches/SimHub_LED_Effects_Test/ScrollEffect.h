/*
#ifndef SCROLLINGEFFECT_H
#define SCROLLINGEFFECT_H

#include <FastLED.h>

/// @file ScrollingEffect.h
/// Utility functions for color fill, palettes, blending, and more

class ScrollingEffect {
private:
  CRGB* _leds;        //pointer to LED strip
  bool _rightToLeft;  //animation direction
  //TODO: Define rate for scrolling
  uint16_t _scrollSpeed;  //how fast to scroll the effect
  uint16_t _startPos;     //Starting LED
  uint16_t _ledCount;     //Number of LEDs to include in effect
  CRGB _scrollColor;      //Scrolling Color in CRGB format
  CRGB _bgColor;          //background color in CRGB format

  
  bool _useDimming = true;    //fade last LED
  uint8_t _dimmingSteps = 8;  //Number of steps to use for dimming function

  //Redline Blink State Variables
  unsigned long _lastBlinkTime = 0;  // Last recorded update time
  uint16_t _currentScrollStep = 0;   // keeps track of where the scrolling led is

public:
  /* CONSTRUCTOR 
  ScrollingEffect(CRGB* leds,            //pointer to LED strip
                  bool rightToLeft,      //animation direction
                  uint16_t scrollSpeed,  //how fast to scroll the effect
                  uint16_t startPos,     //Starting LED
                  uint16_t ledCount,     //Number of LEDs to include in effect
                  CRGB scrollColor,      //Starting Color in CRGB format
                  CRGB bgColor);         //Ending color in CRGB format


  /** Destructor
    * @brief Destroys an RPMs effect object and frees up memory
    
  ~ScrollingEffect();  //Destructor

  /** Destructor
    * @brief updates scrolling effect 
    
  void update();


#endif  //END SCROLLINGEFFECT_H
*/