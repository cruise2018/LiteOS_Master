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

#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

// SPI flash address
//#define OTA_FLAG_ADDR1                0x00000000
//#define OTA_FLAG_ADDR2                0x00004000
//#define UPDATE_INFO_ADDR              0x00008000
//#define UPDATE_INFO_SIZE              0x00008000
//#define OTA_IMAGE_DOWNLOAD_ADDR       (UPDATE_INFO_ADDR + UPDATE_INFO_SIZE)
//#define OTA_IMAGE_DOWNLOAD_SIZE       0x00040000
//#define OTA_IMAGE_BCK_ADDR            (OTA_IMAGE_DOWNLOAD_ADDR + OTA_IMAGE_DOWNLOAD_SIZE)

#define OTA_FLAG_ADDR1                (0x08008000)
#define OTA_FLAG_ADDR2                (0x0800C000)

//#define UPDATE_INFO_SIZE              0x00008000
//#define UPDATE_INFO_ADDR              (OTA_FLAG_ADDR1 - UPDATE_INFO_SIZE)
#define OTA_IMAGE_DOWNLOAD_ADDR       0x08040000
#define OTA_IMAGE_DOWNLOAD_SIZE       0x00020000
#define OTA_IMAGE_BCK_ADDR            0x08060000


// Built in flash address
#define OTA_DEFAULT_IMAGE_ADDR        0x08020000

int board_jump2app(void);
int board_update_copy(int32_t old_image_len, int32_t new_image_len,
                      void (*func_get_update_record)(uint8_t* state, uint32_t* offset),
                      int (*func_set_update_record)(uint8_t state, uint32_t offset));
int board_rollback_copy(int32_t image_len,
                        void (*func_get_update_record)(uint8_t* state, uint32_t* offset),
                        int (*func_set_update_record)(uint8_t state, uint32_t offset));

#if defined(__cplusplus)
}
#endif

#endif /* _BOARD_H_*/