#ifndef RPMSEFFECT_H
#define RPMSEFFECT_H

/// @file RPMsEffect.h
/// Utility functions for color fill, palettes, blending, and more

#include <FastLED.h>
#include "BlinkEffect.h"

class RPMsEffect {
private:
  CRGB* _leds;               //pointer to LED strip
  bool _rightToLeft;         //animation direction
  uint16_t _startPos;        //Starting LED
  uint16_t _ledCount;        //Number of LEDs to include in effect
  CRGB _startColor;          //Starting Color in CRGB format
  CRGB _endColor;            //Ending color in CRGB format
  uint16_t _minRPM;          //minimum RPM value (Default: 0%)
  uint16_t _maxRPM;          //maximum RPM value (Default: 100%)
  bool _redlineBlink;        //blink Color 1 & 2 when RPM = 100%
  CRGB _redlineColor1;       //Color 1 of RPM Redline animation blink
  CRGB _redlineColor2;       //Color 2 of PRM redline animation blink
  BlinkEffect redlineBlink;  //blinkeffect object

  //default effect (can be updated with functions later)
  uint16_t _blinkDelay = 100;  //blink delay for redline blink animation
  bool _useDimming = true;     //fade last LED
  uint8_t _dimmingSteps = 8;   //Number of steps to use for dimming function

  //Redline Blink State Variables
  unsigned long _lastBlinkTime = 0;  // Last recorded blink time
  bool _blinkState = false;          // State of blinking for redline alert

public:

  /* CONSTRUCTOR */
  RPMsEffect(CRGB* leds,           //pointer to LED strip
             bool rightToLeft,     //animation direction
             uint16_t startPos,    //Starting LED
             uint16_t ledCount,    //Number of LEDs to include in effect
             CRGB startColor,      //Starting Color in CRGB format
             CRGB endColor,        //Ending color in CRGB format
             uint16_t minRPM,      //minimum RPM value (Default: 0%)
             uint16_t maxRPM,      //maximum RPM value (Default: 100%)
             bool redlineBlink,    //blink Color 1 & 2 when RPM = 100%
             CRGB redlineColor1,   //Color 1 of RPM Redline animation blink
             CRGB redlineColor2);  //Color 2 of PRM redline animation blink


  /** Destructor
    * @brief Destroys an RPMs effect object and frees up memory
    */
  ~RPMsEffect();  //Destructor


  /**
    * @brief Calculates the LED colors based on currentRPMs value
    * @param currentRPM RPMs value (0-100%) for the effect
  */
  void update(uint16_t currentRPM);

  //TODO: move to separate helper .h/.cpp
  /*#####################################################*/
  /*####              HELPER FUNCTIONS               ####*/
  /*#####################################################*/

  /**
    * @brief Sets the LED blink rate
    * @param delay delay in ms
  */
  void setBlinkDelay(uint16_t delay);


  /**
    * @brief returns uint16_t of current blink delay in ms
  */
  uint16_t getBlinkDelay();


  /**
    * @brief returns the LED array index based on draw direction
    * @param index of element in LED array
  */
  int getRTLIndex(int index);
};

#endif  //SH_EFFECTS_H






// // FROM FASTLEDS LIBRARY
// // Uncomment/edit one of the following lines for your leds arrangement.
// // ## Clockless types ##
// // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
// // FastLED.addLeds<SM16703, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<TM1812, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<UCS1904, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<UCS2903, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
// // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
// // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
// // FastLED.addLeds<GS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
// // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<APA106, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<PL9823, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<GE8822, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<LPD1886, DATA_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<LPD1886_8BIT, DATA_PIN, RGB>(leds, NUM_LEDS);
// // ## Clocked (SPI) types ##
// // FastLED.addLeds<LPD6803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
// // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
// // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<WS2803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
// // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
// // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
// // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
// // FastLED.addLeds<SK9822, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
