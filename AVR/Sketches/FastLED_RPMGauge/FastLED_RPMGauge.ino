#include <FastLED.h>
#include "RPMGauge.h"

#define NUM_LEDS 18
#define DATA_PIN 6
#define DIMMING_STEPS 8
#define MAX_BRIGHTNESS 100                                          //maximum brightness [0-255]
#define DEBUG_ENABLED true                                          //serial output messages
#define REFRESH_RATE 10                                             //RefreshRate [10 FPS]
#define POT_PIN A1                                                  // Potentiometer input to represent RPMs
unsigned long previousMillis = 0;                                   //last time HID was polled
const long refreshRateDelayInMillis = ((1 / REFRESH_RATE) * 1000);  //HID Polling delay in milliseconds
const int half_LED_Num = NUM_LEDS / 2;

RPMGauge<6> rpmGauge(
  0,            //  Arg1: Starting LED index in the strip
  NUM_LEDS,     //  Arg2: Number of LEDs used for the gauge
  CRGB::Green,   //  Arg3: Color at the lowest RPM
  CRGB::Red,    //  Arg4: Color at the highest RPM
  CRGB::Black,  //  Arg5: Default color of the LEDs
  0,            //  Arg6: Minimum RPM value (0% level)
  100,          //  Arg7: Maximum RPM value (100% level)
  250,          //  Arg8: Blinking interval for redline alert
  true,         //  Arg9: Whether to use dimming effect
  false);       // Arg10: Direction of LED fill (true = right to left)

/*#############################################################################*/
void setup() {

  // FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  //GRB ordering
  FastLED.setBrightness(MAX_BRIGHTNESS);  //max brightness

  FastLED.clear();
  FastLED.show();
  if (DEBUG_ENABLED) {
    Serial.begin(115200);
    Serial.println(">>Serial Initialized.");
  }
}

/*#############################################################################*/

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= refreshRateDelayInMillis) {
    previousMillis = currentMillis;
    int potValue = analogRead(POT_PIN);
    int rpmPercentage = map(potValue, 0, 1023, 0, 100);  // Convert to percentage
    if (DEBUG_ENABLED) {
      Serial.print("potValue");
      Serial.print(potValue);
      Serial.print(" | rpmPercentage");
      Serial.println(rpmPercentage);
    }
    rpmGauge.update(rpmPercentage);
    rpmGauge.show();
  }
}














/*#############################################################################
void loop() {
  /* Timer 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= RefreshRateDelayInMillis) {
    previousMillis = currentMillis;  //save last time polled.

    //Get RPM value (Potentiometer for now)
    int potValue = analogRead(potPin);
    int ledPosition = map(potValue, 0, 1023, 0, half_LED_Num * DIMMING_STEPS);  // 4 steps per LED

    int fullyOnIndex = ledPosition / DIMMING_STEPS;  // Whole LED index
    int fadeStep = ledPosition % DIMMING_STEPS;      // 0-3 step for fading

    // Define the color gradient from Blue to Red
    for (int i = 0; i < half_LED_Num; i++) {
      leds[i] = blend(CRGB::Green, CRGB::Red, (i * 255) / (half_LED_Num - 1));
    }

    // Adjust brightness
    for (int i = 0; i < half_LED_Num; i++) {
      if (i < fullyOnIndex) {
        leds[i].maximizeBrightness();  // Fully on
      } else if (i == fullyOnIndex) {
        uint8_t brightness = map(fadeStep, 0, DIMMING_STEPS - 1, 0, 255);  // Start dim, increase brightness
        leds[i].nscale8(brightness);
      } else {
        leds[i].nscale8(0);  // Off
      }
    }

    for (int i = 0; i < half_LED_Num + 1; i++) {
      leds[(NUM_LEDS - 1) - i] = leds[i];
    }
    FastLED.show();
  }
}
*/