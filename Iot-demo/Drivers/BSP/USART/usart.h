/**
 ****************************************************************************************************
 * @file        usart.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       串口初始化代码(一般是串口1)，支持printf
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef _USART_H
#define _USART_H

#include <stdint.h>
#include "stm32f4xx_hal.h"


extern UART_HandleTypeDef g_uart1_handle;                  /* UART句柄 */


#define  USART1_BAUD_RATE   115200

/* 定义最大接收字节数 200 */
//#define RXBUFFERSIZE    1024                    /* 缓存大小 */


//extern uint16_t g_usart_rx_sta;                 /* 接收状态标记 */


//typedef struct 
//{
//    uint8_t *pRxBuf;
//    uint8_t rxFlag;
//    uint16_t rxLen;
//}sBspUsart;

//extern sBspUsart g_sBspUart1;
void usart_init(void);             /* 串口初始化函数 */

#endif







