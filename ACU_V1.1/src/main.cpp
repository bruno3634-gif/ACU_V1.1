#include <Arduino.h>
#include "defines.h"
#include "CAN.h"
#include "Autonomous_IO_control.h"
#include "Watchdog_t4.h"
#include "sequences.h"






STATE ACU_STATE = MISSION_SELECT;

IntervalTimer CAN_Timer;
IntervalTimer Non_critical_CAN_Timer;
#if PRESSURE_READINGS_ENABLE
IntervalTimer Pressure_Timer;
#endif


volatile uint8_t Mission_flag = 0, Mission_confirmation = 0;
volatile uint8_t Ignition_flag = 0, Ignition_confirmation = 0;
volatile uint8_t R2D_flag = 0, R2D_confirmation = 0;
volatile uint8_t Autonomous_Mission_STATUS = 0;
volatile bool wdt_pin_enable = true;


unsigned long heartbit = 0, hardware_wdt = 0;
unsigned long ASSI_YELLOW_time = 0, ASSI_BLUE_time = 0;


#if PRESSURE_READINGS_ENABLE
uint8_t pneumatic_counter = 0;
float pneumatic_pressure1[4] = {0,0,0,0};
float pneumatic_pressure2[4] = {0,0,0,0};

float pneumatic_pressure1_avg = 0;
float pneumatic_pressure2_avg = 0;
bool pneumatic_pressure_flag = false;
#endif

/***
 * last time msg was received for
 * 1. RES
 * 2. MAXON
 * 3. JETSON
 * 4. VCU
 */

unsigned long last_msg_time[4] = {0, 0, 0, 0};


WDT_T4<WDT1> wdt;

void can_tmr_callback();
void can_tmr2_callback();
void pressure_readings_callback();

void setup() {
  pinMode(R2D_PIN, INPUT);
  pinMode(MS_BUTTON,INPUT);
  pinMode(ASMS_PIN, INPUT);

  pinMode(MS_LED1, OUTPUT);
  pinMode(MS_LED2, OUTPUT);
  pinMode(MS_LED3, OUTPUT);
  pinMode(MS_LED4, OUTPUT);
  pinMode(MS_LED5, OUTPUT);
  pinMode(MS_LED6, OUTPUT);
  pinMode(MS_LED7, OUTPUT);

  pinMode(HEARTBIT_LED, OUTPUT);
  pinMode(Debug_LED1, OUTPUT);
  pinMode(Debug_LED2, OUTPUT);
  pinMode(Debug_LED3, OUTPUT);
  pinMode(Debug_LED4, OUTPUT);
  pinMode(CAN_LED_TX, OUTPUT);

  pinMode(YELLOW_LEDS, OUTPUT);
  pinMode(BLUE_LEDS, OUTPUT);
  pinMode(wdt_pin, OUTPUT);
  pinMode(solenoid_1_pin, OUTPUT);
  pinMode(solenoid_2_pin, OUTPUT);  

  pinMode(BP1, INPUT);
  pinMode(BP2, INPUT);

  CAN_init();
  Serial.begin(115200);
  Serial2.begin(115200);
  CAN_Timer.begin(can_tmr_callback, 200000); // 200ms
  Non_critical_CAN_Timer.begin(can_tmr2_callback, 1000000); // 1s
  #if PRESSURE_READINGS_ENABLE
  Pressure_Timer.begin(pressure_readings_callback, 50000); // 50ms 
  #endif
  
  WDT_timings_t config;           /* in seconds, 0->128 Warning trigger before timeout */
  config.timeout = 2;            /* in seconds, 0->128 Timeout to reset */// Callback function to be called on timeout
  wdt.begin(config); 
  
}

void loop() {

  wdt.feed();
  if(heartbit + 1000 <= millis())
  {
    digitalWriteFast(HEARTBIT_LED, !digitalRead(HEARTBIT_LED));
    heartbit = millis();
  }
  if(hardware_wdt + 10 <= millis() && wdt_pin_enable)
  {
    hardware_wdt = millis();
  }

  can_msg_read();

  Mission_Indicator(Mission_confirmation);

  switch (ACU_STATE)
  {
  case MISSION_SELECT:
    if(digitalRead(ASMS_PIN) == HIGH) // Here also check in can if the car is ready to drive
    {
      Mission_confirmation == 0 ? ACU_STATE = MANUAL_MODE : ACU_STATE = INITIAL_SEQUENCE;
    }
    else 
    {
      Mission_flag = Mission_Selet(Mission_flag);
    }
    
    break;
  case INITIAL_SEQUENCE:
    initial_sequence();
    break;
  break;
  case MANUAL_MODE:
    wdt_pin_enable = true;
    digitalWrite(solenoid_1_pin, LOW);  // No EBS activation
    digitalWrite(solenoid_2_pin, LOW);  // No EBS activation
    break;
  case AUTONOMOUS_MODE:
      wdt_pin_enable = true;
      continuous_sequence();
    break;
  case EMERGENCY:
    // Activate the solenoid valves & stop feeding wdt_pin
    wdt_pin_enable = false;
    digitalWrite(solenoid_1_pin, HIGH);
    digitalWrite(solenoid_2_pin, HIGH);
  break;
    case RESET:
      
      wdt.reset();

    break;
  default:
    break;
  }
#if PRESSURE_READINGS_ENABLE
  if(pneumatic_pressure_flag){
    pneumatic_pressure_flag = false;
    pneumatic_pressure1_avg = adc_hardware_compensation * ((pneumatic_pressure1[0] + pneumatic_pressure1[1] + pneumatic_pressure1[2] + pneumatic_pressure1[3])  / 4);
     //Serial.println("Voltage 1: " + String(pneumatic_pressure1_avg));
    pneumatic_pressure2_avg = adc_hardware_compensation * ((pneumatic_pressure2[0] + pneumatic_pressure2[1] + pneumatic_pressure2[2] + pneumatic_pressure2[3])  / 4);
    pneumatic_pressure1_avg = (pneumatic_pressure1_avg - 0.5)/0.4;
    pneumatic_pressure2_avg = (pneumatic_pressure2_avg - 0.5)/0.4;
    //Serial.println("Pressure 1: " + String(pneumatic_pressure1_avg));
    //Serial.println("Pressure 2: " + String(pneumatic_pressure2_avg));
  }
#endif

}


void can_tmr_callback()
{
  CAN_message_t msg;
  switch (ACU_STATE)
  {
  case MISSION_SELECT:
    
  msg.id = ACU_MS;
  msg.len = 1;
  msg.buf[0] = Mission_flag;
  //can_msg_send(msg.id, msg.len, msg.buf);
   
    break;
  
  default:
    break;
  }
  msg.id = ACU_MS;
  msg.len = 1;
  msg.buf[0] = Mission_flag;
  can_msg_send(msg.id, msg.len, msg.buf);
}



void can_tmr2_callback(){
  CAN_message_t msg;
  msg.id = 0x111;
  msg.len = 1;
  msg.buf[0] = 0x01;
  can_msg_send(msg.id, msg.len, msg.buf);
}


void pressure_readings_callback()
{
  #if PRESSURE_READINGS_ENABLE
  float digi_bp1 = analogRead(BP1);
  pneumatic_pressure1[pneumatic_counter] = (digi_bp1 * 3.3) / 1024;
  float digi_bp2 = analogRead(BP2);
  pneumatic_pressure2[pneumatic_counter] = (digi_bp2* 3.3) / 1024;
 // Serial.println("Pressure 1: " + String(pneumatic_pressure1[pneumatic_counter]));
  pneumatic_counter++;
  if(pneumatic_counter >3)
  {
    //wdt.reset();
    pneumatic_counter = 0;
  }
  pneumatic_pressure_flag = true;
  #endif
  
}