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

#include <string.h>
#include <stdlib.h>
#include "hal_flash.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"

#include <osport.h>

#ifdef HAL_FLASH_MODULE_ENABLED

#define FLASH_SECTOR_ILEGAL      0xFFFFFFFF
#define ADDR_FLASH_SECTOR_0      ((u32_t)0x08000000) /* Base address of Sector 0, 2 Kbytes   */
#define ADDR_FLASH_SECTOR_END    ((u32_t)0x08080000) /* End address of Sector 256 */


//F44RE:FLASH ARE ORGNAZIED BY 16 16 16 16 64 128 128 128 ==512KB could only be erased by SECTOR OR FULL CHIP
#define cn_flash_baseaddr     0x08000000
#define cn_flash_kbyte        1024
#define cn_flash_sector_num   8
static u32_t  s_flash_secsize_tab[cn_flash_sector_num] = {16*cn_flash_kbyte,16*cn_flash_kbyte,16*cn_flash_kbyte,16*cn_flash_kbyte,\
                                       64*cn_flash_kbyte,128*cn_flash_kbyte,128*cn_flash_kbyte,128*cn_flash_kbyte};

                                       

static bool_t __flash_addr2sector(u32_t addr, u32_t *sector)
{
    bool_t ret = false;
    
    s32_t i =0;
    u32_t baseedge;
      
    if(addr < cn_flash_baseaddr)
    {
        return ret;
    }
    
    baseedge = cn_flash_baseaddr;
    for (i =0;i< cn_flash_sector_num;i++)
    {
        baseedge +=  s_flash_secsize_tab[i];
        if(addr < baseedge)
        {
            *sector = i;
            ret = true;
            break;   
        }
    }
    return ret;
}

int hal_flash_read(void* buf, s32_t len, u32_t location)
{
    u32_t  addr_start;
    u32_t  addr_end;
    u32_t  sector;   
    if (NULL == buf )
    {
        return -1;
    }
    
    addr_start = location;
    addr_end = location + len -1;
    if(__flash_addr2sector(addr_start,&sector) && __flash_addr2sector(addr_end,&sector))
    {
        memcpy(buf, (uint8_t*)location, len);
        return 0;
    }
    else
    {
        return -1;
    }
}

int hal_flash_erase(u32_t addr, s32_t len)
{
    s32_t ret = -1;
    u32_t begin_sector;
    u32_t end_sector;
    u32_t i;
    u32_t sector_err;
    
    
    FLASH_EraseInitTypeDef para;
    
    if(false == __flash_addr2sector(addr,&begin_sector))
    {
        return ret;
    }
    if(false == __flash_addr2sector(addr + len -1,&end_sector))
    {
        return ret;
    }

    HAL_FLASH_Unlock();

    para.TypeErase = FLASH_TYPEERASE_SECTORS;
    para.Banks = 0;
    para.Sector = begin_sector;
    para.NbSectors = end_sector - begin_sector +1;
    para.VoltageRange = FLASH_VOLTAGE_RANGE_1; 
    if(HAL_OK == HAL_FLASHEx_Erase(&para, &sector_err))
    {
        ret = 0;
    }
    
    HAL_FLASH_Lock();
      
    return ret;
}

HAL_StatusTypeDef HAL_FLASH_Program(u32_t TypeProgram, u32_t Address, uint64_t Data);
int hal_flash_write(const void* buf, s32_t len, u32_t* location)
{
    uint8_t* pbuf;
    u32_t location_cur;
    u32_t addr;
    s32_t ret = -1;
    u32_t begin_sector;
    u32_t end_sector;
    u32_t i;
    u32_t sector_err;
       
    FLASH_EraseInitTypeDef flashret;
    FLASH_EraseInitTypeDef para;
    
    
    addr = *location;
    pbuf = (u8_t *)buf;
    
    if(false == __flash_addr2sector(addr,&begin_sector))
    {
        return ret;
    }
    if(false == __flash_addr2sector(addr + len -1,&end_sector))
    {
        return ret;
    }

    HAL_FLASH_Unlock();
    
    for (i =0;i<len; i++)
    {
        if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,addr,(uint64_t )(*pbuf)))
        {
            break;
        }
        addr++;
        pbuf++;
    }
    HAL_FLASH_Lock();
    
    if(i == len)
    {
        ret = 0;
    }
    return ret;
}

int hal_flash_erase_write(const void* buf, s32_t len, u32_t location)
{
    if (NULL == buf)
    {
        return -1;
    }

    if (hal_flash_erase(location, len) != 0)
    {
        return -1;
    }

    if (hal_flash_write(buf, len, &location) != 0)
    {
        return -1;
    }

    return 0;
}

void hal_flash_lock(void)
{
    (void)HAL_FLASH_Lock();
}
#include <shell.h>
//here we add some shell command to test the flash driver  //flash_read  addr len
static s32_t shell_flash_read(s32_t argc,const char *argv[])
{
    u8_t *buf;
    s32_t len;
    u32_t position;
    if(argc != 3)
    {
        printf("paraerr\r\n");
        return 0;
    }
    
    len = strtoul(argv[2],NULL,0);
    buf = malloc(len);
    if(NULL == buf)
    {
        printf("mem err\r\n");
        return 0;
    }
    
    position = strtoul(argv[1],NULL,0);
    if(0 == hal_flash_read(buf,len,position))
    {
        s32_t i = 0;
        for (i =0;i<len;i++)
        {
            if(0 == i%16)
            {
                printf("\n\r");
            }
            printf("0x%02x ",buf[i]); 
        }
        printf("\n\r");
    }
    else
    {
        printf("flash read err\r\n");
    
    }
    
    free(buf);
    return 0;
}
OSSHELL_EXPORT_CMD(shell_flash_read,"flashread","flashread addr len");

//here we add some shell command to test the flash driver  //flash_read  addr len
static s32_t shell_flash_ewrite(s32_t argc,const char *argv[])
{
    u32_t position;
    if(argc != 3)
    {
        printf("paraerr\r\n");
        return 0;
    }
       
    position = strtoul(argv[1],NULL,0);
    
    
    if(0 != hal_flash_erase_write((const void *)argv[2],strlen(argv[2]),position))
    {
        printf("erase write error\n\r");
    }
    else
    {
        printf("erase write OK\n\r");
    }

    return 0;
}
OSSHELL_EXPORT_CMD(shell_flash_ewrite,"flashwrite","flashwrite addr data");






#endif /* HAL_FLASH_MODULE_ENABLED */