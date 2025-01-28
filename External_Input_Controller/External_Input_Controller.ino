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

/*#######   GENERAL SETUP Parameters   #######*/
#define POLLING_FREQUENCY 120  //HID polling frequency [Default: 120Hz]
#define CAN_SPEED 500000       // 500 kbps
#define CAN_TXMSG_SIZE 8       // 8 Bytes


/*#########################################*/
/*#######   ANALOG INPUT SETTINGS   #######*/
/*#########################################*/
#define ENABLED_ANALOG_AXIS_NUM 6          //Number of analog inputs used (Range: 1-6)
#define CAN_AXIS14_ID 110                  //CAN Frame ID for Analog Input Data Axis1:4 [Default: 110]
#define CAN_AXIS56_ID (CAN_AXIS14_ID + 1)  //CAN Frame ID for Analog Input Data Axis5:6 [Default: 111]
#define STM32_ADC_RESOLUTION 12            //Analog Input ADC Resolution (Default: 12 , STM32 ADC = 12-bit(0 to 4095)
// #if (ENABLED_ANALOG_AXIS_NUM > 0)
// #define ANALOG_INPUTS_ENABLED true
// #else
// #define ANALOG_INPUTS_ENABLED false
// #endif
int analogFrames = 0;                          //Number of Analog Frames reuired (0-2)
uint32_t analogPins[ENABLED_ANALOG_AXIS_NUM];  //array of pin#s for enabled analog axis

/* ANALOG INPUT PINS */
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
//(OpenFFBoard-main v1.2.4, Pins:CAN_RX=PD0 | CAN_TX=PD1, RX Buffer size = 8MB)
STM32_CAN Can(CAN1, ALT_2, RX_SIZE_8, TX_SIZE_8);  //Use PD0/1 pins for CAN1 with RX/TX buffer 8MB
ShiftIn<SPI_BUTTON_BOARDS * 4> shift;              //Init SPI ShiftIn instance with 4x 74HC165 = 32 inputs
uint8_t FrameIndex = 0;                            //For which CAN Frame to send (0:Digital 1:64[100], 1:Analog 1:4[110], 2:Analog 5:6[111])
uint8_t CAN_Frame_interval;                        //Time in ms between sending CAN Frames
uint8_t FrameCount = 0;                            //Number of CAN Frames enabled
unsigned long previousMillis = 0;                  //last time inputs were read






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



  /* INIIALIZE DIGITAL INPUTS */
  if (SPI_BUTTON_BOARDS > 0) {
    DIGITAL_INPUTS_ENABLED = true;
    FrameCount++;  // 1 frame used for all digital Buttons
  }



  /* INITIALIZE ANALOG INPUTS */
  if (ENABLED_ANALOG_AXIS_NUM > 0) {
    //Calculated how many Analog CAN frames required (1 or 2)
    analogFrames = (((ENABLED_ANALOG_AXIS_NUM + 4) - 1) / 4);

    //Load array of enabled Analog Axis Pins
    for (int i = 0; i < ENABLED_ANALOG_AXIS_NUM; i++) {
      analogPins[i] = OpenFFB_Analog_Input_Pins[i];
      pinMode(analogPins[i], INPUT_ANALOG);
    }
    //Analog Input Resolution of ADC (Default:12-bit)
    analogReadResolution(STM32_ADC_RESOLUTION);
  }


  /* Timing */

  //TIme between Polls in ms = (1/120Hz * 1000ms/sec) / FrameCount

  CAN_Frame_interval = (((1 / POLLING_FREQUENCY) * 1000) / (FrameCount));
  //const long HIDpollingDelayInMillis = (((1 / POLLING_FREQUENCY) * 1000)/CAN_Frame_interval;  //HID Polling delay in milliseconds
}


//=====================
//        LOOP        |
//=====================
void loop() {

  /*Input Polling Timer*/
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= CAN_Frame_interval) {
    previousMillis = currentMillis;  //save last time polled.






    //For which CAN Frame to send (0:Digital 1:64[100], 1:Analog 1:4[110], 2:Analog 5:6[111])
    switch (FrameIndex) {
      /* SEND DIGITAL BUTTON DATA*/
      case 0:
        {
          if (DIGITAL_INPUTS_ENABLED) {
            CAN_message_t CAN_Msg;
            bool status;
            CAN_Msg.len = 0;
            CAN_Msg.id = (CAN_DIGITAL_ID);
          }


          break;
        }  //END CASE 0
      /* SEND ANALOG AXIS 1:4 DATA*/
      case 1:  // your hand is on the sensor
        {
         if (ANALOG_INPUTS_ENABLED) {
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

  //Frame Index increment
  if ()
}




//===================
// Helper Functions:
//===================

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
