/* Based on Oleg Mazurov's code for rotary encoder interrupt service routines for AVR micros
   here https://chome.nerpa.tech/mcu/reading-rotary-encoder-on-arduino/
   and using interrupts https://chome.nerpa.tech/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros/
 
 
   This example does not use the port read method. Tested with Nano and ESP32
   both encoder A and B pins must be connected to interrupt enabled pins
*/
#include "Encoder.h"

// Lookup table for encoder state transitions
static const int8_t enc_states[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

// Initialize static variables
Encoder* Encoder::instances[MAX_ENCODERS] = { nullptr };
int Encoder::encoderCount = 0;

// Constructor
Encoder::Encoder(uint8_t pinA, uint8_t pinB, uint16_t pulse)
  : _pinA(pinA), _pinB(pinB), _counter(0), _lastCounter(0), _oldAB(3), _encval(0), _dir(0), _pulse(pulse) {
  if (encoderCount < MAX_ENCODERS) {
    _index = encoderCount;
    instances[encoderCount++] = this;
  } else {
    _index = 255;  // Invalid index if max instances exceeded
  }
}


// Initialize pins and interrupts
void Encoder::begin() {
  pinMode(_pinA, INPUT_PULLUP);
  pinMode(_pinB, INPUT_PULLUP);

  // Attach the correct interrupt based on instance index
  switch (_index) {
    case 0:
      attachInterrupt(digitalPinToInterrupt(_pinA), readEncoderWrapper0, CHANGE);
      attachInterrupt(digitalPinToInterrupt(_pinB), readEncoderWrapper0, CHANGE);
      break;
    case 1:
      attachInterrupt(digitalPinToInterrupt(_pinA), readEncoderWrapper1, CHANGE);
      attachInterrupt(digitalPinToInterrupt(_pinB), readEncoderWrapper1, CHANGE);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(_pinA), readEncoderWrapper2, CHANGE);
      attachInterrupt(digitalPinToInterrupt(_pinB), readEncoderWrapper2, CHANGE);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(_pinA), readEncoderWrapper3, CHANGE);
      attachInterrupt(digitalPinToInterrupt(_pinB), readEncoderWrapper3, CHANGE);
      break;
  }
}


//Updates the encoder output state
void Encoder::update() {

  unsigned long currentMillis = millis();  //get current time

  if (_dir != 3) {
    // knob has turned start counting
    if (currentMillis - _prevMillis >= _pulse) {
      //timer expired
      _prevMillis = currentMillis;  //record state change time
      _dir = 3;                     //reset dir to end pulse 0b11 (inverted off)
    }
  }
}


// Get encoder count
int Encoder::getCount() const {
  return _counter;
}

// Get encoder state
uint8_t Encoder::getState() const {
  return _dir;
}

// Reset encoder count
void Encoder::reset() {
  _counter = 0;
}


// ISR Wrappers to call correct instance
void Encoder::readEncoderWrapper0() {
  if (instances[0]) instances[0]->readEncoder();
}
void Encoder::readEncoderWrapper1() {
  if (instances[1]) instances[1]->readEncoder();
}
void Encoder::readEncoderWrapper2() {
  if (instances[2]) instances[2]->readEncoder();
}
void Encoder::readEncoderWrapper3() {
  if (instances[3]) instances[3]->readEncoder();
}

// Actual ISR function
void Encoder::readEncoder() {
  _oldAB <<= 2;

  if (digitalRead(_pinA)) _oldAB |= 0x02;
  if (digitalRead(_pinB)) _oldAB |= 0x01;

  _encval += enc_states[_oldAB & 0x0F];

  if (_encval > 3) {
    _dir = 1;  //CW = 1 [0b01]
    _encval = 0;
  } else if (_encval < -3) {
    _dir = 2;  //CCW = 2 [0b10]
    _encval = 0;  
  }

  //update time when triggered
  _prevMillis = millis();
}