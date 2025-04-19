#include <Arduino.h>
#include "OpenFFBoard_Pinouts.h"    //https://github.com/Ultrawipf/OpenFFBoard/wiki/Pinouts-and-peripherals)
#include "Config.h"                 //Settings
#include "src/lib/ShiftIn.h"        //https://github.com/InfectedBytes/ArduinoShiftIn
#include "src/lib/STM32_CAN.h"      //https://github.com/pazi88/STM32_CAN
#include "src/lib/Error_Handler.h"  //Debuggingt
//#include "src/Encoders/Encoder.h"       //https://chome.nerpa.tech/mcu/reading-rotary-encoder-on-arduino/
#include "src/Encoders/STM32HWEncoder.h"  // https://github.com/simplefoc/Arduino-FOC-drivers/tree/master/src/encoders/stm32hwencoder
#include "FastLED.h"                      //Forked Repo to include STM32F407 support https://github.com/Sqwince/FastLED

//Custom classes to replicate SimHub RGBLED strip effects (Sqwince)
#include "src\SimHub\RPMsEffect.h"
#include "src\SimHub\BlinkEffect.h"
#include "src\SimHub\ScrollEffect.h"

//(OpenFFBoard-main v1.2.4, Pins:CAN_RX=PD0 | CAN_TX=PD1, RX Buffer size = 8MB)
STM32_CAN Can(CAN1, ALT_2, RX_SIZE_8, TX_SIZE_8);        //Use PD0/1 pins for CAN1 with RX/TX buffer 8MB
ShiftIn<(SPI_BUTTON_BOARDS * 4)> shift;                  //Init SPI ShiftIn instance with 4x 74HC165 = 32 inputs
ErrorHandler error_handler(LED_RED_PIN, DEBUG_ENABLED);  //Init Error handler (Serial Debugging & onboard RED LED)

//Rotary Knob Encoders
//Supports up to 4 encoders (See OpenFFBoard_Pinouts.h)
// Encoder encoder1(ENC1_A_PIN, ENC1_B_PIN, ENCODER_PULSE_MS);
// Encoder encoder2(ENC2_A_PIN, ENC2_B_PIN, ENCODER_PULSE_MS);
STM32HWEncoder encoder1 = STM32HWEncoder(ENCODER_PPR, ENCODER_PULSE_MS, ENC1_A_PIN, ENC1_B_PIN);
STM32HWEncoder encoder2 = STM32HWEncoder(ENCODER_PPR, ENCODER_PULSE_MS, ENC2_A_PIN, ENC2_B_PIN);

/*#############################################################################*/
/*############      SimHub RGBLED Strip Effect Replication      ###############*/
/*      Sorry no fancy GUI. See: SimHub_Effects_Readme.h for other examples */
/*#############################################################################*/
CRGB leds[NUM_LEDS];  //Represents the LED strip as array of CRGB colors FastLED library

/* Setup Simhub RGBLed Effects */
// Map RPMs (0-100%) Left to right from LED#3 to #6 with a gradiant from Green to Red, with Revlimiter Red/Blue
// Map RPMs (0-100%) right to left from LED#9 to #14 with a gradiant from Green to Red, with Revlimiter Red/Blue
// RPMsEffect RPMs_Left(leds, false, 3, 6, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Red, CRGB::Blue);
// RPMsEffect RPMs_Right(leds, true, 9, 6, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Blue, CRGB::Red);

//Used for flags & spotter flags
// BlinkEffect yellowFlag1(leds, BlinkEffect::flashing, 0, 3, CRGB::Yellow, CRGB::Black);
// BlinkEffect yellowFlag2(leds, BlinkEffect::flashing, 15, 3, CRGB::Yellow, CRGB::Black);

//Just RPM no flags yet:
RPMsEffect RPMs_Left(leds, false, 0, 9, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Red, CRGB::Black);
RPMsEffect RPMs_Right(leds, true, 9, 9, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Black, CRGB::Red);

//TODO: Remove this, used LED Testing
#define POT_PIN AIN_1_PIN     //Potentiometer input to represent RPMs
#define BUTTON_PIN DIN_7_PIN  //Push Button input for testing flags

static CAN_message_t CAN_RX_msg;  //Incommming CAN msg from Simhub
uint8_t RPM_percent = 0;          //Simhub reported RPM% (0-100)


/*###############################################*/
/*#######              SETUP              #######*/
/*###############################################*/
void setup() {

  //Configure OpenFFB Onboard LED pins
  pinMode(LED_BLU_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);  //also established in the error handler
  pinMode(LED_YEL_PIN, OUTPUT);


  /* Initialize RGB LED Strip using FastLED Library */
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);  //set maximum brightness for the LEDs
  FastLED.clear();                        //all to black
  FastLED.show();                         //refresh strip
  //[OPTIONAL]Sets blinking rate for each effect
  RPMs_Left.setBlinkDelay(100);   //RevLimiter Blink rate
  RPMs_Right.setBlinkDelay(100);  //RevLimiter Blink Rate
  // yellowFlag1.setBlinkDelay(250);  //Flag Blink Rate
  // yellowFlag2.setBlinkDelay(250);  //Flag Blink Rate


/* Initialize serial communication for debugging */
#if (DEBUG_ENABLED == true)
  Serial.begin(115200);
  delay(2000);
  Serial.println("Serial initialized Successfully!");
#endif



/* Initialize CAN bus */
#if (DEBUG_ENABLED == true)
  Serial.println("Initializing CAN...");
#endif
  Can.begin();
  Can.setBaudRate(CAN_SPEED);
#if (DEBUG_ENABLED == true)
  delay(2000);
  Serial.println("CAN initialized Successfully!");
#endif



  /* INIIALIZE DIGITAL INPUTS */
  if ((SPI_BUTTON_BOARDS > 0) || (ROTARY_ENCODER_COUNT > 0)) {
    DIGITAL_INPUTS_ENABLED = true;
    //Initalize SPI port: pLoadPin, clockEnablePin, dataPin, clockPin
    shift.begin(SPI2_CS1_PIN, SPI2_MISO_PIN, SPI2_SCK_PIN);
    FrameCount++;  //Single frame is used for up to 64 digital Buttons
  }


  /* INITIALIZE ROTARY KNOB ENCODERS*/
#if (DEBUG_ENABLED == true)
  Serial.println("Initializing Encoder...");
#endif

  // encoder1.begin(); //old lib - DELETE
  // encoder2.begin(); //old lib - DELETE
  encoder1.init();
  encoder2.init();

#if (DEBUG_ENABLED == true)
  if ((encoder1.initialized == false) || (encoder2.initialized == false)) {
    error_handler.handleError(errorCode_ENC_Init_Fail);
  } else {
    delay(1000);
    Serial.println("Encoder initialized Successfully!");
  }
#endif



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

  /* Calculate Timing Intervals */
  //Time between interval in ms = (1/__Hz * 1000ms/sec) / (1,2,3)
  CAN_Frame_interval = (((1 / POLLING_FREQUENCY) * 1000) / (FrameCount));
  LED_Refresh_interval = ((1 / LED_REFRESH_RATE) * 1000);  //Convert LED Refresh rate (Hz) to milliseconds
}



/*###############################################*/
/*#######               LOOP              #######*/
/*###############################################*/
void loop() {
  unsigned long currentMillis = millis();


  //update encoder states
  // encoder1.update();
  // encoder2.update();
  // encoder3.update();
  // encoder4.update();


  //Timer for SimHub MSG Handling / LED Drawing:
  if (currentMillis - LED_previousMillis >= LED_Refresh_interval) {
    LED_previousMillis = currentMillis;  //save last time polled.

    //Check for SimHub MSG:
    if (Can.read(CAN_RX_msg)) {

      RPM_percent = CAN_RX_msg.buf[0];  //first byte = rpm value 0-100%

#if ((DEBUG_ENABLED == true) && (DEBUG_CANRX_MSGS == true))
      Serial.print("<< CAN ID: ");
      Serial.print(CAN_RX_msg.id);
      Serial.print(" | Len: ");
      Serial.print(CAN_RX_msg.len);
      Serial.print(" | Data: ");
      for (int i = 0; i < CAN_RX_msg.len; i++) {
        Serial.print(CAN_RX_msg.buf[i], HEX);
        Serial.print("|");
      }
      //Serial.println();
      Serial.print(" Val: ");
      Serial.println(RPM_percent);
#endif
    }

    RPMs_Left.update(RPM_percent);
    RPMs_Right.update(RPM_percent);
    //yellowFlag1.update(!state);
    //yellowFlag2.update(!state);
  }


  //BUG: Seems to require the show outide of the timers to draw every loop cycle?
  //Draw LEDS
  FastLED.show();


  /*Input HID Polling Timer*/
  if (currentMillis - previousMillis >= CAN_Frame_interval) {
    previousMillis = currentMillis;  //save last time polled.

    //Old arduino interrupt based encoder lib - DELETE
    // uint8_t enc1_dir = encoder1.getState();
    // uint8_t enc2_dir = encoder2.getState();
    // uint8_t enc3_dir = encoder3.getState();
    // uint8_t enc4_dir = encoder4.getState();

    //Next Frame
    FrameIndex = (FrameIndex + 1) % FrameCount;  //Cycles through 0, 1, 2 (depending on how many frames needed)

    CAN_message_t CAN_Msg;
    bool status;

    //For which CAN Frame to send (0:Digital 1:64[100], 1:Analog 1:4[110], 2:Analog 5:6[111])
    switch (FrameIndex) {
        /*#######################################################################*/
      case digitalInputs:  // READ DIGITAL BUTTON DATA
        {
          if (DIGITAL_INPUTS_ENABLED) {
            // CANID: 100
            // Each bit = 1 digital I/O
            // 32 of 64 possible mapped below in one CAN Frame:
            //    |11111111|11111111|11111111|11111111|
            //    |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|

            CAN_Msg.len = 0;
            CAN_Msg.id = (CANID_DIGITAL);

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
          // Frame ID: 110 -> |axis1[0:7], axis1[8:15]||axis2[0:7], axis2[8:15]||axis3[0:7], axis3[8:15]||axis4[0:7], axis4[8:15]||
          CAN_Msg.len = 0;
          CAN_Msg.id = (CANID_AXIS14);

          //Load ANALOG Inputs AXIS1:4 into CAN Buffer
          for (int i = 0; i < 4; i++) {
            if (i < ENABLED_ANALOG_AXIS_NUM) {
              status = Append_s16(&CAN_Msg, readAnalogInput(analogPins[i], ANALOG_DEADZONE_LOWER, ANALOG_DEADZONE_UPPER));
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
      case AnalogAxis56:  // READ ANALOG AXIS 5:6 DATA
        {
          // Frame ID: 111 -> |axis5[0:7], axis5[8:15]||axis6[0:7], axis6[8:15]||xx||xx||xx||xx|
          CAN_Msg.len = 0;
          CAN_Msg.id = (CANID_AXIS56);
          //Load ANALOG Inputs AXIS5:6 into CAN Buffer
          for (int i = 4; i < 8; i++) {

            if (i < ENABLED_ANALOG_AXIS_NUM) {
              status = Append_s16(&CAN_Msg, readAnalogInput(analogPins[i], 5.0, 95.0));
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
    sendCANmsg(&CAN_Msg);  //Send the input statuses over CAN
  }                        //polling timer
}  //loop

/*###############################################*/
/*#######          HELPER FUNCTIONS       #######*/
/*###############################################*/
void sendCANmsg(CAN_message_t* msg) {
  //You guys silly? I'm Just gonna send it!!! (https://www.youtube.com/watch?v=RSuLFvalhnQ)
  bool CAN_status = Can.write(*msg);

  if (!CAN_status) {
    error_handler.handleError(errorCode_CAN_Send_Fail);
  }

  error_handler.clearError();

#if ((DEBUG_ENABLED == true) && (DEBUG_CANTX_MSGS == true))
  Serial.print("CAN ID: ");
  Serial.print(msg->id);
  Serial.print(" | Data: |");
  for (int i = 0; i < msg->len; i++) {
    //Serial.print(CAN_Msg.buf[i], HEX);
    Serial.printf("%02X", msg->buf[i]);  //zero padded two places
    Serial.print("|");
  }
  Serial.println("");
#endif
}


/* Insert s16b value into CAN Msg Buffer */
bool Append_s16(CAN_message_t* msg, int16_t val) {
  size_t start_byte = msg->len;
  if ((start_byte + 2) > 8) return false;        // can't add more data, message is full
  msg->buf[start_byte] = val & 0xFF;             // Low byte of val
  msg->buf[start_byte + 1] = (val >> 8) & 0xFF;  // High byte of val
  msg->len += 2;
  return true;
}

/*Read Analog Axis and re-map values for HID*/
int16_t readAnalogInput(uint32_t ainPin, float lowerDeadzonePercent, float upperDeadzonePercent) {
  int16_t val = (int16_t)analogRead(ainPin);

  // Convert percentage deadzones to raw 12b values
  uint16_t lowerDeadzone = (uint16_t)(lowerDeadzonePercent * 4095.0 / 100.0);
  uint16_t upperDeadzone = (uint16_t)(upperDeadzonePercent * 4095.0 / 100.0);

  //LOWER analog deadzone
  if (val <= lowerDeadzone) {
    return -32767;
  } else if (val >= upperDeadzone) {
    return 32767;
  } else {

    /* Remap from [lowerDeadzone, upperDeadzone] to [-32767, 32767]
       12bit uint (0 to 4095) MAP TO 16bit signed (-32767 to +32767) */
    int32_t mapped_val = (int32_t)(val - lowerDeadzone) * 65534 / (upperDeadzone - lowerDeadzone) - 32767;
    return (int16_t)mapped_val;

    /* mapped_val = (input_value − in_min) * (out_max − out_min / in_max − in_min) + out_min
              val = ((int32_t)val - in_min) * ((out_max - out_min) / (in_max - in_min)) + out_min;
    in_min = 0
    in_max = 4095
    out_min = -32767
    out_max = 32767  
  12bit uint (0 to 4095) MAP TO 16bit signed (-32767 to +32767)*/
    // val = ((int32_t)val * 65534 / 4095) - 32767;
    // return val;
  }
}

/*Insert Button values into the CAN Msg Buffer*/
template<uint8_t N>
bool Append_BTN_States(CAN_message_t* _msg, ShiftIn<N>* _shift) {
  uint64_t val = _shift->read();  // Read button values as 64-bit integer

  //unsigned long currentMillis = millis();

  uint8_t enc1_dir = encoder1.Update();
  uint8_t enc2_dir = encoder2.Update();

  /* ENCODERS */
  //Include Rotary Knob Encoder states
  // Left shift 2 and insert Rotary Encoder States (2 bits each)
  // val = (val << 2) | encoder4.Update();
  // val = (val << 2) | encoder3.Update();
  // val = (val << 2) | encoder2.Update();
  // val = (val << 2) | encoder1.Update();


//encoder debugging
#if ((DEBUG_ENABLED == true) && (DEBUG_ENCODERS == true))
  //Debugging - Encoder Count Serial Reporting
  Serial.print("Encoder 1|2: ");
  Serial.print(enc1_dir, DEC);
  Serial.print("|");
  Serial.print(enc2_dir, DEC);
  Serial.println("|");
#endif


  // Check to ensure the message buffer is empty and can fit 8 bytes
  if (_msg->len != 0) { return false; }

  for (int i = 0; i < 8; i++) {
    _msg->buf[i] = val & 0xFF;  // Extract the lowest byte
    val >>= 8;                  // Shift val right by 8 bits
    _msg->len++;                // Increment the length for each byte added
  }
  return true;
}
