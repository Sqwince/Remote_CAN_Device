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
uint32_t OpenFFB_Analog_Input_Pins[6] = { AIN_1_PIN, AIN_2_PIN, AIN_3_PIN, AIN_4_PIN, AIN_5_PIN, AIN_6_PIN };



/*#########################################*/
/*#######       CAN SETTINGS        #######*/
/*#########################################*/

//CAN IDs to TRANSMIT Inputs to HID:
#define CANID_DIGITAL 100                // CAN Frame ID for Digital Input Data [Default: 100]
#define CANID_AXIS14 110                 //CAN Frame ID for Analog Input Data Axis1:4 [Default: 110]
#define CANID_AXIS56 (CANID_AXIS14 + 1)  //CAN Frame ID for Analog Input Data Axis5:6 [Default: 111]

//CAN IDs to RECEIVE Game Data from Simhub:
#define CANID_SIMHUB_RPM 200  //CAN ID of RPM Value received from SimHub custom serial protocol


/*#########################################*/
/*#######      INPUT SETTINGS       #######*/
/*#########################################*/
// HID expects signed integer range (MIN:-32767[0x7fff],CENTER:0[0x0000], MAX:+32767[0x8001])
#define ENABLED_ANALOG_AXIS_NUM 4              //Number of analog inputs used (Range: 1-6)
#define STM32_ADC_RESOLUTION 12                //Analog Input ADC Resolution (Default: 12 , STM32 ADC = 12-bit(0 to 4095)
uint32_t analogPins[ENABLED_ANALOG_AXIS_NUM];  //array of pin#s for enabled analog axis
#define SPI_BUTTON_BOARDS 1                    //Number of SPI-Button boards used (0-2)

/*  ROTARY ENCODER KNOB SETTINGS
    A/B encoder phases attached to interrupts
    converted into a pulse lasting #ms for HID to read. 
    Encoder Update Returns: -1 for CCW direction 
    Encoder Update Returns: +1 for CW direction
    Encoder Update Returns:  0 once pulse has timed out*/
#define ENCODER_PPR 20  //20ppr / 40cpr - KY-040 Style encoder    
#define ROTARY_ENCODER_COUNT 2  //Number of Rotary Encoder knobs used (Supports up to 4)
#define ENCODER_PULSE_MS 50     //Encoder Pulse length in milliseconds (default = 50ms)


/*########################################################*/
/*#######   General Settings & Timing Parameters   #######*/
/*########################################################*/
#define POLLING_FREQUENCY 120   //Input polling frequency [Default: 120Hz]
#define CAN_SPEED 500000        //500 kbps for OpenFFBoard
#define CAN_TXMSG_SIZE 8        //8 Bytes
#define DEBUG_ENABLED true      //serial print Debug msgs
#define DEBUG_CANTX_MSGS false  //Serial print DATA sent over CAN
#define DEBUG_CANRX_MSGS false  //Serial print DATA sent over CAN

/* RGB LED Strip Settings */
#define NUM_LEDS 18          //Total count of LEDs for strip
#define LED_DATA_PIN SPI2_MOSI_PIN    //WS2812B Data Input
#define LED_REFRESH_RATE 20  //RefreshRate [30 FPS]
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/** CAUTION: Be mindful of LED current draw when setting MAX brightness **/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
#define MAX_BRIGHTNESS 20  //maximum brightness [0-255]


//Global Variables
uint16_t CAN_Frame_interval;           //Time in ms between sending CAN Frames
uint16_t LED_Refresh_interval;         //Time in ms between updating LEDs
uint16_t FrameCount;                   //Number of CAN Frames enabled
uint8_t FrameIndex;                    // Keeps track of the current frame
unsigned long previousMillis = 0;      //init last time inputs were read
unsigned long LED_previousMillis = 0;  //init Last time LEDs updated
bool DIGITAL_INPUTS_ENABLED;           //Set to true when SPI Button boards > 0


#endif  // CONFIG_H