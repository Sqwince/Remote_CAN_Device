#ifndef SIMHUB_MSG_HANDLER_H
#define SIMHUB_MSG_HANDLER_H

#include <Arduino.h>


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




#endif //ERROR_HANDLER_H