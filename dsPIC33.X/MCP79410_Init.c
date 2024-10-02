#include <xc.h>

#include "mcc_generated_files/i2c_host/i2c2.h"
#include "MCP79410.h"
#include "mcc_generated_files/timer/delay.h"
#include "mcc_generated_files/uart/uart1.h"
#include "mcc_generated_files/system/reset.h"

uint16_t data_temp_ten;
uint16_t data_temp_one;
uint16_t dummy;
uint16_t flag_MCP79410;

typedef struct{
    uint8_t addr;
    uint8_t command;
    char name[16];
}cmd;


MCP79410_STATUS_u rtcc_status;

cmd MCP79410_initCmd[] = {
    /* 0x00 */  {RTCSEC,     0x30,                              "RTCSEC"},
    /* 0x01 */  {RTCMIN,     0x05,                              "RTCMIN"},
    /* 0x02 */  {RTCHOUR,    0x01,                              "RTCHOUR"},
    /* 0x03 */  {RTCWKDAY,   (PWRFAIL|VBATEN|WKDAY),            "RTCWKDAY"},
    /* 0x04 */  {RTCDATE,    0x18,                              "RTCDATE"},
    /* 0x05 */  {RTCMTH,     0x09,                              "RTCMTH"},
    /* 0x06 */  {RTCYEAR,    0x23,                              "RTCYEAR"},
    /* 0x07 */  {CONTROL,    (ALM1EN|ALM0EN|EXTOSC|CRSTRIM),    "CONTROL"}, // ALM0EN=1, CRSTRIM=1
    /* 0x08 */  {OSCTRIM,    0x07,                              "OSCTRIM"},
    /* 0x09 */  {EEUNLOCK,   0x00,                              "EEUNLOCK"},
    /* 0x0A */  {ALM0SEC,    0x00,                              "ALM0SEC"},
    /* 0x0B */  {ALM0MIN,    0x00,                              "ALM0MIN"},
    /* 0x0C */  {ALM0HOUR,   0x02,                              "ALM0HOUR"},
    /* 0x0D */  {ALM0WKDAY,  (ALMMSK|ALM0IF|WKDAY),             "ALM0WKDAY"},
    /* 0x0E */  {ALM0DATE,   0x00,                              "ALM0DATE"},
    /* 0x0F */  {ALM0MTH,    0x00,                              "ALM0MTH"},
};

cmd MCP79410_rxBuf[] = {
    /* 0x00 */  {RTCSEC,     0x30,                              "RTCSEC"},
    /* 0x01 */  {RTCMIN,     0x06,                              "RTCMIN"},
    /* 0x02 */  {RTCHOUR,    0x16,                              "RTCHOUR"},
    /* 0x03 */  {RTCWKDAY,   (PWRFAIL|VBATEN|WKDAY),            "RTCWKDAY"},
    /* 0x04 */  {RTCDATE,    0x18,                              "RTCDATE"},
    /* 0x05 */  {RTCMTH,     0x09,                              "RTCMTH"},
    /* 0x06 */  {RTCYEAR,    0x23,                              "RTCYEAR"},
    /* 0x07 */  {CONTROL,    (ALM1EN|ALM0EN|EXTOSC|CRSTRIM),    "CONTROL"}, // ALM0EN=1, CRSTRIM=1
    /* 0x08 */  {OSCTRIM,    0x07,                              "OSCTRIM"},
    /* 0x09 */  {EEUNLOCK,   0x00,                              "EEUNLOCK"},
    /* 0x0A */  {ALM0SEC,    0x00,                              "ALM0SEC"},
    /* 0x0B */  {ALM0MIN,    0x00,                              "ALM0MIN"},
    /* 0x0C */  {ALM0HOUR,   0x00,                              "ALM0HOUR"},
    /* 0x0D */  {ALM0WKDAY,  (ALMMSK|ALM0IF|WKDAY),             "ALM0WKDAY"},
    /* 0x0E */  {ALM0DATE,   0x00,                              "ALM0DATE"},
    /* 0x0F */  {ALM0MTH,    0x00,                              "ALM0MTH"},
};

void MCP79410_Init(void) {
    uint8_t index= 0;
    
    rtcc_status.flag = 0;
    rtcc_status.First = 1;
    
#if 0
    index = RTCSEC;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
//    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].register_name, 1, &MCP79410_initCmd[index].command, 1));
//    MCP79410_process();
    
    index = RTCWKDAY;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = ALM0WKDAY;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = ALM0SEC;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = ALM0MIN;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = ALM0HOUR;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    /* for fast test, setting CRSTRIM=1 and OSCTRIM = 0x7F */
    index = CONTROL;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = OSCTRIM;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    /* end of setting fast test */
    
    index = RTCYEAR;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = RTCMTH;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = RTCDATE;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = RTCHOUR;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = RTCMIN;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = RTCWKDAY;
    MCP79410_initCmd[index].command = 0x0A;
//    MCP79410_initCmd[index].command = 0x09;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
    MCP79410_process();
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    
    index = RTCSEC;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    MCP79410_initCmd[index].command = 0xB0;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index], 2));
    MCP79410_process();
#else
    for(index = 0; index < 16; index++) {
        while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index].addr, 2));
        MCP79410_process();
        while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
        MCP79410_process();
    }
    
    index = RTCSEC;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[index].addr, 1, &MCP79410_initCmd[index].command, 1));
    MCP79410_process();
    MCP79410_initCmd[index].command = 0xB0;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[index], 2));
    MCP79410_process();
#endif
    
    rtcc_status.Initinal = 1;
    
    printf("\n\r");
    for(index = 0; index < 16; index++) {
        printf("%s 0x%2X = 0x%2X \n\r", 
                MCP79410_initCmd[index].name, 
                MCP79410_initCmd[index].addr, 
                MCP79410_initCmd[index].command);
//        DELAY_milliseconds(20);
        while(!UART1_IsTxDone());
    }
}

void MCP79410_Start(void) {
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[RTCWKDAY].addr, 1, &MCP79410_initCmd[RTCWKDAY].command, 1));
    MCP79410_process();
    
    dummy = (MCP79410_initCmd[RTCWKDAY].command & 0xF0) >> 4;
    
    if(dummy >= 1) {
        MCP79410_initCmd[RTCWKDAY].command = (PWRFAIL|VBATEN|WKDAY);
        while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[RTCWKDAY], 2));
        MCP79410_process();
    }
    MCP79410_initCmd[RTCSEC].command = ST;
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[RTCSEC], 2));
    MCP79410_process();
}

void MCP79410_Reset_Hour(void) {
    
    static volatile uint16_t count_reset = 0;
    /* Read Control */
//    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_cmd[CONTROL].register_name, 1, &MCP79410_cmd[CONTROL].command, 1));
//    MCP79410_process();
//    printf("CONTROL : 0x%2X \n\r", MCP79410_cmd[CONTROL].command);
    
    
    /* reset ALM0HOUR */
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[ALM0HOUR].addr, 1, &MCP79410_initCmd[ALM0HOUR].command, 1));
    MCP79410_process();
    printf("ALM0HOUR : 0x%2X \n\r", MCP79410_initCmd[ALM0HOUR].command);
    
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[RTCHOUR].addr, 1, &MCP79410_initCmd[RTCHOUR].command, 1));
    MCP79410_process();
    printf("RTCHOUR : 0x%2X \n\r", MCP79410_initCmd[RTCHOUR].command);
    
    if(MCP79410_initCmd[RTCHOUR].command == MCP79410_initCmd[ALM0HOUR].command) {
        count_reset++;
        printf("Count Rest : %d \n\r", count_reset);
    }
    else {
        rtcc_status.Issue = 1;
        printf("/******** Issue Detect ********/ \n\r");
    }
    
    if(count_reset >= 4) {
        asm("RESET");
    }
    
    dummy = MCP79410_initCmd[ALM0HOUR].command;
    
    if(rtcc_status.First == 1) {
        dummy = 0;
        rtcc_status.First = 0;
    }
    else {
        if(dummy == 0) {
            dummy = 0x23;
        }
        else {
            data_temp_ten = (dummy & 0xF0) >> 4;
            data_temp_one = dummy & 0x0F;

            if(data_temp_one  == 0) {
                data_temp_ten -= 1;
                data_temp_one = 9;
            }
            else {
                data_temp_one -= 1;
            }

            dummy = (data_temp_ten << 4) | data_temp_one;
        }
    }
    
    MCP79410_initCmd[ALM0HOUR].command = dummy;
    printf("Write ALM0HOUR \n\r");
    printf("Register : 0x%2X | Command : 0x%2X \n\r", MCP79410_initCmd[ALM0HOUR].addr, MCP79410_initCmd[ALM0HOUR].command);
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[ALM0HOUR], 2));
    MCP79410_process();


    /* Read ALM0WKDAY for test */
//    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_cmd[ALM0WKDAY].register_name, 1, &MCP79410_cmd[ALM0WKDAY].command, 1));
//    MCP79410_process();
//    printf("ALM0WKDAY : 0x%2X \n\r", MCP79410_cmd[ALM0WKDAY].command);

    /* Set ALM0IF = 0 for test */
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_initCmd[ALM0WKDAY].addr, 1, &MCP79410_initCmd[ALM0WKDAY].command, 1));
    MCP79410_process();
    MCP79410_initCmd[ALM0WKDAY].command &= 0xF7;
    printf("Write ALM0WKDAY \n\r");
    printf("Register : 0x%2X | Command : 0x%2X \n\r", MCP79410_initCmd[ALM0WKDAY].addr, MCP79410_initCmd[ALM0WKDAY].command);
    while(!I2C2_Write(MCP79410_address, &MCP79410_initCmd[ALM0WKDAY], 2));
    MCP79410_process();
    
    /* Read Control */
//    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_cmd[CONTROL].register_name, 1, &MCP79410_cmd[CONTROL].command, 1));
//    MCP79410_process();
//    printf("CONTROL : 0x%2X \n\r", MCP79410_cmd[CONTROL].command);
}

void MCP79410_Print(void) {
    uint8_t index = 0;
    
#if 0
    index = ALM0WKDAY;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = RTCWKDAY;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();  
//    if(rtcc_status.Initinal == 0) {
//        MCP79410_rxBuf[index].command = 0x2F;
//    }
//    else {
//        MCP79410_rxBuf[index].command = 0x29;
//    }
    while(I2C2_Write(MCP79410_address, &MCP79410_rxBuf[index], 2));
    MCP79410_process();  
    
    index = RTCSEC;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = RTCMIN;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();  
    
    index = RTCHOUR;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = RTCDATE;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = RTCMTH;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = RTCYEAR;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = CONTROL;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = OSCTRIM;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = ALM0HOUR;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = ALM0MIN;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process();    
    
    index = ALM0SEC;
    while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
    MCP79410_process(); 
#else
    for(index = 0; index < 16; index++) {
        while(!I2C2_WriteRead(MCP79410_address, &MCP79410_rxBuf[index].addr, 1, &MCP79410_rxBuf[index].command, 1));
        MCP79410_process();
    }
    
#endif
    
    printf("\n\r");
    for(index = 0; index < 16; index++) {
        printf("%s 0x%2X = 0x%2X \n\r", 
                MCP79410_rxBuf[index].name, 
                MCP79410_rxBuf[index].addr, 
                MCP79410_rxBuf[index].command);
//        DELAY_milliseconds(20);
        while(!UART1_IsTxDone());
    }
}

void MCP79410_process(void) {
    while(!rtcc_status.Interrupt);
    rtcc_status.Interrupt = 0;
//    DELAY_microseconds(15);
    DELAY_milliseconds(10);
}
