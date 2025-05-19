#include "Autonomous_IO_control.h"




void ASSI(int ASSI_status)
{
switch (ASSI_status)
  {
  case 1:
    digitalWrite(YELLOW_LEDS, HIGH);
    digitalWrite(BLUE_LEDS, LOW);

    break;
  case 2:
    digitalWrite(YELLOW_LEDS, LOW);
    digitalWrite(BLUE_LEDS, HIGH);

    break;
  case 3:
    digitalWrite(BLUE_LEDS, LOW);

    extern unsigned long ASSI_YELLOW_time;

    if (millis() - ASSI_YELLOW_time >= 500)
    {
      ASSI_YELLOW_time = millis();
      digitalWrite(YELLOW_LEDS, !digitalRead(YELLOW_LEDS));
    }

    break;
  case 4:
    extern unsigned long ASSI_BLUE_time;
    digitalWrite(YELLOW_LEDS, HIGH);
    if (millis() - ASSI_BLUE_time >= 500)
    {
         
      ASSI_BLUE_time = millis();
      digitalWrite(BLUE_LEDS, !digitalRead(BLUE_LEDS));
    }

    break;
  case 5:
    digitalWrite(YELLOW_LEDS, HIGH);
    digitalWrite(BLUE_LEDS, LOW);
    break;

  default:

    digitalWrite(YELLOW_LEDS, HIGH);
    digitalWrite(BLUE_LEDS, HIGH);

    break;
  }
}

void Mission_Indicator(int mission)
{
  switch (mission)
  {
  case 0:
    digitalWrite(MS_LED1, HIGH);
    digitalWrite(MS_LED2, LOW);
    digitalWrite(MS_LED3, LOW);
    digitalWrite(MS_LED4, LOW);
    digitalWrite(MS_LED5, LOW);
    digitalWrite(MS_LED6, LOW);
    digitalWrite(MS_LED7, LOW);
    break;
  case 1:
    digitalWrite(MS_LED1, LOW);
    digitalWrite(MS_LED2, HIGH);
    digitalWrite(MS_LED3, LOW);
    digitalWrite(MS_LED4, LOW);
    digitalWrite(MS_LED5, LOW);
    digitalWrite(MS_LED6, LOW);
    digitalWrite(MS_LED7, LOW);
    break;
  case 2:
    digitalWrite(MS_LED1, LOW);
    digitalWrite(MS_LED2, LOW);
    digitalWrite(MS_LED3, HIGH);
    digitalWrite(MS_LED4, LOW);
    digitalWrite(MS_LED5, LOW);
    digitalWrite(MS_LED6, LOW);
    digitalWrite(MS_LED7, LOW);

    case 3:
    digitalWrite(MS_LED1, LOW);
    digitalWrite(MS_LED2, LOW);     
    digitalWrite(MS_LED3, LOW);
    digitalWrite(MS_LED4, HIGH);
    digitalWrite(MS_LED5, LOW);
    digitalWrite(MS_LED6, LOW);
    digitalWrite(MS_LED7, LOW);
    break;
  case 4:
    digitalWrite(MS_LED1, LOW);
    digitalWrite(MS_LED2, LOW);
    digitalWrite(MS_LED3, LOW);
    digitalWrite(MS_LED4, LOW);
    digitalWrite(MS_LED5, HIGH);
    digitalWrite(MS_LED6, LOW);
    digitalWrite(MS_LED7, LOW);
    break;
  case 5:
    digitalWrite(MS_LED1, LOW);
    digitalWrite(MS_LED2, LOW);
    digitalWrite(MS_LED3, LOW);
    digitalWrite(MS_LED4, LOW);
    digitalWrite(MS_LED5, LOW);
    digitalWrite(MS_LED6, HIGH);
    digitalWrite(MS_LED7, LOW);
    break;
    case 6:
    digitalWrite(MS_LED1, LOW);
    digitalWrite(MS_LED2, LOW);
    digitalWrite(MS_LED3, LOW);
    digitalWrite(MS_LED4, LOW); 
    digitalWrite(MS_LED5, LOW);
    digitalWrite(MS_LED6, LOW);
    digitalWrite(MS_LED7, HIGH);
    break;
  default:
    digitalWrite(MS_LED1, LOW);
    digitalWrite(MS_LED2, LOW);
    digitalWrite(MS_LED3, LOW);
    digitalWrite(MS_LED4, LOW);
    digitalWrite(MS_LED5, LOW);
    digitalWrite(MS_LED6, LOW);
    digitalWrite(MS_LED7, LOW);
      break;
  }
}



uint8_t Mission_Selet(uint8_t mission){
    unsigned long time = millis();
    if(digitalRead(MS_BUTTON) == HIGH)
    {
        while(digitalRead(MS_BUTTON) == HIGH)
        {
            
        }
        if(millis() - time >= 100)
        {
            mission++;
            if(mission > 6)
            {
                mission = 0;
            }
        }
    }
    return mission;
}