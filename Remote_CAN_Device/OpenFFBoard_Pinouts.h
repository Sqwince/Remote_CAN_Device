/*#############################################################*/
/*#######          OpenFFBoard Pin Definitions          #######*/
/*#############################################################*/
/*
OpenFFBoard WIKI:
https://github.com/Ultrawipf/OpenFFBoard/wiki/Pinouts-and-peripherals)

Alternate Function Pin mapping From here: 
https://github.com/stm32duino/Arduino_Core_STM32/blob/55e4695ce625d24758b96756b6e7dbda8d09b35e/variants/STM32F4xx/F407V(E-G)T_F417V(E-G)T/PeripheralPins.c#L117

STM32F407 Reference Manual:
https://www.st.com/resource/en/reference_manual/rm0090-stm32f405415-stm32f407417-stm32f427437-and-stm32f429439-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
*/

#if defined(STM32F407xx)
/*################################*/
/*#######   Onboard LEDs   #######*/
/*################################*/
#define LED_BLU_PIN PD7  //Blue   (Sys)
#define LED_RED_PIN PE1  //Red    (Error)
#define LED_YEL_PIN PE0  //Yellow (Clip)


/*########################################*/
/*#######   Digital Input Header   #######*/
/*########################################*/
#define DIN_1_PIN PC15
#define DIN_2_PIN PC14
#define DIN_3_PIN PC13
#define DIN_4_PIN PE6
#define DIN_5_PIN PE5
#define DIN_6_PIN PE4
#define DIN_7_PIN PE3
#define DIN_8_PIN PE2


/*########################################*/
/*#######   Analog Input Header   #######*/
/*########################################*/
#define AIN_1_PIN PA3
#define AIN_2_PIN PA2
#define AIN_3_PIN PC3
#define AIN_4_PIN PC2
#define AIN_5_PIN PC1
#define AIN_6_PIN PC0


/*##########################################*/
/*#######   SPI-2 Interface Header   #######*/
/*##########################################*/
#define SPI2_CS1_PIN PB12
#define SPI2_CS2_PIN PD8
#define SPI2_CS3_PIN PD9
#define SPI2_SCK_PIN PB13
#define SPI2_MISO_PIN PB14
#define SPI2_MOSI_PIN PB15


/*##############################*/
/*#######   EXT header   #######*/
/*##############################*/
#define E_STOP_PIN PD5    //E-STOP
#define GPIO1_PIN PD6     //GP1
#define GPIO2_PIN PB4     //GP2
#define GPIO3_PIN PB5     //GP3
#define GPIO4_PIN PB3     //GP4(SWO)
#define I2C_SCL_PIN PB8   //SCL
#define I2C_SDA_PIN PB9   //SDA
#define UART1_TX_PIN PB6  //TX
#define UART1_RX_PIN PB7  //RX

/*SPI-3 Interface (EXT) */
#define SPI3_CS1_PIN PA15   //CS1
#define SPI3_CS2_PIN PD2    //CS2
#define SPI3_CS3_PIN PD3    //CS3
#define SPI3_SCK_PIN PC10   //SCK
#define SPI3_MISO_PIN PC11  //MI3
#define SPI3_MOSI_PIN PC12  //MO3

//CAN interface (through U2 & not accessible)
// #define CAN_S_PIN PD4
// #define CAN_TX_PIN PD1
// #define CAN_RX_PIN PD0


/*##############################*/
/*#######   JP7 Header   #######*/
/*##############################*/
#define PWM1_PIN PE9
#define PWM2_PIN PE11
#define PWM3_PIN PE13
#define PWM4_PIN PE14

/*###############################################################*/
/*#######   JP8 Header (Motor Driver/TMC B2B connector)   #######*/
/*###############################################################*/
/*SPI-1 Interface (J8) */
#define SPI1_CS1_PIN PA4
#define SPI1_CS2_PIN PB0
#define SPI1_CS3_PIN PB1
#define SPI1_SCK_PIN PA5
#define SPI1_MISO_PIN PA6
#define SPI1_MOSI_PIN PA7
#define DRV_FLAG_PIN PE8
#define DRV_ENABLE_PIN PE7
#define DRV_BRAKE_CTRL_PIN PE10
#define DRV_GPIO_PIN PE12
#define DRV_VINT_PIN PC5
#define DRV_VEXT_PIN PC4

/*###############################*/
/*#######   JP11 Header   #######*/
/*###############################*/
#define GPIO5_PIN PA1
#define GPIO6_PIN PA0  //WAKEUP
#define GPIO7_PIN PB10
#define GPIO8_PIN PB11

/*##################################*/
/*#######   ENCODER Header   #######*/
/*##################################*/
#define ENCODER_A_PIN PC_6  //ENC Header / ENCODER A / Alternate Function AF1 = TIM3_CH1
#define ENCODER_B_PIN PC_7  //ENC Header / ENCODER B / Alternate Function AF1 = TIM3_CH2
#define ENCODER_C_PIN PD15


/*####################################################*/
/*#######       Alternate Pin Functions        #######*/
/*#######  <See reference manual info below>   #######*/
/*####################################################*/
/*---------------------------------------------------------------*/
/*>>>>>>>    ENCODER #1 - TIMER3 (16bit) - (Select One)   <<<<<<<*/
/*---------------------------------------------------------------*/
//Option 1:
//#define ENC1_A_PIN PC_6  //ENC Header / ENCODER A / Alternate Function AF1 = TIM3_CH1 
//#define ENC1_B_PIN PC_7  //ENC Header / ENCODER B / Alternate Function AF1 = TIM3_CH2 
//Option 2:
#define ENC1_A_PIN PB_4  //EXT Header /   GPIO2   / Alternate Function AF2 = TIM3_CH1  (Default)
#define ENC1_B_PIN PB_5  //EXT Header /   GPIO3   / Alternate Function AF2 = TIM3_CH2  (Default)
//Option 3:
// #define ENC1_A_PIN PA_6  //JP8 header / SPI1_MISO / Alternate Function AF2 = TIM3_CH1
// #define ENC1_B_PIN PA_7  //JP8 header / SPI1_MOSI / Alternate Function AF2 = TIM3_CH2

/*---------------------------------------------------------------*/
/*>>>>>>>    ENCODER #2 - TIMER4 (16bit) - (Select One)   <<<<<<<*/
/*---------------------------------------------------------------*/
#define ENC2_A_PIN PB_6  //EXT Header / UART1_TX / Alternate Function AF1 = TIM4_CH1
#define ENC2_B_PIN PB_7  //EXT Header / UART1_RX / Alternate Function AF1 = TIM4_CH2
// #define ENC_A_PIN PD_12  //NO CONNECT / Alternate Function AF1 = TIM4_CH1  (Note: Not Routed to a header. requires direct connection to U1)
// #define ENC_B_PIN PD_13  //NO CONNECT / Alternate Function AF1 = TIM4_CH2  (Note: Not Routed to a header. requires direct connection to U1)

/*---------------------------------------------------------------*/
/*>>>>>>>    ENCODER #3 - TIMER5 (32-bit)   <<<<<<<<<<<<<<<<<<<<<*/
/*---------------------------------------------------------------*/
#define ENC3_A_PIN PA_0  //JP11 Header /   GPIO6   / Alternate Function AF2 = TIM5_CH1
#define ENC3_B_PIN PA_1  //JP11 Header /   GPIO5   / Alternate Function AF2 = TIM5_CH2

/*---------------------------------------------------------------*/
/*>>>>>>>   ENCODER #4 - TIMER2 (32bit)     <<<<<<<<<<<<<<<<<<<<<*/
/*---------------------------------------------------------------*/
#define ENC4_A_PIN PA_15  //EXT Header  / SPI3_CS1   / Alternate Function AF1 = TIM2_CH1
#define ENC4_B_PIN PB_3   //EXT Header  / GPIO4(SWO) / Alternate Function AF1 = TIM2_CH2
//#define ENC_B_PIN PA_1    //JP11 Header / GPIO5    / Alternate Function AF1 = TIM2_CH2 (Used by TIM5_CH2)


#else
#error "ERROR: This platform isn't recognized."
#endif