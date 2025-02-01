#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <Arduino.h>


//Errors
enum ERROR_Code {
  errorCode_Index_Fault,       //"ERROR: INPUT number out of bounds."
  errorCode_CAN_Buf_Overflow,  //"ERROR: CAN Buffer Overflow!!"
  errorCode_CAN_Send_Fail,     //"ERROR: Sending CAN frame failed."
};


class ErrorHandler {
  public:
    // Constructor to initialize LED pin and serial debug flag
    ErrorHandler(int ledPin, bool debugEnabled = false);

    // Clear the error LED
    void clearError();

    // Handle errors and optionally print to serial
    void handleError(ERROR_Code errorCode);

  private:
    int _ledPin;               // Pin connected to the LED
    bool _debugEnabled;        // Flag to enable/disable serial debugging
};

#endif //ERROR_HANDLER_H