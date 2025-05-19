#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef enum {
    MISSION_SELECT,
    INITIAL_SEQUENCE,
    MANUAL_MODE,
    AUTONOMOUS_MODE,
    EMERGENCY,
    RESET
} STATE;





// PIN DEFINITIONS

/***
 * Buttons
 */

#define R2D_PIN 4
#define ASMS_PIN 38
#define MS_BUTTON 21

/***
 * Mission indicator LEDs
 */

#define MS_LED1 14
#define MS_LED2 15
#define MS_LED3 16
#define MS_LED4 17  
#define MS_LED5 33
#define MS_LED6 13
#define MS_LED7 20

#define YELLOW_LEDS 24
#define BLUE_LEDS 23

/***
 * Debug LEDs
 */

 #define HEARTBIT_LED 28
 #define Debug_LED1 29
 #define Debug_LED2 34
 #define Debug_LED3 35
 #define Debug_LED4 30
 #define Debug_LED5 31
 #define CAN_LED_TX 12


/***
 * CAN ID'S
 */

#define RES_ID 0x191
#define ACU_MS 0x51
#define JETSON_MS 0x61
#define VCU_IGN 0x71        // -> Envio para te dizer para abrir os contactores  1º byte -> 0 -> Ignição desligada 1 -> Ignição ligada
#define JETSON_AMS 0x502
#define IGN_FROM_VCU 0x81   // -> Envias para me dizer que os contactores estão fechados 1º byte -> 0 -> Contactores abertos 1 -> Contactores fechados
#define IGN_TO_ACU 0x512     



/***
 * Emergency
 */

 #define wdt_pin 39
 #define solenoid_1_pin 36
 #define solenoid_2_pin 10


 #define SDC_FEEDBACK 25  // will read one when open



/***
 * ADC's                                
 */
#define BP1 26
#define BP2 27




#define PRESSURE_READINGS_ENABLE 0



#endif // DEFINITIONS_H