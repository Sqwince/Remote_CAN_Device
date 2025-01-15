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

#include "STM32_CAN.h"

//TODO: inherit SPI2 buttons to report

/* OpenFFBoard SETUP Parameters */
#define POLLING_FREQUENCY 1000   //HID polling frequency [Default: 1kHz]
#define STM32_ADC_RESOLUTION 12  //Analog Input ADC Resolution (Default: 12 , STM32 ADC = 12-bit(0 to 4095)

/* CAN RX/TX PINS - uncomment selection and pick based on your board*/
//STM32_CAN Can( CAN1, DEF );  //Use PA11/12 pins for CAN1.
//STM32_CAN Can( CAN1, ALT );  //Use PB8/9 pins for CAN1.
STM32_CAN Can(CAN1, ALT_2);  //Use PD0/1 pins for CAN1 (OpenFFBoard-main v1.2.4 CAN_RX=PD0 | CAN_TX=PD1) 
//STM32_CAN Can( CAN2, DEF );  //Use PB12/13 pins for CAN2.
//STM32_CAN Can( CAN2, ALT );  //Use PB5/6 pins for CAN2
//STM32_CAN Can( CAN3, DEF );  //Use PA8/15 pins for CAN3.
//STM32_CAN Can( CAN3, ALT );  //Use PB3/4 pins for CAN3

#define CAN_SPEED 500000         // 500 kbps
#define CAN_ANALOG_ID 110        //CAN Frame ID for Analog Input Data [Default: 110]
#define CAN_DIGITAL_ID 100        //CAN Frame ID for Analog Input Data [Default: 100]

// Define the OpenFFBoard (STM32407VGT6) onboard DIGITAL input pins
#define DIGITAL_PIN_1 PC15
#define DIGITAL_PIN_2 PC14
#define DIGITAL_PIN_3 PC13
#define DIGITAL_PIN_4 PE6
#define DIGITAL_PIN_5 PE5
#define DIGITAL_PIN_6 PE4
#define DIGITAL_PIN_7 PE3
#define DIGITAL_PIN_8 PE2

/*OpenFFBoard ANALOG INPUTS:
Up to 6 analog pins are mapped to gamepad axes with 12 bit ADC resolution.
FFBoard pin	| STM pin
    A0 (1)	| PA3
    A1 (2)	| PA2
    A2 (3)	| PC3
    A3 (4)	| PC2
    A4 (5)	| PC1
    A5 (6)	| PC0
*/

#define ANALOG_PIN_1 PA3
#define ANALOG_PIN_2 PA2
#define ANALOG_PIN_3 PC3
#define ANALOG_PIN_4 PC2
#define ANALOG_PIN_5 PC1
#define ANALOG_PIN_6 PC0






//===================
// Helper Functions:
//===================

//Analog Reading and data manipulation for HID analog inputs
int16_t analogReadInput(uint32_t ainPin) {
  int16_t val = (int16_t)analogRead(ainPin);
  Serial.print("RAW Val: ");
  Serial.print(val);
  val = map(val, 0, 4095, -32767, 32767);  // 12bit unsigned (0 to 4095) TO 16bit signed (-32767 to +32767)
  Serial.print(" | Mapped Val: ");
  Serial.println(val);
  return val;
}

//Insert value into CAN Msg Buffer
bool Append_s16(CAN_message_t *msg, int16_t val) {
  size_t start_byte = msg->len;
  if ((start_byte + 2) > 8) return false;        // can't add more data, message is full
  msg->buf[start_byte] = val & 0xFF;             // Low byte of val
  msg->buf[start_byte + 1] = (val >> 8) & 0xFF;  // High byte of val
  msg->len += 2;
  return true;
}

unsigned long previousMillis = 0;       //last time HID was polled
const long HIDpollingDelayInMillis = ((1/POLLING_FREQUENCY)*1000);  //HID Polling delay in milliseconds 



//========================
//         SETUP         |
//========================
void setup() {
  //OpenFFBoard Input Parameters
  analogReadResolution(STM32_ADC_RESOLUTION);  //Analog Input Resolution of ADC (Default:12-bit)

  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Initialize CAN bus
  Can.begin();
  Can.setBaudRate(CAN_SPEED);
  Serial.println("CAN initialized.");
}


void loop() {

/*HID Polling Timer*/
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= HIDpollingDelayInMillis) {
    previousMillis = currentMillis; //save last time polled.

    CAN_message_t CAN_TX_msg;
    CAN_TX_msg.id = (CAN_ANALOG_ID);  //Analog CAN frame id
    CAN_TX_msg.len = 0;               //incremented as data is added to buffer

    bool status;
    status = Append_s16(&CAN_TX_msg, analogReadInput(ANALOG_PIN_1));
    status = Append_s16(&CAN_TX_msg, analogReadInput(ANALOG_PIN_2));
    status = Append_s16(&CAN_TX_msg, analogReadInput(ANALOG_PIN_3));
    status = Append_s16(&CAN_TX_msg, analogReadInput(ANALOG_PIN_4));

    if (!status) {  // check to make sure buffer didn't overflow
      Serial.println("ERROR: CAN Buffer Overflow!!");
    }

    // Transmit Analog Inputs CAN frame
    if (Can.write(CAN_TX_msg)) {
      Serial.println("CAN frame sent successfully!");
      Serial.print("Msg.len: ");
      Serial.print(CAN_TX_msg.len);
      Serial.print(" | Sent CAN.buf: ");
      for (size_t i = 0; i < CAN_TX_msg.len; i++) {
        Serial.print(CAN_TX_msg.buf[i], HEX);
        Serial.print("|");
      }
      Serial.println("");
    } else {
      Serial.println("Error sending CAN frame.");
    }
  }
}