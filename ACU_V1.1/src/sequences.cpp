#include "sequences.h"

volatile bool SDC_prev_state = true;   // false = open, true = closed  
unsigned long timeout = 0;  

void initial_sequence(){

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