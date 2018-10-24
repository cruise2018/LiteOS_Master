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

/*
#include "at_hal.h"
#include "stm32f1xx_hal.h"

static UART_HandleTypeDef at_usart;
static USART_TypeDef* s_pUSART = USART2;
static uint32_t       s_uwIRQn = USART2_IRQn;

#define CN_RCVBUF_LEN  256
static  uint32_t       s_rcv_len =0;
static  uint32_t       s_rcv_off = 0;
static  uint8_t        s_rcv_buf[CN_RCVBUF_LEN];
static  uint32_t       s_rcv_sync;
//uint32_t list_mux;
static void atuart_config(uint32_t port)
{
    switch ( port )
    {
        case 1 :
            s_pUSART = USART1;
            s_uwIRQn = USART1_IRQn;
            break;
        case 2 :
            s_pUSART = USART2;
            s_uwIRQn = USART2_IRQn;
            break;
        case 3 :
            s_pUSART = USART3;
            s_uwIRQn = USART3_IRQn;
            break;
        default:
            break;
    }
}
void atuart_irq(void)
{
    uint8_t value;
    if(__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_RXNE) != RESET)
    {
       value = (uint8_t)(at_usart.Instance->DR & 0x00FF);
       if( s_rcv_len != 0)
       {
       }
       else
       {
            s_rcv_buf[s_rcv_off++] = value;
       }
    }
    else if (__HAL_UART_GET_FLAG(&at_usart,UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&at_usart);
        if(s_rcv_len != 0) //should use buffer tab instead
        {
        }
        else
        {
            s_rcv_len = s_rcv_off;
            s_rcv_off = 0;
        }
        (void)LOS_SemPost(s_rcv_sync);
    }
}

int32_t atuart_init(void)
{
	UART_HandleTypeDef * usart = &at_usart;

    //atuart_config(at_user_conf.usart_port);
    
    if(LOS_OK != LOS_SemCreate(0,&s_rcv_sync))
    {
        printf("semp error\n\r");
        return -1;
    }

    usart->Instance = s_pUSART;
    usart->Init.BaudRate = 115200;

    usart->Init.WordLength = UART_WORDLENGTH_8B;
    usart->Init.StopBits = UART_STOPBITS_1;
    usart->Init.Parity = UART_PARITY_NONE;
    usart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart->Init.Mode = UART_MODE_RX | UART_MODE_TX;
    if(HAL_UART_Init(usart) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    __HAL_UART_CLEAR_FLAG(usart,UART_FLAG_TC);
    LOS_HwiCreate(s_uwIRQn, 0, 0, atuart_irq, 0);
    __HAL_UART_ENABLE_IT(usart, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(usart, UART_IT_RXNE);

    return AT_OK;
}

void atuart_deinit(void)
{
    UART_HandleTypeDef * husart = &at_usart;
    __HAL_UART_DISABLE(husart);
    __HAL_UART_DISABLE_IT(husart, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(husart, UART_IT_RXNE);
}
int32_t atuart_snd(uint8_t *buf, int32_t len,uint32_t timeout)
{
    HAL_UART_Transmit(&at_usart,buf,len,timeout);
    return len;
}
int32_t atuart_rcv(uint8_t *buf,int32_t len,uint32_t timeout)
{
    int32_t cpylen;
    int32_t ret = 0;
    UINTPTR lock;
    if(LOS_OK == LOS_SemPend(s_rcv_sync,timeout))
    {
        lock = LOS_IntLock();
        cpylen = len > s_rcv_len?s_rcv_len:len;
        memcpy(buf,s_rcv_buf,cpylen);
        s_rcv_len = 0;
        ret = cpylen;
        LOS_IntRestore(lock);
    }
    return ret;
}
*/
