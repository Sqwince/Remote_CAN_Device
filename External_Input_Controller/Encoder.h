/* Based on Oleg Mazurov's code for rotary encoder interrupt service routines for AVR micros
   here https://chome.nerpa.tech/mcu/reading-rotary-encoder-on-arduino/
   and using interrupts https://chome.nerpa.tech/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros/
 
 
   This example does not use the port read method. Tested with Nano and ESP32
   both encoder A and B pins must be connected to interrupt enabled pins
*/

#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>



class Encoder {
public:
  Encoder(uint8_t pinA, uint8_t pinB, uint16_t pulse = 50);  // Constructor: A/B inputs | [optional] pulse duration for HID detection
  void begin();                                              // Initialize encoder
  void update();                                             // Update pulse state
  uint8_t getState() const;                                  // Get current output state
  int getCount() const;                                      // Get encoder count
  void reset();                                              // Reset counter

private:
  uint8_t _pinA, _pinB;       // Encoder pins
  volatile int _counter;      // Counter for encoder position
  int _lastCounter;           // previous counter position
  volatile uint8_t _oldAB;    // Previous state of encoder
  volatile int8_t _encval;    // Temporary value for tracking encoder steps
  uint8_t _index;             // Index for identifying the encoder in the ISR
  unsigned long _prevMillis;  // last time input was read
  uint16_t _pulse;            // Output pulse duration for HID input compatibility
  uint8_t _dir;               // CW = 2 [0b10], CCW = 1 [0b01], None = 0 [0b00]


  static const int MAX_ENCODERS = 4;        // Adjust as needed
  static Encoder* instances[MAX_ENCODERS];  // Store instances
  static int encoderCount;                  // Track the number of encoders

  static void readEncoderWrapper0();
  static void readEncoderWrapper1();
  static void readEncoderWrapper2();
  static void readEncoderWrapper3();
  void readEncoder();  // Actual ISR function
};

#endif  // ENCODER_H