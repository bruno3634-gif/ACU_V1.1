#include "sequences.h"

volatile bool SDC_prev_state = true;   // false = open, true = closed  
unsigned long timeout = 0;  
bool wdt_tested = false;
unsigned long wdt_test_time = 0;

typedef enum {
    WAIT_SDC_OPEN,
    WDT_TEST,
    WDT_TEST_CLOSE,
    BRAKE_PRESSURE_CHECK,
    IGNITION,
    Test_MB1,
    Test_MB2,
    last_state
} SEQ_STATE;

SEQ_STATE init_sequence_state = WAIT_SDC_OPEN;

void initial_sequence(){

    bool conditions_met = true; // change when pressures available


    extern float pneumatic_pressure1_avg;
    extern float pneumatic_pressure2_avg; 

    extern STATE ACU_STATE; 
    extern bool wdt_pin_enable;
    switch(init_sequence_state){
        case WAIT_SDC_OPEN:
            if(digitalRead(SDC_FEEDBACK) == 1){ // SDC OPEN
                init_sequence_state = WDT_TEST;
            }

        break;

        case WDT_TEST:
            if(digitalRead(SDC_FEEDBACK) == 1){
                if(wdt_test_time + 10 <= millis()){
                    digitalWrite(wdt_pin, !digitalRead(wdt_pin));
                    wdt_test_time = millis();
                }
            }else{
                wdt_test_time = millis();
                init_sequence_state = WDT_TEST_CLOSE;
            }
        break;
        case WDT_TEST_CLOSE:
            wdt_pin_enable = true;
            if(digitalRead(SDC_FEEDBACK) == 0 && wdt_test_time + 300 <= millis()){              
                    ACU_STATE = EMERGENCY;  
            }else{
                init_sequence_state = BRAKE_PRESSURE_CHECK;
            }
            break;
        case BRAKE_PRESSURE_CHECK:
            if(pneumatic_pressure1_avg < 6 || pneumatic_pressure1_avg > 10 || pneumatic_pressure2_avg < 6 || pneumatic_pressure2_avg > 10){
                ACU_STATE = EMERGENCY;
            }
        break;
        case IGNITION:
            
        break;
        case Test_MB1:
            digitalWrite(solenoid_1_pin, LOW); // open
            digitalWrite(solenoid_2_pin, HIGH);// close
            /***
             * TODO: Read hydraulic preassures from CAN, 
             */
            
            
            if (conditions_met){    
               init_sequence_state = Test_MB2;
            }else{
                ACU_STATE = EMERGENCY;
            }
            
            break;
        case Test_MB2:
            digitalWrite(solenoid_1_pin, HIGH);// close
            digitalWrite(solenoid_2_pin, LOW);// open
            /***
             * TODO: Read hydraulic preassures from CAN, 
             */
            break;
            case last_state:
                digitalWrite(solenoid_1_pin, LOW); // open
                digitalWrite(solenoid_2_pin, LOW); // open
                ACU_STATE = AUTONOMOUS_MODE;
                break;
        default:
            Serial.println("Unexpected Error in initial sequence");
        break;
    }

}



void continuous_sequence(){
    extern STATE ACU_STATE;

    if(!digitalRead(SDC_FEEDBACK) == 0){
        if(SDC_prev_state)
        {
            SDC_prev_state = false;
            timeout = millis();
        }else{
            if(millis() - timeout >= 50){
                /***
                 * TODO:
                 * 1. Read brake preassures from CAN
                 * 2. Check they are between 60 and 120 bar
                 * 3. Engage brakes and error or Stop monitoring
                 */
                bool pressures_ok = true;
                if(pressures_ok){
                    ACU_STATE = MANUAL_MODE;
                }
            }
        }
        
    }
    else{
        extern unsigned long last_msg_time[4]; // 0 -RES, 1 - MAXON, 2. JETSON, 3 - VCU
        for(int i = 0; i < 4; i++){
            if(millis() - last_msg_time[i] >= 2000){
                ACU_STATE = EMERGENCY;
                //  engage brakes
                // error 
            }
        }
        if(BP1 < 6 || BP1 > 10 || BP2 < 6 || BP2 > 10){
            ACU_STATE = EMERGENCY;
            // engage brakes
            // error

        }
    }
}