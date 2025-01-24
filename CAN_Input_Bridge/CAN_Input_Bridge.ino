#include <STM32_CAN.h>

/* OpenFFBoard SETUP Parameters */
#define POLLING_FREQUENCY 10       //HID polling frequency [Default: 1kHz]
#define STM32_ADC_RESOLUTION 12    //Analog Input ADC Resolution (Default: 12 , STM32 ADC = 12-bit(0 to 4095)
#define ENABLED_ANALOG_AXIS_NUM 6  //Number of analog inputs used (Range: 1-6)
#define CAN_SPEED 500000   // 500 kbps
#define CAN_ANALOG_ID 110  // CAN Frame ID for Analog Input Data [Default: 110]
#define CAN_TXMSG_SIZE 8   // 8 Bytes

/*TODO: Replace pin mapping with HAL?
/* ANALOG INPUT PINS (Ref:https://github.com/Ultrawipf/OpenFFBoard/wiki/Pinouts-and-peripherals) */
uint32_t OpenFFB_Analog_Input_Pins[6] = {
  PA3,  //OpenFFB Analog Input 1
  PA2,  //OpenFFB Analog Input 2
  PC3,  //OpenFFB Analog Input 3
  PC2,  //OpenFFB Analog Input 4
  PC1,  //OpenFFB Analog Input 5
  PC0
};

//array of pin#s for enabled analog axis
uint32_t analogPins[ENABLED_ANALOG_AXIS_NUM];
unsigned long previousMillis = 0;                                       //last time HID was polled
const long HIDpollingDelayInMillis = ((1 / POLLING_FREQUENCY) * 1000);  //HID Polling delay in milliseconds

//(OpenFFBoard-main v1.2.4, Pins:CAN_RX=PD0 | CAN_TX=PD1, RX Buffer size = 8MB)
STM32_CAN Can(CAN1, ALT_2, RX_SIZE_8, TX_SIZE_8);  //Use PD0/1 pins for CAN1 with RX/TX buffer 8MB

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
  val = map(val, 0, 4095, -32767, 32767);  //12bit unsigned (0 to 4095) TO 16bit signed (-32767 to +32767)
  return val;
}

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

  //Load array of enabled Analog Axis Pins
  if (ENABLED_ANALOG_AXIS_NUM > 0) {

    //fill analogPins array from ffboard mapping
    for (int i = 0; i < ENABLED_ANALOG_AXIS_NUM; i++) {
      analogPins[i] = OpenFFB_Analog_Input_Pins[i];
      pinMode(analogPins[i],INPUT);
    }
    analogReadResolution(STM32_ADC_RESOLUTION);  //Analog Input Resolution of ADC (Default:12-bit)
  }
}



void loop() {
  /*HID Polling Timer*/
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= HIDpollingDelayInMillis) {
    previousMillis = currentMillis;  //save last time polled.
    
    //int16_t analogAxisValues[ENABLED_ANALOG_AXIS_NUM];
    //for (int i=0; i<ENABLED_ANALOG_AXIS_NUM)




    //Message Counter
    for (int j = 0; j < 2; j++) {

      CAN_message_t CAN_Msg;
      bool status;
      CAN_Msg.len = 0;
      CAN_Msg.id = (CAN_ANALOG_ID + j);  //110

      //Add Axis to the CAN buffer (4 / message)
      for (int i = 0; i < 4; i++) {

        if ((4 * j) + i <= ENABLED_ANALOG_AXIS_NUM) {
          status = Append_s16(&CAN_Msg, readAnalogInput(analogPins[(4 * j) + i]));
        }
        else {
            size_t start_byte = CAN_Msg.len;
             CAN_Msg.buf[start_byte] = 0;
             CAN_Msg.len+= 2;
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
