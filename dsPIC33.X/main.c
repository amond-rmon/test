/*
? [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/i2c_host/i2c2.h"
#include "sources/os/os.h"
#include "MCP79410.h"
#include "mcc_generated_files/timer/tmr1.h"
#include "mcc_generated_files/timer/delay.h"


/*
    Main application
*/

int main(void)
{
    uint8_t index = 0;
    
    SYSTEM_Initialize();
    
    DELAY_milliseconds(100);

//    for(index = 0; index < 14; index++) {
//        printf("\n\r Index : %d \n\r", index);
        MCP79410_Print();
//    }
    
    printf("\n\r MCP79410 Initinal \n\r");
    MCP79410_Init();
//    MCP79410_Start(); 
    
//    for(index = 0; index < 9; index++) {
//        printf("\n\r Index : %d \n\r", index);
//        MCP79410_Print();
//    }
    
    
    OS_Init();
    TMR1_Start();
    OS_Scheduler_RunForever();
    
    while(1)
    {
    }    
}

void I2C2_Callback(void) {
    rtcc_status.Interrupt = 1;
}

