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
#include "ShiftIn.h"    //https://github.com/InfectedBytes/ArduinoShiftIn
#include "STM32_CAN.h"  //https://github.com/pazi88/STM32_CAN

enum ERROR_Code {
  errorCode_Index_Fault,       //"ERROR: INPUT number out of bounds."
  errorCode_CAN_Buf_Overflow,  //"ERROR: CAN Buffer Overflow!!"
  errorCode_CAN_Send_Fail,     //"ERROR: Sending CAN frame failed."
};

/*#########################################*/
/*#######   ANALOG INPUT SETTINGS   #######*/
/*#########################################*/
// HID expects signed integer range (MIN:-32767[0x7fff],CENTER:0[0x0000], MAX:+32767[0x8001])
// Frame ID: 110 -> |axis1[0:7], axis1[8:15]||axis2[0:7], axis2[8:15]||axis3[0:7], axis3[8:15]||axis4[0:7], axis4[8:15]||
// Frame ID: 111 -> |axis5[0:7], axis5[8:15]||axis6[0:7], axis6[8:15]||xx||xx||xx||xx|

#define ENABLED_ANALOG_AXIS_NUM 6              //Number of analog inputs used (Range: 1-6)
#define CAN_AXIS14_ID 110                      //CAN Frame ID for Analog Input Data Axis1:4 [Default: 110]
#define CAN_AXIS56_ID (CAN_AXIS14_ID + 1)      //CAN Frame ID for Analog Input Data Axis5:6 [Default: 111]
#define STM32_ADC_RESOLUTION 12                //Analog Input ADC Resolution (Default: 12 , STM32 ADC = 12-bit(0 to 4095)
uint32_t analogPins[ENABLED_ANALOG_AXIS_NUM];  //array of pin#s for enabled analog axis

//TODO: Convert this to enum?
uint32_t OpenFFB_Analog_Input_Pins[6] = {
  PA3,  //OpenFFB Analog Input 1
  PA2,  //OpenFFB Analog Input 2
  PC3,  //OpenFFB Analog Input 3
  PC2,  //OpenFFB Analog Input 4
  PC1,  //OpenFFB Analog Input 5
  PC0   //OpenFFB Analog Input 6
};      //Ref:https://github.com/Ultrawipf/OpenFFBoard/wiki/Pinouts-and-peripherals)


/*##########################################*/
/*#######   DIGITAL INPUT SETTINGS   #######*/
/*##########################################*/
// ID: 100
// Each bit = 1 digital I/O
// 32 of 64 possible mapped below in one CAN Frame:
//    |11111111|11111111|11111111|11111111|
//    |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|
#define CAN_DIGITAL_ID 100   // CAN Frame ID for Analog Input Data [Default: 110]
#define SPI_BUTTON_BOARDS 1  //Number of SPI-Button boards used (0-2)
bool DIGITAL_INPUTS_ENABLED = false;

//---  Openffbaord SPI2 Interface Pins Definition  ---
#define SPI2_SCK_PIN PB13   //Clock
#define SPI2_MISO_PIN PB14  //Data In from last shift register in chain
#define SPI2_CS_PIN PB12    //Data Latch


/*###############################################*/
/*#######   General & Timing Parameters   #######*/
/*###############################################*/
#define POLLING_FREQUENCY 120  //Input polling frequency [Default: 120Hz]
#define CAN_SPEED 500000       //500 kbps for OpenFFBoard
#define CAN_TXMSG_SIZE 8       //8 Bytes
#define DEBUG_ENABLED 1        //serial print debug msgs
uint8_t LED_BLU_Pin = PD7;     //Blue   (Sys)
uint8_t LED_RED_Pin = PE1;     //Red    (Error)
uint8_t LED_YEL_Pin = PE0;     //Yellow (Clip)

//(OpenFFBoard-main v1.2.4, Pins:CAN_RX=PD0 | CAN_TX=PD1, RX Buffer size = 8MB)
STM32_CAN Can(CAN1, ALT_2, RX_SIZE_8, TX_SIZE_8);  //Use PD0/1 pins for CAN1 with RX/TX buffer 8MB
ShiftIn<(SPI_BUTTON_BOARDS * 4)> shift;            //Init SPI ShiftIn instance with 4x 74HC165 = 32 inputs
uint8_t CAN_Frame_interval = 8;                    //Time in ms between sending CAN Frames
uint8_t FrameCount = 0;                            //Number of CAN Frames enabled
uint8_t FrameIndex = 0;                            // Keeps track of the current frame
unsigned long previousMillis = 0;                  //last time inputs were read

enum FrameIndexNumber {
  digitalInputs,
  AnalogAxis14,
  AnalogAxis56,
};


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
  pinMode(LED_RED_Pin, OUTPUT);
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
      errorHandling(errorCode_Index_Fault);
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
  /*Input Polling Timer*/
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= CAN_Frame_interval) {
    previousMillis = currentMillis;              //save last time polled.
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
              errorHandling(errorCode_CAN_Buf_Overflow);
            } else {
              errorClear();
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
          if (!status) { errorHandling(errorCode_CAN_Buf_Overflow); }  // check to make sure buffer didn't overflow
          else {
            errorClear();
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
          if (!status) { errorHandling(errorCode_CAN_Buf_Overflow); }  // check to make sure buffer didn't overflow
          else {
            errorClear();
          }
          break;
        }
    }

    //Just gonna send it!!! (https://www.youtube.com/watch?v=RSuLFvalhnQ)
    if (Can.write(CAN_Msg)) {
      Serial.print("CAN ID: ");
      Serial.print(CAN_Msg.id);
      Serial.print(" | Data: |");
      for (int i = 0; i < CAN_Msg.len; i++) {
        Serial.print(CAN_Msg.buf[i], HEX);
        Serial.print("|");
      }
      Serial.println("");
    } else {
      errorHandling(errorCode_CAN_Send_Fail);
    }
  }
}


/*###############################################*/
/*#######          HELPER FUNCTIONS       #######*/
/*###############################################*/

//Insert value into CAN Msg Buffer
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
  // Check to ensure the message buffer is empty and can fit 8 bytes
  if (_msg->len != 0) { return false; }
  for (int i = 0; i < 8; i++) {
    _msg->buf[i] = val & 0xFF;  // Extract the lowest byte
    val >>= 8;                  // Shift val right by 8 bits
    _msg->len++;                // Increment the length for each byte added
  }
  return true;
}


/*###############################################*/
/*#######          ERROR HANDLING         #######*/
/*###############################################*/
void errorClear() {
  digitalWrite(LED_RED_Pin, LOW);
}

void errorHandling(ERROR_Code errorCode) {
  //TODO: Blink red ERROR Light
  digitalWrite(LED_RED_Pin, HIGH);
//Serial Debugger
#if defined(DEBUG_ENABLED)
  switch (errorCode) {
    case errorCode_Index_Fault:
      {
        Serial.println("ERROR: INPUT number out of bounds.");
        break;
      }
    case errorCode_CAN_Buf_Overflow:
      {
        Serial.println("ERROR: CAN Buffer Overflow!!");
        break;
      }
    case errorCode_CAN_Send_Fail:
      {
        Serial.println("ERROR: Sending CAN frame failed.");
        break;
      }
  }
#endif
}
