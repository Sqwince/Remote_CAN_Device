/* SIMHUB CUSTOM SERIAL DEVICE PROTOCOL
 * Up to 16x WS2812 RGBLEDS (+5VDC, DataPin, GND)
 * Ext Input Controller SimHub RX CAN ID: 120
 * LED Mask for effect 16b value 
 * IE: 10LEDs ALL RED: 
                                                            |  FUTURE   |
   |          LED MASK BIN(HEX)            |  R  |  G  | B  | ? | ? | ? |
   | 0000(0) | 0011(3) | 1111(F) | 1111(F) |  FF |  00 | 00 | ? | ? | ? |

 * IE: First 3LEDs Yellow: 
                                                            |  FUTURE   |
   |          LED MASK BIN(HEX)            |  R  |  G  | B  | ? | ? | ? |
   | 0000(0) | 0011(3) | 1100(C) | 0000(0) |  FF |  FF | 00 | ? | ? | ? |








#include <Arduino.h>
#include "STM32_CAN.h"  //https://github.com/pazi88/STM32_CAN

#define CAN_SIMHUB_ID 120  //SimHub Frame:   ||



//(OpenFFBoard-main v1.2.4, Pins:CAN_RX=PD0 | CAN_TX=PD1, RX Buffer size = 8MB)
STM32_CAN Can(CAN1, ALT_2, RX_SIZE_8, TX_SIZE_8);      //Use PD0/1 pins for CAN1 with RX/TX buffer 8MB













// Read data from CAN bus and print out the messages to serial bus. 
// Note that only message ID's that pass filters are read.
void readCanMessage()  
{
  Serial.print("Channel:");
  Serial.print(CAN_inMsg.bus);
  if (CAN_inMsg.flags.extended == false) {
    Serial.print(" Standard ID:");
  }
  else {
    Serial.print(" Extended ID:");
  }
  Serial.print(CAN_inMsg.id, HEX);

  Serial.print(" DLC: ");
  Serial.print(CAN_inMsg.len);
  if (CAN_inMsg.flags.remote == false) {
     Serial.print(" buf: ");
    for(int i=0; i<CAN_inMsg.len; i++) {
      Serial.print("0x"); 
      Serial.print(CAN_inMsg.buf[i], HEX); 
      if (i != (CAN_inMsg.len-1))  Serial.print(" ");
    }
    Serial.println();
  } else {
     Serial.println(" Data: REMOTE REQUEST FRAME");
  }
}