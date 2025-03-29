/*Examples of the RGB LED Effects available to use */



/*#############################################################################*/
/* RPMs EFFECT with redline flashing animation */
//CRGB *leds          //pointer to LED strip
//bool rightToLeft    //animation direction
//uint16_t startPos   //Starting LED
//uint16_t ledCount   //Number of LEDs to include in effect
//CRGB startColor     //Starting Color in CRGB format
//CRGB endColor       //Ending color in CRGB format
//uint16_t minRPM,    //minimum RPM value (Default: 0%)
//uint16_t maxRPM     //maximum RPM value (Default: 100%)
//bool redlineBlink   //blink Color 1 & 2 when RPM = 100%
//CRGB redlineColor1  //Color 1 of RPM Redline animation blink
//CRGB redlineColor2  //Color 2 of PRM redline animation blink

/*EXAMPLE: RPMs EFFECT 0-100% with Redline Animation */
RPMsEffect RPMs_Left(leds, false, 3, 6, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Red, CRGB::Blue);
RPMsEffect RPMs_Right(leds, true, 9, 6, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Blue, CRGB::Red);



/*#############################################################################*/
/* Blink EFFECT Animations */
// CRGB* leds,                  //pointer to LED strip
//blinkPattern pattern,        //animation pattern
//uint16_t startPos,           //Starting LED (BASE 0)
//uint16_t ledCount,           //Number of LEDs to include in effect
//CRGB color1,                 //Color in CRGB format
//CRGB color2,                 //Color in CRGB format
//CRGB color3 = CRGB::Black,   //Color in CRGB format
//uint16_t blinkDelay = 100);  //blink delay for redline blink animation (Default = 100ms)

/*EXAMPLE: Yellow Flag EFFECT Blink Animation */
BlinkEffect yellowFlag1(leds, BlinkEffect::flashing, 0, 3, CRGB::Yellow, CRGB::Black);
BlinkEffect yellowFlag2(leds, BlinkEffect::flashing, 15, 3, CRGB::Yellow, CRGB::Black);

//Examples of all variants:
BlinkEffect blink1(leds, BlinkEffect::flashing, 0, 6, CRGB::Red, CRGB::Green);
BlinkEffect blink2(leds, BlinkEffect::alternateTwo, 6, 6, CRGB::Orange, CRGB::Blue);
BlinkEffect blink3(leds, BlinkEffect::alternateThree, 12, 6, CRGB::Yellow, CRGB::Purple, CRGB::Black);



/*#############################################################################*/
/* Scrolling EFFECT Animations */
// CRGB* leds,                  //pointer to LED strip
//bool rightToLeft,            //animation direction
//ScrollPattern pattern,       //selected animation for effect
//uint16_t startPos,           //Starting LED
//uint16_t ledCount,           //Number of LEDs to include in effect
//uint16_t ledSpacing,         //distance between lit LEDs (between 1 and ledCount)
//CRGB color1,                 //Color in CRGB format
//CRGB color2,                 //Color in CRGB format
//uint16_t scrollSpeed = 50);  //how fast to scroll the effect

ScrollEffect scroll1(leds, true, ScrollEffect::chase, 0, 6, 4, CRGB::Blue, CRGB::Black);
ScrollEffect scroll2(leds, false, ScrollEffect::invertedChase, 6, 6, 4, CRGB::Purple, CRGB::Black);
ScrollEffect scroll3(leds, true, ScrollEffect::chaseDimmed1, 12, 6, 3, CRGB::Red, CRGB::Black);