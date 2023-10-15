#include "usart.h"
#include <stdio.h>




#define USART_TX_GPIO_PORT              GPIOA
#define USART_TX_GPIO_PIN               GPIO_PIN_9
#define USART_TX_GPIO_AF                GPIO_AF7_USART1
#define USART_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   


#define USART_RX_GPIO_PORT              GPIOA
#define USART_RX_GPIO_PIN               GPIO_PIN_10
#define USART_RX_GPIO_AF                GPIO_AF7_USART1
#define USART_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  

// #define USART_UX                        USART1
// #define USART_UX_IRQn                   USART1_IRQn
// #define USART_UX_IRQHandler             USART1_IRQHandler
// #define USART_UX_CLK_ENABLE()           do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  



  



/******************************************************************************************/

#if 1
#if (__ARMCC_VERSION >= 6010050)                   
__asm(".global __use_no_semihosting\n\t");          
__asm(".global __ARM_use_no_argv \n\t");           

#else

#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{    
    while ((USART1->SR & 0X40) == 0);               /* 等待上一个字符发送完成 */
    USART1->DR = (uint8_t)ch;                       /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/***********************************************END*******************************************/
    







//uint8_t g_rx_buffer[RXBUFFERSIZE];                  /* HAL库使用的串口接收缓冲 */
//sBspUsart g_sBspUart1={
//    .pRxBuf = g_rx_buffer,
//};



UART_HandleTypeDef g_uart1_handle;                  /* UART句柄 */
//DMA_HandleTypeDef hdma_usart1_tx;                   /* DMA_TX句柄 */
//DMA_HandleTypeDef hdma_usart1_rx;                   /* DMA_RX句柄 */


/**
  * Enable DMA controller clock
  */
//void Usart_DMA_Init(void)
//{
//    /* DMA controller clock enable */
//  __HAL_RCC_DMA2_CLK_ENABLE();

//  /* DMA interrupt init */
//  /* DMA2_Stream2_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 3, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
//  /* DMA2_Stream7_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 3, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
//}

/**
 * @brief       串口X初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @note        注意: 必须设置正确的时钟源, 否则串口波特率就会设置异常.
 *              这里的USART的时钟源在sys_stm32_clock_init()函数中已经设置过了.
 * @retval      无
 */
void usart_init(void)
{
    //Usart_DMA_Init();
    g_uart1_handle.Instance = USART1;                           /* USART1 */
    g_uart1_handle.Init.BaudRate = USART1_BAUD_RATE;            /* 波特率 */
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;        /* 字长为8位数据格式 */
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;             /* 一个停止位 */
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;              /* 无奇偶校验位 */
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;        /* 无硬件流控 */
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;                 /* 收发模式 */
    HAL_UART_Init(&g_uart1_handle);                             /* HAL_UART_Init()会使能UART1 */
    
    
    /* 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    //__HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_IDLE);
    //HAL_UARTEx_ReceiveToIdle_DMA(&g_uart1_handle, g_sBspUart1.pRxBuf, RXBUFFERSIZE);
    //HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
    
}

/**
 * @brief       UART底层初始化函数
 * @param       huart: UART句柄类型指针
 * @note        此函数会被HAL_UART_Init()调用
 *              完成时钟使能，引脚配置，中断配置
 * @retval      无
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct={0};
    if(huart->Instance == USART1)                             /* 如果是串口1，进行串口1 MSP初始化 */
    {
        __HAL_RCC_USART1_CLK_ENABLE();                                  /* USART1 时钟使能 */
        USART_TX_GPIO_CLK_ENABLE();                             /* 发送引脚时钟使能 */                            /* 接收引脚时钟使能 */

        gpio_init_struct.Pin = USART_TX_GPIO_PIN;               /* TX引脚 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 *///这里很重要，特别是对于接受来说
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* 高速 */
        gpio_init_struct.Alternate = USART_TX_GPIO_AF;          /* 复用为USART1 */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);   /* 初始化发送引脚 */

        gpio_init_struct.Pin = USART_RX_GPIO_PIN;               /* RX引脚 */
        gpio_init_struct.Alternate = USART_RX_GPIO_AF;          /* 复用为USART1 */
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);   /* 初始化接收引脚 */
#if 0
        /* USART1 DMA Init */
        /* USART1_TX Init */
        hdma_usart1_tx.Instance = DMA2_Stream7;
        hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
        hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart1_tx.Init.Mode = DMA_NORMAL;
        hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_usart1_tx);

        __HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);//

        /* USART1_RX Init */
        hdma_usart1_rx.Instance = DMA2_Stream2;
        hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart1_rx.Init.Mode = DMA_NORMAL;
        hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_usart1_rx);

        __HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);//
        
        
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);              /* 抢占优先级3，子优先级3 */
        HAL_NVIC_EnableIRQ(USART1_IRQn);                      /* 使能USART1中断通道 */
#endif        
        
    }
}


/**
 * @brief       Rx传输回调函数
 * @param       huart: UART句柄类型指针
 * @retval      无
 */
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)             /* 如果是串口1 */
    {
        if((g_usart_rx_sta & 0x8000) == 0)      /* 接收未完成 */
        {
            if(g_usart_rx_sta & 0x4000)         /* 接收到了0x0d */
            {
                if(g_rx_buffer[0] != 0x0a) 
                {
                    g_usart_rx_sta = 0;         /* 接收错误,重新开始 */
                }
                else 
                {
                    g_usart_rx_sta |= 0x8000;   /* 接收完成了 */
                }
            }
            else                                /* 还没收到0X0D */
            {
                if(g_rx_buffer[0] == 0x0d)
                {
                    g_usart_rx_sta |= 0x4000;
                }
                else
                {
                    g_usart_rx_buf[g_usart_rx_sta & 0X3FFF] = g_rx_buffer[0] ;
                    g_usart_rx_sta++;
                    if(g_usart_rx_sta > (USART_REC_LEN - 1))
                    {
                        g_usart_rx_sta = 0;     /* 接收数据错误,重新开始接收 */
                    }
                }
            }
        }
    }
}



/**
 * @brief       串口1中断服务函数
 * @param       无
 * @retval      无
 */
//串口传输完成回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}
//串口发生错误回调函数
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}

//串口接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	g_sBspUart1.rxFlag = 1;
}

 void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
 {
    //HAL_UART_RECEPTION_TOIDLE 说明是DMA接收完成，或者半传输完成
    if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
    {
        
    }
  
     //HAL_UART_RECEPTION_STANDARD 说明是空闲中断触发的接收事件
     if (huart->ReceptionType == HAL_UART_RECEPTION_STANDARD)
     {
         g_sBspUart1.rxFlag = 1;
         g_sBspUart1.rxLen  = Size;
     }
 }



 

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&g_uart1_handle);
}


/* Rx DMA */
void DMA2_Stream2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
}


/* Tx DMA */ 
void DMA2_Stream7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
}
#endif



