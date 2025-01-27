/*OpenFFboard External Input Controller
========================================
Description: Reads Analog & Digital inputs 
and reports status over CAN to OpenFFBoard-Main

OpenFFBoarD CAN Input Protocol:
BAUD: 500k
Packet Size: 8 bytes

Digital Input CAN FRAME:
------------------------
ID: 100
Each bit = 1 digital I/O
32 of 64 possible mapped below in one CAN Frame:
   |11111111|11111111|11111111|11111111|
   |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|

Analog Input CAN Frame:
------------------------
HID expects signed integer range (MIN:-32767[0x7fff],CENTER:0[0x0000], MAX:+32767[0x8001])

Frame ID: 110
|axis1[0:7], axis1[8:15]||axis2[0:7], axis2[8:15]||axis3[0:7], axis3[8:15]||axis4[0:7], axis4[8:15]||
Frame ID: 111
|axis5[0:7], axis5[8:15]||axis6[0:7], axis6[8:15]||xx||xx||xx||xx|
*/

#include <Arduino.h>
#include "ShiftIn.h"    //https://github.com/InfectedBytes/ArduinoShiftIn
#include "STM32_CAN.h"  //https://github.com/pazi88/STM32_CAN
#include "External_Input_Controller.h"


/*#######   GENERAL SETUP Parameters   #######*/
#define POLLING_FREQUENCY 120  //HID polling frequency [Default: 120Hz]
#define CAN_SPEED 500000       // 500 kbps
#define CAN_TXMSG_SIZE 8       // 8 Bytes


/*#######   ANALOG INPUT SETTINGS   #######*/
#define ENABLED_ANALOG_AXIS_NUM 6  //Number of analog inputs used (Range: 1-6)

#if (ENABLED_ANALOG_AXIS_NUM > 0)
#define STM32_ADC_RESOLUTION 12            //Analog Input ADC Resolution (Default: 12 , STM32 ADC = 12-bit(0 to 4095)
#define CAN_AXIS14_ID 110                  // CAN Frame ID for Analog Input Data Axis1:4 [Default: 110]
#define CAN_AXIS56_ID = CAN_AXIS14_ID + 1  //CAN Frame ID for Analog Input Data Axis5:6 [Default: 111]
#endif




/*#######   DIGITAL INPUT SETTINGS   #######*/
#define CAN_DIGITAL_ID 100   // CAN Frame ID for Analog Input Data [Default: 110]
#define SPI_BUTTON_BOARDS 1  //Number of SPI-Button boards used (1-2)
//---  Openffbaord SPI2 Interface Pins Definition  ---
#define SPI2_SCK_PIN PB13   //Clock
#define SPI2_MISO_PIN PB14  //Data In from last shift register in chain
#define SPI2_CS_PIN PB12    //Data Latch



//(OpenFFBoard-main v1.2.4, Pins:CAN_RX=PD0 | CAN_TX=PD1, RX Buffer size = 8MB)
STM32_CAN Can(CAN1, ALT_2, RX_SIZE_8, TX_SIZE_8);  //Use PD0/1 pins for CAN1 with RX/TX buffer 8MB
ShiftIn<SPI_BUTTON_BOARDS * 4> shift;              //Init SPI ShiftIn instance with 4x 74HC165 = 32 inputs

//array of pin#s for enabled analog axis
uint32_t analogPins[ENABLED_ANALOG_AXIS_NUM];

uint8_t AnalogFrameCount = (((ENABLED_ANALOG_AXIS_NUM + 4) - 1) / 4);  // How many analog CAN frames needed.
uint8_t FrameIndex = 0;                                                //For which CAN Frame to send (0:Digital 1:64[100], 1:Analog 1:4[110], 2:Analog 5:6[111])
uint8_t CAN_Frame_interval = (POLLING_FREQUENCY / (1 + FrameCount));

unsigned long previousMillis = 0;                                       //last time HID was polled
const long HIDpollingDelayInMillis = ((1 / POLLING_FREQUENCY) * 1000);  //HID Polling delay in milliseconds




//========================
//         SETUP         |
//========================
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("Serial initialized.");

  // Initialize CAN bus
  Can.begin();
  Can.setBaudRate(CAN_SPEED);
  Serial.println("CAN initialized.");

  if (!EnableAnalogAxis()) {Serial.println("ERROR: Analog Axis number out of bounds (Valid 0-6)") }
}


//=====================
//        LOOP        |
//=====================
void loop() {
  /*HID Polling Timer*/
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= HIDpollingDelayInMillis) {
    previousMillis = currentMillis;  //save last time polled.


    //For which CAN Frame to send (0:Digital 1:64[100], 1:Analog 1:4[110], 2:Analog 5:6[111])
    switch (FrameIndex) {
      /* SEND DIGITAL BUTTON DATA*/
      case 0:
        {
          CAN_message_t CAN_Msg;
          bool status;
          CAN_Msg.len = 0;
          CAN_Msg.id = (CAN_DIGITAL_ID);



          break;
        }  //END CASE 0
      /* SEND ANALOG AXIS 1:4 DATA*/
      case 1:  // your hand is on the sensor
        {
          CAN_message_t CAN_Msg;
          bool status;
          CAN_Msg.len = 0;
          CAN_Msg.id = (CAN_AXIS14_ID);


          break;
        }  //END CASE 1

      /* SEND ANALOG AXIS 5:6 DATA*/
      case 2:  // your hand is on the sensor
        {
          CAN_message_t CAN_Msg;
          bool status;
          CAN_Msg.len = 0;
          CAN_Msg.id = (CAN_AXIS56_ID);


          break;
        }  //END CASE 2
    }
  }
}











/*
    //Message Counter
    for (int j = 0; j < 2; j++) {
      CAN_message_t CAN_Msg;
      bool status;
      CAN_Msg.len = 0;
      CAN_Msg.id = (CAN_ANALOG_ID + j);  //110

      //Add Axis to the CAN buffer (4 / message)
      for (int i = 0; i < 4; i++) {
        int axisIndex = (4 * j) + i;
        if (axisIndex < ENABLED_ANALOG_AXIS_NUM) {
          status = Append_s16(&CAN_Msg, readAnalogInput(analogPins[axisIndex]));
        } else {
          size_t start_byte = CAN_Msg.len;
          status = Append_s16(&CAN_Msg, 0);
          //  CAN_Msg.buf[start_byte] = 0;
          //  CAN_Msg.buf[start_byte+1] = 0;
          //  CAN_Msg.len+= 2;
        }


        //ERROR handling
        if (!status) {  // check to make sure buffer didn't overflow
          Serial.println("ERROR: CAN Buffer Overflow!!");
        }
      }

      //Send it!
      if (Can.write(CAN_Msg)) {
        Serial.print("CAN ID: ");
        Serial.print(CAN_Msg.id);
        Serial.print(" | Data: |");
        for (int i = 0; i < 8; i++) {
          Serial.print(CAN_Msg.buf[i], HEX);
          Serial.print("|");
        }
        Serial.println("");
      } else {
        Serial.println("Error sending CAN frame.");
      }
      delay(1);
    }
  }
}

/* Reference Information*/
/*OpenFFboard External CAN Input monitor
======================================
Description: Reads Digital and Analog I/O and reports status over CAN to OpenFFBoard-Main

OpenFFBoarD CAN Input Protocol:
BAUD: 500k
Packet Size: 8 bytes

Digital Input CAN FRAME:
------------------------
ID: 100
Each bit = 1 digital I/O
32 of 64 possible mapped below in one CAN Frame:
   |11111111|11111111|11111111|11111111|
   |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|

Analog Input CAN Frame:
------------------------
HID expects signed integer range (MIN:-32767[0x7fff],CENTER:0[0x0000], MAX:+32767[0x8001])

Frame ID: 110
|axis1[0:7], axis1[8:15]||axis2[0:7], axis2[8:15]||axis3[0:7], axis3[8:15]||axis4[0:7], axis4[8:15]||
Frame ID: 111
|axis5[0:7], axis5[8:15]||axis6[0:7], axis6[8:15]||xx||xx||xx||xx|
*/
