#include "Error_Handler.h"

/*###############################################*/
/*#######          ERROR HANDLING         #######*/
/*###############################################*/

// Constructor
ErrorHandler::ErrorHandler(int ledPin, bool debugEnabled) {
  _ledPin = ledPin;
  _debugEnabled = debugEnabled;
  pinMode(_ledPin, OUTPUT);  // Set LED pin as output
}

// Clear the error LED
void ErrorHandler::clearError() {
  digitalWrite(_ledPin, LOW);
}

// Handle errors and optionally print to serial
void ErrorHandler::handleError(ERROR_Code errorCode) {
  // Blink the error LED (error indication)
  digitalWrite(_ledPin, HIGH);

  // Serial debugging
  if (_debugEnabled) {
    switch (errorCode) {
      case errorCode_Index_Fault:
        Serial.println("ERROR: INPUT number out of bounds.");
        break;

      case errorCode_CAN_Buf_Overflow:
        Serial.println("ERROR: CAN Buffer Overflow!!");
        break;

      case errorCode_CAN_Send_Fail:
        Serial.println("ERROR: Sending CAN frame failed.");
        break;

        case errorCode_ENC_Init_Fail:
        Serial.println("ERROR: Encoder initialization failed.");
        break;
    }
  }
}
