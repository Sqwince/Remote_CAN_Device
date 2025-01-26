/*OpenFFboard External CAN Input monitor
======================================
Description: Reads Digital inputs from SPI-Button Board
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
*/

#include <Arduino.h>
#include "ShiftIn.h"
#include "STM32_CAN.h"

/* OpenFFBoard SETUP Parameters */
#define POLLING_FREQUENCY 100  // HID polling frequency [Default: 1kHz]
#define SPI_BUTTON_BOARDS 1  // Supports 32 inputs
//#define ENABLED_DIGITAL_INPUTS_NUM 32  // SPI Buttons Board = 32
#define CAN_SPEED 500000    // 500 kbps
#define CAN_DIGITAL_ID 100  // CAN Frame ID for Analog Input Data [Default: 110]
#define CAN_TXMSG_SIZE 8    // 8 Bytes

/*#### SPI2 Interface definitions #### */
#define SPI2_SCK_PIN PB13   //Clock
#define SPI2_MISO_PIN PB14  //Data In from last shift register in chain
#define SPI2_CS_PIN PB12    //Data Latch


unsigned long previousMillis = 0;                                       //last time HID was polled
const long HIDpollingDelayInMillis = ((1 / POLLING_FREQUENCY) * 1000);  //HID Polling delay in milliseconds

// Init SPI ShiftIn instance with 4x 74HC165 = 32 inputs
//https://github.com/InfectedBytes/ArduinoShiftIn
ShiftIn<4 * SPI_BUTTON_BOARDS> shift;

//(OpenFFBoard-main v1.2.4, Pins:CAN_RX=PD0 | CAN_TX=PD1, RX Buffer size = 8MB)
STM32_CAN Can(CAN1, ALT_2, RX_SIZE_8, TX_SIZE_8);  //Use PD0/1 pins for CAN1 with RX/TX buffer 8MB


//===================
// Helper Functions:
//===================

//Insert value into CAN Msg Buffer
//|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|00000000|00000000|00000000|00000001
bool Append_BTN_States(CAN_message_t* msg) {
  uint64_t val = shift.read();  // Read button values as 64-bit integer

  Serial.print("Val:");
  Serial.println(val, HEX);

  // Check to ensure the message buffer is empty and can fit 8 bytes
  if (msg->len != 0) {
    return false;
  }

  for (int i = 0; i < 8; i++) {
    msg->buf[i] = val & 0xFF;  // Extract the lowest byte
    val >>= 8;                 // Shift val right by 8 bits
    msg->len++;                // Increment the length for each byte added
  }
  return true;
}



//========================
//         SETUP         |
//========================
void setup() {
  //startup delay
  delay(1000);

  //Initialize Serial port for debugging
  Serial.begin(115200);
  Serial.println(">>Serial initialized.");

  //Initalize SPI port: pLoadPin, clockEnablePin, dataPin, clockPin
  shift.begin(SPI2_CS_PIN, SPI2_MISO_PIN, SPI2_SCK_PIN);
  Serial.println(">>SPI Port initialized.");
  delay(1000);

  // Initialize CAN bus
  Can.begin();
  Can.setBaudRate(CAN_SPEED);
  Serial.println(">>CAN Bus initialized.");
}


//=====================
//        LOOP        |
//=====================
void loop() {
  /*HID Polling Timer*/
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= HIDpollingDelayInMillis) {
    previousMillis = currentMillis;  //save last time polled.

    CAN_message_t CAN_Msg;
    bool status;
    CAN_Msg.len = 0;
    CAN_Msg.id = (CAN_DIGITAL_ID);  //100

    Append_BTN_States(&CAN_Msg);

    //Are you guys Silly? I'm Still Gonna Send it!! (https://www.youtube.com/watch?v=RSuLFvalhnQ)
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
  }
}
