/**
 ****************************************************************************************************
 * @file        key.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       �������� ��������
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

#include "key.h"
#include "stm32f4xx_hal.h"
/******************************************************************************************/
/* ���� ���� */

#define KEY0_GPIO_PORT                  GPIOE
#define KEY0_GPIO_PIN                   GPIO_PIN_4
#define KEY0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

#define KEY1_GPIO_PORT                  GPIOE
#define KEY1_GPIO_PIN                   GPIO_PIN_3
#define KEY1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

#define KEY2_GPIO_PORT                  GPIOE
#define KEY2_GPIO_PIN                   GPIO_PIN_2
#define KEY2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

#define WKUP_GPIO_PORT                  GPIOA
#define WKUP_GPIO_PIN                   GPIO_PIN_0
#define WKUP_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

/******************************************************************************************/

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN)     /* ��ȡKEY0���� */
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)     /* ��ȡKEY1���� */
#define KEY2        HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN)     /* ��ȡKEY2���� */
#define WK_UP       HAL_GPIO_ReadPin(WKUP_GPIO_PORT, WKUP_GPIO_PIN)     /* ��ȡWKUP���� */


#define KEY0_PRES    1              /* KEY0���� */
#define KEY1_PRES    2              /* KEY1���� */
#define KEY2_PRES    3              /* KEY2���� */
#define WKUP_PRES    4              /* KEY_UP����(��WK_UP) */


/**
 * @brief       ������ʼ������
 * @param       ��
 * @retval      ��
 */
void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ò����洢���� */
    KEY0_GPIO_CLK_ENABLE();                                     /* KEY0ʱ��ʹ�� */
    KEY1_GPIO_CLK_ENABLE();                                     /* KEY1ʱ��ʹ�� */
    KEY2_GPIO_CLK_ENABLE();                                     /* KEY2ʱ��ʹ�� */
    WKUP_GPIO_CLK_ENABLE();                                     /* WKUPʱ��ʹ�� */

    gpio_init_struct.Pin = KEY0_GPIO_PIN;                       /* KEY0���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);           /* KEY0����ģʽ����,�������� */

    gpio_init_struct.Pin = KEY1_GPIO_PIN;                       /* KEY1���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);           /* KEY1����ģʽ����,�������� */

    gpio_init_struct.Pin = KEY2_GPIO_PIN;                       /* KEY2���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY2_GPIO_PORT, &gpio_init_struct);           /* KEY2����ģʽ����,�������� */

    gpio_init_struct.Pin = WKUP_GPIO_PIN;                       /* WKUP���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                      /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);           /* WKUP����ģʽ����,�������� */

}

/**
 * @brief       ����ɨ�躯��
 * @note        �ú�������Ӧ���ȼ�(ͬʱ���¶������): WK_UP > KEY2 > KEY1 > KEY0!!
 * @param       mode:0 / 1, ���庬������:
 *   @arg       0,  ��֧��������(���������²���ʱ, ֻ�е�һ�ε��û᷵�ؼ�ֵ,
 *                  �����ɿ��Ժ�, �ٴΰ��²Ż᷵��������ֵ)
 *   @arg       1,  ֧��������(���������²���ʱ, ÿ�ε��øú������᷵�ؼ�ֵ)
 * @retval      ��ֵ, ��������:
 *              KEY0_PRES, 1, KEY0����
 *              KEY1_PRES, 2, KEY1����
 *              KEY2_PRES, 3, KEY2����
 *              WKUP_PRES, 4, WKUP����
 */
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;  /* �������ɿ���־ */
    uint8_t keyval = 0;

    if (mode) key_up = 1;       /* ֧������ */

    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || WK_UP == 1))  /* �����ɿ���־Ϊ1, ��������һ������������ */
    {
        //delay_ms(10);           /* ȥ���� */
        key_up = 0;

        if (KEY0 == 0)  keyval = KEY0_PRES;

        if (KEY1 == 0)  keyval = KEY1_PRES;

        if (KEY2 == 0)  keyval = KEY2_PRES;

        if (WK_UP == 1) keyval = WKUP_PRES;
    }
    else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && WK_UP == 0)         /* û���κΰ�������, ��ǰ����ɿ� */
    {
        key_up = 1;
    }

    return keyval;              /* ���ؼ�ֵ */
}




















