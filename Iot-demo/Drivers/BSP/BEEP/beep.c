
#include "beep.h"
#include "stm32f4xx_hal.h"

/******************************************************************************************/
/* 引脚 定义 */

#define BEEP_GPIO_PORT                  GPIOF
#define BEEP_GPIO_PIN                   GPIO_PIN_8
#define BEEP_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)         /* PF口时钟使能 */

/******************************************************************************************/

/* 蜂鸣器控制 */
#define BEEP(x)         do{ x ? \
                            HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN, GPIO_PIN_SET) : \
                            HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

/* BEEP状态翻转 */
#define BEEP_TOGGLE()   do{ HAL_GPIO_TogglePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN); }while(0)    /* BEEP = !BEEP */



/**
 * @brief       初始化BEEP相关IO口, 并使能时钟
 * @param       无
 * @retval      无
 */
void beep_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    BEEP_GPIO_CLK_ENABLE();                             /* BEEP时钟使能 */

    gpio_init_struct.Pin = BEEP_GPIO_PIN;               /* 蜂鸣器引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* 高速 */
    HAL_GPIO_Init(BEEP_GPIO_PORT, &gpio_init_struct);   /* 初始化蜂鸣器引脚 */

    BEEP(0);                                            /* 关闭蜂鸣器 */
}






