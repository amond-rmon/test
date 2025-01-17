//=======================================================================================================
// Copyright(c) 2018 Microchip Technology Inc. and its subsidiaries.
// Subject to your compliance with these terms, you may use Microchip software and any derivatives
// exclusively with Microchip products. It is your responsibility to comply with third party license
// terms applicable to your use of third-party software (including open source software) that may
// accompany Microchip software.
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
// APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND
// FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
// LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
// MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT
// ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT
// EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//=======================================================================================================

//=======================================================================================================
// @file main_tasks.c
//
// @brief contains the application specific tasks that are called regularly by the os_scheduler
//        two different timings priorities are available:
//          1. 1ms Tasks called from the os_scheduler interrupt
//              the jitter that you will have in the 1ms realtime tasks called by the interrupt depends
//              on other interrupts that have a higher interrupt priority
//          2. 1ms, 10ms, 100ms, 1s Tasks called from the main loop
//              these tasks are for soft realtime and not for hard realtime
//              so in average they are called with the required timing but the jitter can be very huge,
//              depending on the calls before.
//              use this for your non-timing critical application state machines
//
// @note    you might consider to implement a watchdog and put the watchdog triggering into some Task
//
// @version v1.0
// @date 2019-08-09
// @author M52409
//
//=======================================================================================================
#define _MAIL_BOX  1

#include <stdint.h>
#include <xc.h>

#include <stdio.h>

#include "sources/config/os_settings.h"
//#include "mail_box.h"

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/i2c_host/i2c2.h"

#include "MCP79410.h"
#include "mcc_generated_files/timer/delay.h"




//=======================================================================================================
//
//                          put your application specific code in the following functions:
//                              choose wisely between real-time and non-realtime!
//
//  Interrupt Realtime Functions:
//  Tasks_Realtime_1ms  :   is called by the interrupt every ms - for time critical low jitter stuff
//
//
//  Mainloop Non-Realtime Functions:
//  Tasks_1ms           :   function is called by the main loop in average every 1ms
//  Tasks_10ms          :   function is called by the main loop in average every 10ms
//  Tasks_100ms         :   function is called by the main loop in average every 100ms
//  Tasks_1s            :   function is called by the main loop in average every second
//
//  @note there could be some jitter here because it is not called directly by a timer interrupt
//        the timing in average is exact (keep in mind: in average), the jitter depends on the
//        called functions before
//=======================================================================================================


//=======================================================================================================
//  @brief  Tasks_Realtime_100us gets called directly from the timer interrupt every 100 �s
//  @note   keep this routine as short as possible
//=======================================================================================================

void Tasks_Realtime_100us(void)
{
    
}

//=======================================================================================================
//  @brief  Tasks_Realtime_1ms gets called directly from the timer interrupt every millisecond
//  @note   keep this routine as short as possible
//=======================================================================================================
void Tasks_Realtime_1ms(void)
{

}

//=======================================================================================================
//  @brief  Tasks_100us gets called every 100�s, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================
void Tasks_100us(void)
{
    
}

//=======================================================================================================
//  @brief  Tasks_1ms gets called every millisecond, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================
void Tasks_1ms(void)
{

}

//=======================================================================================================
//  @brief  Tasks_10ms gets called every 10ms, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================
void Tasks_10ms(void)
{
    
}

//=======================================================================================================
//  @brief  Tasks_100ms gets called every 100 ms, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================

void Tasks_100ms(void)
{
    static volatile uint16_t count = 0;
    
    switch(count++) {
        case 3:
            LD3_Toggle();
            break;
            
        case 5:
            break;
            
        case 9:
            count = 0;
            break;
            
        default:
            if(rtcc_status.Issue == 1) {
                LD2_Toggle();
            }
            break;
    }
}

//=======================================================================================================
//  @brief  Tasks_1s gets called every second, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================

void Tasks_1s(void)
{
//    if(IO_RB13_GetValue() == 0) {
//        printf("**************** ALM0IF = 1 ****************\n\r");
//        MCP79410_Reset_Hour();
//        printf("**************** ALM0IF = 0 ****************\n\r");
//    }
//    MCP79410_Print();
}

//=======================================================================================================
//  @brief  Tasks_Background gets called all the time when no other of the above tasks are being called
//  @note   call this function when you want to implement your own timing or get code called as often
//          as possible. You can also put your timing variables into Tasks_Realtime_100us or
//          Tasks_Realtime_1ms. This way you get accurate timing variables that you can use here.
//=======================================================================================================
void Tasks_Background(void) 
{
    if(IO_RB13_GetValue() == 0) {
        LD3_SetHigh();
        MCP79410_Print();
        printf("**************** ALM0IF = 1 ****************\n\r");
        MCP79410_Reset_Hour();
        printf("**************** ALM0IF = 0 ****************\n\r");
        LD3_SetLow();
    }
}
