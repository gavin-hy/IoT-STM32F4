
#include "beep.h"
#include "stm32f4xx_hal.h"

/******************************************************************************************/
/* ���� ���� */

#define BEEP_GPIO_PORT                  GPIOF
#define BEEP_GPIO_PIN                   GPIO_PIN_8
#define BEEP_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)         /* PF��ʱ��ʹ�� */

/******************************************************************************************/

/* ���������� */
#define BEEP(x)         do{ x ? \
                            HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN, GPIO_PIN_SET) : \
                            HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

/* BEEP״̬��ת */
#define BEEP_TOGGLE()   do{ HAL_GPIO_TogglePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN); }while(0)    /* BEEP = !BEEP */



/**
 * @brief       ��ʼ��BEEP���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��
 */
void beep_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    BEEP_GPIO_CLK_ENABLE();                             /* BEEPʱ��ʹ�� */

    gpio_init_struct.Pin = BEEP_GPIO_PIN;               /* ���������� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
    HAL_GPIO_Init(BEEP_GPIO_PORT, &gpio_init_struct);   /* ��ʼ������������ */

    BEEP(0);                                            /* �رշ����� */
}






