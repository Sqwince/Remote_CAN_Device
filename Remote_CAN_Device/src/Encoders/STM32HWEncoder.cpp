#include "wiring_time.h"
#include "STM32HWEncoder.h"

#if defined(_STM32_DEF_)

/*
  Construtor HardwareEncoder(int ppr, PinA, PinB)
*/
STM32HWEncoder::STM32HWEncoder(unsigned int ppr, uint16_t pulse, int8_t pinA, int8_t pinB)
  : _pulse(pulse) {
  _lastCounter = 0;  //initialize last counter
  _dir = 0;          //initialize direction
  cpr = ppr * 4;     // 4x for quadrature
  _pinA = digitalPinToPinName(pinA);
  _pinB = digitalPinToPinName(pinB);
}

/*
  Get Encoder Output State
*/
uint8_t STM32HWEncoder::Update() {

  //WIP: IN PROGRESS left off here

  uint32_t count = this->getSensorCount();  //get current encoder count
  unsigned long currentMillis = millis();   //get current time

  //Check if count has changed
  if (count > _lastCounter) {         //Increasing
    _dir = 1;                         //CW = 1 [0b01]
    _lastCounter = count;             //
  } else if (count < _lastCounter) {  //Decreasing
    _dir = 2;                         //CCW = 2 [0b10]
    _lastCounter = count;             //
  }

  //Change detected, timer to extend HID pulse
  if (_dir != 3) {
    if (currentMillis - _prevMillis >= _pulse) {
      //timer expired
      _prevMillis = currentMillis;  //record state change time
      _dir = 3;                     //reset dir to end pulse 0b11 (inverted off)
    }
  }

  return _dir;
}




/*
  Shaft angle calculation
*/
uint32_t STM32HWEncoder::getSensorAngle() {
  //return M_TWOPI * encoder_handle.Instance->CNT / static_cast<float>(cpr);
  return M_TWOPI * encoder_handle.Instance->CNT / cpr * 180 / M_PI;  //2*PI * CNT / CPR = angle(radians)   angle(radians) * 180/pi = (degrees)
  //return encoder_handle.Instance->CNT;
}

/*
  Get Encoder counter Value
*/
uint32_t STM32HWEncoder::getSensorCount() {
  return encoder_handle.Instance->CNT;
}


// encoder initialisation of the hardware pins
void STM32HWEncoder::init() {
  // GPIO configuration
  TIM_TypeDef *InstanceA = (TIM_TypeDef *)pinmap_peripheral(_pinA, PinMap_TIM);

  if (!IS_TIM_ENCODER_INTERFACE_INSTANCE(InstanceA)) {
    initialized = false;
    return;
  }
  TIM_TypeDef *InstanceB = (TIM_TypeDef *)pinmap_peripheral(_pinB, PinMap_TIM);
  if (InstanceA != InstanceB) {
    initialized = false;
    return;
  }
  pinmap_pinout(_pinA, PinMap_TIM);
  pinmap_pinout(_pinB, PinMap_TIM);

  //TODO: check width:
  //IS_TIM_32B_COUNTER_INSTANCE(InstanceA);

  // set up timer for encoder
  encoder_handle.Init.Period = cpr - 1;
  encoder_handle.Init.Prescaler = 0;
  encoder_handle.Init.ClockDivision = 0;
  encoder_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  encoder_handle.Init.RepetitionCounter = 0;
  encoder_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  TIM_Encoder_InitTypeDef encoder_config;

  encoder_config.EncoderMode = TIM_ENCODERMODE_TI12;

  //encoder_config.IC1Polarity = TIM_ICPOLARITY_RISING;
  encoder_config.IC1Polarity = TIM_ICPOLARITY_FALLING;  //CubeMX
  encoder_config.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  encoder_config.IC1Prescaler = TIM_ICPSC_DIV1;
  encoder_config.IC1Filter = 0;

  // encoder_config.IC2Polarity = TIM_ICPOLARITY_RISING;
  encoder_config.IC2Polarity = TIM_ICPOLARITY_FALLING;  //CubeMX
  encoder_config.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  encoder_config.IC2Prescaler = TIM_ICPSC_DIV1;
  encoder_config.IC2Filter = 0;

  encoder_handle.Instance = InstanceA;  // e.g. TIM3;
  enableTimerClock(&encoder_handle);

  if (HAL_TIM_Encoder_Init(&encoder_handle, &encoder_config) != HAL_OK) {
    initialized = false;
    return;
  }

  if (HAL_TIM_Encoder_Start(&encoder_handle, TIM_CHANNEL_1) != HAL_OK) {
    initialized = false;
    return;
  }

  initialized = true;
}

#endif