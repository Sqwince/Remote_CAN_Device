/*OpenFFboard External Input Controller
========================================
Description: Reads Analog & Digital inputs 
and reports states over CAN to OpenFFBoard-Main
which hosts the motor driver and single interface
to the PC.

AUTHOR: Sqwince
VERSION: 0.1

LICENSE
--------
The MIT License (MIT)

Copyright© 2025 Sqwince

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <Arduino.h>
#include "Config.h"
#include "ShiftIn.h"    //https://github.com/InfectedBytes/ArduinoShiftIn
#include "STM32_CAN.h"  //https://github.com/pazi88/STM32_CAN
#include "Encoder.h"
#include "Error_Handler.h"


//(OpenFFBoard-main v1.2.4, Pins:CAN_RX=PD0 | CAN_TX=PD1, RX Buffer size = 8MB)
STM32_CAN Can(CAN1, ALT_2, RX_SIZE_8, TX_SIZE_8);        //Use PD0/1 pins for CAN1 with RX/TX buffer 8MB
ShiftIn<(SPI_BUTTON_BOARDS * 4)> shift;                  //Init SPI ShiftIn instance with 4x 74HC165 = 32 inputs
ErrorHandler error_handler(LED_RED_Pin, DEBUG_ENABLED);  //Init Error handler (Serial Debugging & onboard RED LED)

//Rotary Knob Encoders
Encoder encoder1(ENC1_A_PIN, ENC1_B_PIN, ENCODER_PULSE_MS);
Encoder encoder2(ENC2_A_PIN, ENC2_B_PIN, ENCODER_PULSE_MS);



/*###############################################*/
/*#######              SETUP              #######*/
/*###############################################*/
void setup() {
// Initialize serial communication for debugging
#if defined(DEBUG_ENABLED)
  Serial.begin(115200);
  Serial.println("Serial initialized.");
#endif

  //blinky blinky
  pinMode(LED_BLU_Pin, OUTPUT);
  //  pinMode(LED_RED_Pin, OUTPUT); //Moved to the error handler
  pinMode(LED_YEL_Pin, OUTPUT);

  // Initialize CAN bus
  Can.begin();
  Can.setBaudRate(CAN_SPEED);
#if defined(DEBUG_ENABLED)
  Serial.println("CAN initialized.");
#endif

  /* INIIALIZE DIGITAL INPUTS */
  if (SPI_BUTTON_BOARDS > 0) {
    DIGITAL_INPUTS_ENABLED = true;
    //Initalize SPI port: pLoadPin, clockEnablePin, dataPin, clockPin
    shift.begin(SPI2_CS_PIN, SPI2_MISO_PIN, SPI2_SCK_PIN);
    FrameCount++;  //Single frame is used for up to 64 digital Buttons
  }

  /* INITIALIZE ENCODERS*/
  //(Supports up to 4 onboard Digital Inputs)
  encoder1.begin();
  encoder2.begin();

  /* INITIALIZE ANALOG INPUTS */
  if (ENABLED_ANALOG_AXIS_NUM > 0) {
    //Add Analog CAN frames required (1-2)
    FrameCount += (((ENABLED_ANALOG_AXIS_NUM + 4) - 1) / 4);

    //Load array of enabled Analog Axis Pins
    for (int i = 0; i < ENABLED_ANALOG_AXIS_NUM; i++) {
      analogPins[i] = OpenFFB_Analog_Input_Pins[i];
      pinMode(analogPins[i], INPUT_ANALOG);
    }

    //Set Analog Input Resolution of ADC (Default:12-bit)
    analogReadResolution(STM32_ADC_RESOLUTION);

    //Error handling
    if (FrameCount == 0 || FrameCount > 3) {
      error_handler.handleError(errorCode_Index_Fault);
      while (true) {}  //No point in going any further
    }
  }

  /* Set Timing */
  //Time between each CAN Frame in ms = (1/120Hz * 1000ms/sec) / (1,2,3)
  CAN_Frame_interval = (((1 / POLLING_FREQUENCY) * 1000) / (FrameCount));
}



/*###############################################*/
/*#######               LOOP              #######*/
/*###############################################*/
void loop() {

  //update encoder states
  encoder1.update();
  encoder2.update();
  // encoder3.update();
  // encoder4.update();



  /*Input Polling Timer*/
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= CAN_Frame_interval) {
    previousMillis = currentMillis;  //save last time polled.

    uint8_t enc1_dir = encoder1.getState();  //0000 0011
    uint8_t enc2_dir = encoder2.getState();  //0000 1100

    if (enc1_dir == 1 || enc2_dir == 1) {
      digitalWrite(LED_RED_Pin, HIGH);
      digitalWrite(LED_BLU_Pin, LOW);
      digitalWrite(LED_YEL_Pin, LOW);
    } else if (enc1_dir == 2 || enc2_dir == 2) {
      digitalWrite(LED_BLU_Pin, HIGH);
      digitalWrite(LED_RED_Pin, LOW);
      digitalWrite(LED_YEL_Pin, LOW);
    } else {
      digitalWrite(LED_YEL_Pin, HIGH);
      digitalWrite(LED_BLU_Pin, LOW);
      digitalWrite(LED_RED_Pin, LOW);
    }


    FrameIndex = (FrameIndex + 1) % FrameCount;  //Cycles through 0, 1, 2 (depending on how many frames needed)

    CAN_message_t CAN_Msg;
    bool status;

    //For which CAN Frame to send (0:Digital 1:64[100], 1:Analog 1:4[110], 2:Analog 5:6[111])
    switch (FrameIndex) {
      case digitalInputs:  // READ DIGITAL BUTTON DATA
        {
          if (DIGITAL_INPUTS_ENABLED) {
            CAN_Msg.len = 0;
            CAN_Msg.id = (CAN_DIGITAL_ID);

            //Load Digital Inputs into CAN Buffer
            status = Append_BTN_States(&CAN_Msg, &shift);

            //ERROR handling
            if (!status) {
              error_handler.handleError(errorCode_CAN_Buf_Overflow);
            } else {
              error_handler.clearError();
            }
            break;  //SPECIAL CASE: If no digital pins used, will overflow to next case.
          }
        }

      /*#######################################################################*/
      case AnalogAxis14:  //READ ANALOG AXIS 1:4 DATA
        {
          CAN_Msg.len = 0;
          CAN_Msg.id = (CAN_AXIS14_ID);

          //Load ANALOG Inputs AXIS1:4 into CAN Buffer
          for (int i = 0; i < 4; i++) {
            if (i < ENABLED_ANALOG_AXIS_NUM) {
              status = Append_s16(&CAN_Msg, readAnalogInput(analogPins[i]));
            } else {
              size_t start_byte = CAN_Msg.len;   //pad frame with zeros
              status = Append_s16(&CAN_Msg, 0);  //TODO: Test without padding. (New Openffboard FW may render this OBE)
            }
          }

          //ERROR handling
          if (!status) { error_handler.handleError(errorCode_CAN_Buf_Overflow); }  // check to make sure buffer didn't overflow
          else {
            error_handler.clearError();
          }
          break;
        }

      /*#######################################################################*/
      case AnalogAxis56:  // READ ANALOG AXIS 1:4 DATA
        {
          CAN_Msg.len = 0;
          CAN_Msg.id = (CAN_AXIS56_ID);
          //Load ANALOG Inputs AXIS5:6 into CAN Buffer
          for (int i = 4; i < 8; i++) {

            if (i < ENABLED_ANALOG_AXIS_NUM) {
              status = Append_s16(&CAN_Msg, readAnalogInput(analogPins[i]));
            } else {
              size_t start_byte = CAN_Msg.len;   //pad frame with zeros
              status = Append_s16(&CAN_Msg, 0);  //TODO: Test without padding. (New Openffboard FW may render this OBE)
            }
          }

          //ERROR handling
          if (!status) { error_handler.handleError(errorCode_CAN_Buf_Overflow); }  // check to make sure buffer didn't overflow
          else {
            error_handler.clearError();
          }
          break;
        }
    }

    //Just gonna send it!!! (https://www.youtube.com/watch?v=RSuLFvalhnQ)
    if (Can.write(CAN_Msg)) {
      if (DEBUG_ENABLED) {
        Serial.print("CAN ID: ");
        Serial.print(CAN_Msg.id);
        Serial.print(" | Data: |");
        for (int i = 0; i < CAN_Msg.len; i++) {
          Serial.print(CAN_Msg.buf[i], HEX);
          Serial.print("|");
        }
        Serial.println("");
      }
    } else {
      error_handler.handleError(errorCode_CAN_Send_Fail);
    }
  }
}

/*###############################################*/
/*#######          HELPER FUNCTIONS       #######*/
/*###############################################*/

//Insert s16b value into CAN Msg Buffer
bool Append_s16(CAN_message_t* msg, int16_t val) {
  size_t start_byte = msg->len;
  if ((start_byte + 2) > 8) return false;        // can't add more data, message is full
  msg->buf[start_byte] = val & 0xFF;             // Low byte of val
  msg->buf[start_byte + 1] = (val >> 8) & 0xFF;  // High byte of val
  msg->len += 2;
  return true;
}

/*Read Analog Axis and re-map values for HID*/
int16_t readAnalogInput(uint32_t ainPin) {
  int16_t val = (int16_t)analogRead(ainPin);
  /* mapped_val=(input_value−in_min)× (out_max−out_min/in_max−in_min)+out_min
    val = ((int32_t)val - in_min) * ((out_max - out_min) / (in_max - in_min)) + out_min;
    in_min = 0
    in_max = 4095
    out_min = -32767
    out_max = 32767  
  12bit uint (0 to 4095) MAP TO 16bit signed (-32767 to +32767)*/
  val = ((int32_t)val * 65534 / 4095) - 32767;
  return val;
}

//Insert Button values into the CAN Msg Buffer
template<uint8_t N>
bool Append_BTN_States(CAN_message_t* _msg, ShiftIn<N>* _shift) {
  uint64_t val = _shift->read();  // Read button values as 64-bit integer

  //Include Rotary Knob Encoder states
  // Shift in Rotary Encoder States
  // val = (val << 2) | encoder4.getState();
  // val = (val << 2) | encoder3.getState();
  val = (val << 2) | encoder2.getState();
  val = (val << 2) | encoder1.getState();

  // Check to ensure the message buffer is empty and can fit 8 bytes
  if (_msg->len != 0) { return false; }

  for (int i = 0; i < 8; i++) {
    _msg->buf[i] = val & 0xFF;  // Extract the lowest byte
    val >>= 8;                  // Shift val right by 8 bits
    _msg->len++;                // Increment the length for each byte added
  }
  return true;
}
