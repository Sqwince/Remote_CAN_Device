
#pragma once

#include <Arduino.h>

#if defined(_STM32_DEF_)
class STM32HWEncoder {
public:
  /**
    Encoder class constructor
    @param ppr  impulses per rotation  (cpr=ppr*4)
    */
  explicit STM32HWEncoder(unsigned int ppr, uint16_t pulse, int8_t pinA, int8_t pinB);


  void init();

  bool initialized = false;
  uint32_t cpr;  //!< encoder cpr number
                 // PinName _pinA, _pinB, _pinI;
  PinName _pinA, _pinB;


  uint32_t getSensorAngle();
  uint32_t getSensorCount();
  uint8_t Update();


  TIM_HandleTypeDef encoder_handle;
private:
  uint8_t _dir;               //Encoder output state: CW = 2 [0b10], CCW = 1 [0b01], None = 0 [0b00]
  int _lastCounter;           //previous counter position
  unsigned long _prevMillis;  //last time input was read
  uint16_t _pulse;            //HID Pulse duration in milliseconds
};

#endif
