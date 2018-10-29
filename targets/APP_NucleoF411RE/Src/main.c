/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "sys_init.h"
#include "agent_tiny_demo.h"
#include "at_api_interface.h"
#include <osport.h>
#include <at.h>
#include <shell.h>


UINT32 g_TskHandle = 0xFFFF;

VOID HardWare_Init(VOID)
{
    SystemClock_Config();
    dwt_delay_init(SystemCoreClock);
}

VOID main_task(VOID)
{
    //extern at_adaptor_api at_interface;
    //at_api_register(&at_interface);
    
    extern bool_t  sim5320e_init(void);
    sim5320e_init();
    agent_tiny_entry();
}

UINT32 creat_main_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "main_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)main_task;
       
    task_init_param.uwStackSize = 0x2000;   //fota checksum need more stack here

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet){
        return uwRet;
    }
    return uwRet;
}


int main(void){
    UINT32 uwRet = LOS_OK;
	HardWare_Init();
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK){
        return LOS_NOK;
    } 
  
#if 0
    extern  UINT32 LOS_Inspect_Entry(VOID);
    LOS_Inspect_Entry();
#endif    
    //////////////////////APPLICATION INITIALIZE HERE/////////////////////
    //do the shell module initlialize:use uart 2
    extern void uart_debug_init(s32_t baud);
    uart_debug_init(115200);
    shell_install();
 
#if 1   
    //do the at module initialize:use uart 1
    extern bool_t uart_at_init(s32_t baudrate);
    extern s32_t uart_at_send(u8_t *buf, s32_t len,u32_t timeout);
    extern s32_t uart_at_receive(u8_t *buf,s32_t len,u32_t timeout);
    uart_at_init(115200);
    at_install(uart_at_receive,uart_at_send);
#endif

#if 0    
    extern bool_t  los_driv_module_init(void);
    los_driv_module_init();

#endif

 #if 1
	uwRet = creat_main_task();
    if (uwRet != LOS_OK){
        return LOS_NOK;
    }
 #endif 
    
    (void)LOS_Start();
    return 0;
}


