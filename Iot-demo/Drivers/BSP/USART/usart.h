/**
 ****************************************************************************************************
 * @file        usart.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       ���ڳ�ʼ������(һ���Ǵ���1)��֧��printf
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211014
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef _USART_H
#define _USART_H

#include <stdint.h>
#include "stm32f4xx_hal.h"


extern UART_HandleTypeDef g_uart1_handle;                  /* UART��� */


#define  USART1_BAUD_RATE   115200

/* �����������ֽ��� 200 */
//#define RXBUFFERSIZE    1024                    /* �����С */


//extern uint16_t g_usart_rx_sta;                 /* ����״̬��� */


//typedef struct 
//{
//    uint8_t *pRxBuf;
//    uint8_t rxFlag;
//    uint16_t rxLen;
//}sBspUsart;

//extern sBspUsart g_sBspUart1;
void usart_init(void);             /* ���ڳ�ʼ������ */

#endif







