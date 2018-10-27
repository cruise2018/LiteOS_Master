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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "hal_flash.h"
//#include "hal_spi_flash.h"
#include "ota.h"
#include "board.h"

#ifdef USE_IWDG
#include "hal_iwdg.h"
#endif

void SysTick_Handler(void)
{
    HAL_IncTick();
}




int fota_main(void)
{
    int ret;
    ota_assist assist;

    printf("bootloader begin\n");

    assist.func_printf = printf;
    assist.func_ota_read = hal_flash_read;
    assist.func_ota_write = hal_flash_erase_write;
    ota_register_assist(&assist);

    ret = ota_init();
    if (ret < 0)
    {
        OTA_LOG("read/write boot information failed");
    }
    ret = ota_update_process();
    if (ret < 0)
    {
        OTA_LOG("update process failed, try to start up with origin image now");
    }

#ifdef USE_IWDG
    // feed interval: 10s
    hal_iwdg_config(IWDG_PRESCALER_64, 6250);
#endif

    OTA_LOG("begin to jump to application");
    ret = ota_jump_to_application();
    if (ret < 0)
    {
        OTA_LOG("start up image failed, try to roll back now");
    }
    ret = ota_roll_back_image();
    if (ret < 0)
    {
        OTA_LOG("roll back image failed, system start up failed");
        _Error_Handler(__FILE__, __LINE__);
    }
    ret = ota_jump_to_application();
    if (ret < 0)
    {
        OTA_LOG("start up roll back image failed, system start up failed");
        _Error_Handler(__FILE__, __LINE__);
    }
    return ret;
}
