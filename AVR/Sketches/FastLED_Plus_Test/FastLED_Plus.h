#ifndef FASTLED_PLUS_H
#define FASTLED_PLUS_H

#include <FastLED.h>

class FastLED_Plus : public CFastLED {  //Extending FastLED (CFastLED class)
public:
  FastLED_Plus(CRGB* leds, uint16_t numLeds)  // Constructor
    _leds = leds;
  _numLeds = numLeds;

  ~FastLED_Plus() {}  //Destructor

  class RPMGauge : public FastLED_Plus {

    /* Creates a RPM gauge that resembles SimHub RPM Gauge Effect*/
  public:
     drawRPMGauge(CRGB* leds,         //pointer to LED strip
                 uint16_t startPos,  //Starting LED
                 uint16_t ledCount,  //Number of LEDs to include in effect
                 CRGB startColor,    //Starting Color in CRGB format
                 CRGB endColor,      //Ending color in CRGB format
                 uint16_t minRPM,    //minimum RPM value (Default: 0%)
                 uint16_t maxRPM,    //maximum RPM value (Default: 100%)
                 bool useDimming,    //fade last LED
                 bool rightToLeft,   //animation direction
                 //TODO: optional include redline flashing with thse optional args?
                 CRGB redlineColor1,    //Color 1 of RPM Redline animation blink
                 CRGB redlineColor2,    //Color 2 of PRM redline animation blink
                 uint16_t blinkDelay);  //blink delay for redline blink animation


//Left off here



    void updateRPMGauge(uint16_t RPMs);



    void breathe(CRGB* leds, uint16_t numLeds, uint8_t speed);


    void chase(CRGB* leds, uint16_t numLeds, uint8_t color, uint8_t speed);

  private:
    CRGB* _leds;
    uint16_t _numLeds;
    uint8_t _effectIndex;
  };




#endif  //END FASTLED_PLUS_H



  // #include <FastLED.h>

  // template<const int dataPin>  //LED Strip DataIN connected to this MCU pin
  // class RPMGauge {
  // public:
  //   /**
  //      * @brief Constructor for RPMGauge
  //      * @param dataPin LED Strip Data connected to this MCU data pin
  //      * @param startPos Starting LED index in the strip
  //      * @param ledCount Number of LEDs used for the gauge
  //      * @param startColor Color at the lowest RPM
  //      * @param endColor Color at the highest RPM
  //      * @param bgColor Default color of the background LEDs
  //      * @param minRPM Minimum RPM value (0% level)
  //      * @param maxRPM Maximum RPM value (100% level)
  //      * @param redlineBlinkMs Blinking interval for redline alert
  //      * @param useDimming Whether to use dimming effect (default: true)
  //      * @param rightToLeft Direction of LED fill (default: true)
  //      */
  //   RPMGauge(
  //     const int startPos,             //  Arg1: Starting LED index in the strip
  //     const int ledCount,             //  Arg2: Number of LEDs used for the gauge
  //     CRGB startColor,                //  Arg4: Color at the lowest RPM
  //     CRGB endColor,                  //  Arg5: Color at the highest RPM
  //     CRGB bgColor,                   //  Arg3: Default color of the background LEDs
  //     const int minRPM,               //  Arg6: Minimum RPM value (0% level)
  //     const int maxRPM,               //  Arg7: Maximum RPM value (100% level)
  //     const int redlineBlinkMs,       //  Arg8: Blinking interval for redline alert
  //     const bool useDimming = true,   //  Arg9: Whether to use dimming effect
  //     const bool rightToLeft = true)  // Arg10: Direction of LED fill (true = right to left)
  //     : startPos(startPos), ledCount(ledCount), startColor(startColor), endColor(endColor), bgColor(bgColor),
  //       minRPM(minRPM), maxRPM(maxRPM), redlineBlinkMs(redlineBlinkMs), useDimming(useDimming),
  //       rightToLeft(rightToLeft), lastBlinkTime(0), blinkState(false) {

  //     // Allocate memory for the LED array
  //     leds = new CRGB[ledCount];

  //     // Initialize the LED strip (Assuming data pin 6)
  //     FastLED.addLeds<WS2812B, dataPin, GRB>(leds, ledCount);
  //   }
  //   //Destructor
  //   ~RPMGauge() {
  //     delete[] leds;  // Free allocated memory
  //   }

  //   //Upate LED colors in array based on RPM% value
  //   void update(int rpmPercentage) {
  //     int ledPosition = map(rpmPercentage, minRPM, maxRPM, 0, ledCount * dimmingSteps);
  //     int fullyOnIndex = ledPosition / dimmingSteps;  // Whole LED index
  //     int fadeStep = ledPosition % dimmingSteps;      // Steps for fading each LED



  //     bool RPM_redlined;


  //     RPM_redlined = (rpmPercentage >= maxRPM) ? true : false;


  //     //Fill gradiant based on RPMs
  //     if (!RPM_redlined) {
  //       // Define the color gradient from Blue to Red
  //       for (int i = 0; i < ledCount; i++) {
  //         leds[i] = blend(startColor, endColor, (i * 255) / (ledCount - 1));
  //       }

  //       // Adjust brightness
  //       for (int i = 0; i < ledCount; i++) {
  //         if (i < fullyOnIndex) {
  //           leds[i].maximizeBrightness();  // Fully on
  //         } else if (i == fullyOnIndex) {
  //           uint8_t brightness = map(fadeStep, 0, dimmingSteps - 1, 0, 255);
  //           leds[i].nscale8(brightness);
  //         } else {
  //           leds[i] = bgColor;
  //         }
  //       }
  //     } else {

  //       //Redline Blink Animation
  //       //if (rpmPercentage >= maxRPM && millis() - lastBlinkTime >= redlineBlinkMs) {
  //       if (millis() - lastBlinkTime >= redlineBlinkMs) {
  //         blinkState = !blinkState;
  //         lastBlinkTime = millis();

  //         // Debugging
  //         // Serial.print("Blink State: ");
  //         // Serial.println(blinkState ? "RED" : "BLACK");

  //         for (int i = 0; i < ledCount; i++) {
  //           leds[i] = blinkState ? CRGB::Red : CRGB::Black;
  //         }
  //         FastLED.show();
  //       }
  //     }
  //   }

  //   //Update all our controllers with the current led colors
  //   void show() {
  //     FastLED.show();
  //   }

  // private:
  //   const int startPos;           // Starting LED index
  //   const int ledCount;           // Total number of LEDs used in gauge
  //   CRGB startColor;              // Color at the lowest RPM
  //   CRGB endColor;                // Color at the highest RPM
  //   CRGB bgColor;                 // Default color of the background LEDs
  //   const int minRPM;             // Minimum RPM value
  //   const int maxRPM;             // Maximum RPM value
  //   const int redlineBlinkMs;     // Blink interval for redline warning
  //   const bool useDimming;        // Enable/disable dimming effect
  //   const bool rightToLeft;       // Direction of LED fill
  //   const int dimmingSteps = 8;   // Dimming steps for LED before next starts to dim
  //   unsigned long lastBlinkTime;  // Last recorded blink time
  //   bool blinkState;              // State of blinking for redline alert
  //   CRGB* leds;                   // Pointer to the LED array
  // };

  // #endif



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
