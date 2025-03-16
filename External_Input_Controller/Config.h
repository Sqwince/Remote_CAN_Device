#ifndef CONFIG_H
#define CONFIG_H



/*#########################################*/
/*#######           ENUMS           #######*/
/*#########################################*/
enum FrameIndexNumber {
  digitalInputs,
  AnalogAxis14,
  AnalogAxis56,
};

//TODO: Convert this to enum?
uint32_t OpenFFB_Analog_Input_Pins[6] = {
  PA3,  //OpenFFB Analog Input 1
  PA2,  //OpenFFB Analog Input 2
  PC3,  //OpenFFB Analog Input 3
  PC2,  //OpenFFB Analog Input 4
  PC1,  //OpenFFB Analog Input 5
  PC0   //OpenFFB Analog Input 6
};      //Ref:https://github.com/Ultrawipf/OpenFFBoard/wiki/Pinouts-and-peripherals)


//OpenFFBoard Digital Input Pins
#define DIN_1 PC15
#define DIN_2 PC14
#define DIN_3 PC13
#define DIN_4 PE6
#define DIN_5 PE5
#define DIN_6 PE4
#define DIN_7 PE3
#define DIN_8 PE2



/*#########################################*/
/*#######   ANALOG INPUT SETTINGS   #######*/
/*#########################################*/
// HID expects signed integer range (MIN:-32767[0x7fff],CENTER:0[0x0000], MAX:+32767[0x8001])
// Frame ID: 110 -> |axis1[0:7], axis1[8:15]||axis2[0:7], axis2[8:15]||axis3[0:7], axis3[8:15]||axis4[0:7], axis4[8:15]||
// Frame ID: 111 -> |axis5[0:7], axis5[8:15]||axis6[0:7], axis6[8:15]||xx||xx||xx||xx|

#define ENABLED_ANALOG_AXIS_NUM 6              //Number of analog inputs used (Range: 1-6)
#define CAN_AXIS14_ID 110                      //CAN Frame ID for Analog Input Data Axis1:4 [Default: 110]
#define CAN_AXIS56_ID (CAN_AXIS14_ID + 1)      //CAN Frame ID for Analog Input Data Axis5:6 [Default: 111]
#define STM32_ADC_RESOLUTION 12                //Analog Input ADC Resolution (Default: 12 , STM32 ADC = 12-bit(0 to 4095)
uint32_t analogPins[ENABLED_ANALOG_AXIS_NUM];  //array of pin#s for enabled analog axis


/*##########################################*/
/*#######   DIGITAL INPUT SETTINGS   #######*/
/*##########################################*/
// ID: 100
// Each bit = 1 digital I/O
// 32 of 64 possible mapped below in one CAN Frame:
//    |11111111|11111111|11111111|11111111|
//    |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|
#define CAN_DIGITAL_ID 100   // CAN Frame ID for Digital Input Data [Default: 100]
#define SPI_BUTTON_BOARDS 1  //Number of SPI-Button boards used (0-2)
bool DIGITAL_INPUTS_ENABLED = false;

//---  Openffbaord SPI2 Interface Pins Definition  ---
#define SPI2_SCK_PIN PB13   //Clock
#define SPI2_MISO_PIN PB14  //Data In from last shift register in chain
#define SPI2_CS_PIN PB12    //Data Latch

/*  ROTARY ENCODER KNOB SETTINGS
    A/B encoder phases attached to interrupts
    converted into a pulse lasting (default = 100ms) for HID to read. 
    Encoder Update Returns: -1 for CCW direction 
    Encoder Update Returns: +1 for CW direction
    Encoder Update Returns:  0 once pulse has timed out
*/
#define ROTARY_ENCODER_COUNT 2  //Number of Rotary Encoder knobs used (0-4)
#define ENCODER_PULSE_MS 50     //Encoder Pulse length in milliseconds

/*  ROTARY ENCODER KNOB #1 PINS */
#define ENC1_A_PIN DIN_4
#define ENC1_B_PIN DIN_2

/*  ROTARY ENCODER KNOB #2 PINS */
#define ENC2_A_PIN DIN_6
#define ENC2_B_PIN DIN_8

/*  ROTARY ENCODER KNOB #3 PINS */
// #define ENC3_A_PIN DIN_1
// #define ENC3_B_PIN DIN_3

/*  ROTARY ENCODER KNOB #4 PINS */
// #define ENC4_A_PIN DIN_5
// #define ENC4_B_PIN DIN_7


/*########################################################*/
/*#######   General Settings & Timing Parameters   #######*/
/*########################################################*/
#define POLLING_FREQUENCY 120  //Input polling frequency [Default: 120Hz]
#define CAN_SPEED 500000      //500 kbps for OpenFFBoard
#define CAN_TXMSG_SIZE 8      //8 Bytes
#define DEBUG_ENABLED true    //serial print debug msgs

//OpenFFB onboard LEDs
#define LED_BLU_Pin PD7  //Blue   (Sys)
#define LED_RED_Pin PE1  //Red    (Error)
#define LED_YEL_Pin PE0  //Yellow (Clip)

uint8_t CAN_Frame_interval = 8;    //Time in ms between sending CAN Frames
uint8_t FrameCount = 0;            //Number of CAN Frames enabled
uint8_t FrameIndex = 0;            // Keeps track of the current frame
unsigned long previousMillis = 0;  //last time inputs were read

#endif  // CONFIG_H