/* ANALOG INPUT PINS (Ref:https://github.com/Ultrawipf/OpenFFBoard/wiki/Pinouts-and-peripherals) */
uint32_t OpenFFB_Analog_Input_Pins[6] = {
  PA3,  //OpenFFB Analog Input 1
  PA2,  //OpenFFB Analog Input 2
  PC3,  //OpenFFB Analog Input 3
  PC2,  //OpenFFB Analog Input 4
  PC1,  //OpenFFB Analog Input 5
  PC0   //OpenFFB Analog Input 6
};


//===================
// Helper Functions:
//===================

//Load array of enabled Analog Axis Pins
bool EnableAnalogAxis(){
  if (ENABLED_ANALOG_AXIS_NUM > 0 && ENABLED_ANALOG_AXIS_NUM <= 6) {
    //fill analogPins array from ffboard mapping
    for (int i = 0; i < ENABLED_ANALOG_AXIS_NUM; i++) {
      analogPins[i] = OpenFFB_Analog_Input_Pins[i];
      pinMode(analogPins[i], INPUT_ANALOG);
    }
    analogReadResolution(STM32_ADC_RESOLUTION);  //Analog Input Resolution of ADC (Default:12-bit)
    return true;
  }
  return false;
}

//Insert value into CAN Msg Buffer
bool Append_s16(CAN_message_t* msg, int16_t val) {
  size_t start_byte = msg->len;
  if ((start_byte + 2) > 8) return false;        // can't add more data, message is full
  msg->buf[start_byte] = val & 0xFF;             // Low byte of val
  msg->buf[start_byte + 1] = (val >> 8) & 0xFF;  // High byte of val
  msg->len += 2;
  return true;
}

/*Reads Analog Axis and stores into provided array*/
int16_t readAnalogInput(uint32_t ainPin) {
  int16_t val = (int16_t)analogRead(ainPin);

  //TODO: refactor to use standard math over the map function
  val = map(val, 0, 4095, -32767, 32767);  //12bit unsigned (0 to 4095) TO 16bit signed (-32767 to +32767)
  return val;
}