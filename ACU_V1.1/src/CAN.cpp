#include "CAN.h"


FlexCAN_T4<CAN2, RX_SIZE_1024, TX_SIZE_1024> can2;

#define CAN_FILTERS_ACTIVE 1


void can_msg_read(){
    CAN_message_t msg;
    extern volatile uint8_t R2D_confirmation;
    extern unsigned long last_msg_time[4]; // 0 -RES, 1 - MAXON, 2. JETSON, 3 - VCU

    if (can2.read(msg)) {
        switch (msg.id)
        {
        case JETSON_MS:
            extern volatile uint8_t Mission_confirmation;
            Mission_confirmation = msg.buf[0];
            last_msg_time[2] = millis();
            break;
        case VCU_IGN:
            extern volatile uint8_t Ignition_confirmation;
            Ignition_confirmation = msg.buf[0];
            last_msg_time[3] = millis();
            break;
        case JETSON_AMS:   
            extern volatile uint8_t Autonomous_Mission_STATUS;
            Autonomous_Mission_STATUS = msg.buf[0];
            last_msg_time[2] = millis();
        break;
        case RES_ID:
            last_msg_time[0] = millis();
            break;
        default:
            break;
        }
    }
}


void can_msg_send(uint8_t id, uint8_t lenght, uint8_t data[]){
    CAN_message_t msg;
    msg.id = id;
    msg.len = lenght;
    for (int i = 0; i < lenght; i++)
    {
        msg.buf[i] = data[i];
    }
    if(can2.write(msg)){
        digitalWrite(CAN_LED_TX, !digitalRead(CAN_LED_TX));
    }
    
}

void CAN_init() {
    can2.begin();
    can2.setBaudRate(1000000);

    #if CAN_FILTERS_ACTIVE
        can2.setMBFilter(MB0, RES_ID);              // Only accept RES_ID on mailbox 0
        can2.setMBFilter(MB1, IGN_FROM_VCU);        // Only accept IGN_FROM_VCU on mailbox 1
        can2.setMBFilter(MB2, JETSON_AMS);          // Only accept JETSON_AMS on mailbox 2
        can2.setMBFilter(MB3, JETSON_MS); 
        for (int i = 4; i < 16; i++) {
            can2.setMBFilter((FLEXCAN_MAILBOX)i, 0); 
        }
    #endif
}