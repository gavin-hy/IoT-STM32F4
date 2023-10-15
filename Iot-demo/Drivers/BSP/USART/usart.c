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

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE �� stdio.h���涨��. */
FILE __stdout;

/* �ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
int fputc(int ch, FILE *f)
{    
    while ((USART1->SR & 0X40) == 0);               /* �ȴ���һ���ַ�������� */
    USART1->DR = (uint8_t)ch;                       /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
    return ch;
}
#endif
/***********************************************END*******************************************/
    







//uint8_t g_rx_buffer[RXBUFFERSIZE];                  /* HAL��ʹ�õĴ��ڽ��ջ��� */
//sBspUsart g_sBspUart1={
//    .pRxBuf = g_rx_buffer,
//};



UART_HandleTypeDef g_uart1_handle;                  /* UART��� */
//DMA_HandleTypeDef hdma_usart1_tx;                   /* DMA_TX��� */
//DMA_HandleTypeDef hdma_usart1_rx;                   /* DMA_RX��� */


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
 * @brief       ����X��ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @note        ע��: ����������ȷ��ʱ��Դ, ���򴮿ڲ����ʾͻ������쳣.
 *              �����USART��ʱ��Դ��sys_stm32_clock_init()�������Ѿ����ù���.
 * @retval      ��
 */
void usart_init(void)
{
    //Usart_DMA_Init();
    g_uart1_handle.Instance = USART1;                           /* USART1 */
    g_uart1_handle.Init.BaudRate = USART1_BAUD_RATE;            /* ������ */
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;        /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;             /* һ��ֹͣλ */
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;              /* ����żУ��λ */
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;        /* ��Ӳ������ */
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;                 /* �շ�ģʽ */
    HAL_UART_Init(&g_uart1_handle);                             /* HAL_UART_Init()��ʹ��UART1 */
    
    
    /* �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ��������������� */
    //__HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_IDLE);
    //HAL_UARTEx_ReceiveToIdle_DMA(&g_uart1_handle, g_sBspUart1.pRxBuf, RXBUFFERSIZE);
    //HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
    
}

/**
 * @brief       UART�ײ��ʼ������
 * @param       huart: UART�������ָ��
 * @note        �˺����ᱻHAL_UART_Init()����
 *              ���ʱ��ʹ�ܣ��������ã��ж�����
 * @retval      ��
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct={0};
    if(huart->Instance == USART1)                             /* ����Ǵ���1�����д���1 MSP��ʼ�� */
    {
        __HAL_RCC_USART1_CLK_ENABLE();                                  /* USART1 ʱ��ʹ�� */
        USART_TX_GPIO_CLK_ENABLE();                             /* ��������ʱ��ʹ�� */                            /* ��������ʱ��ʹ�� */

        gpio_init_struct.Pin = USART_TX_GPIO_PIN;               /* TX���� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ����������� */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� *///�������Ҫ���ر��Ƕ��ڽ�����˵
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
        gpio_init_struct.Alternate = USART_TX_GPIO_AF;          /* ����ΪUSART1 */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);   /* ��ʼ���������� */

        gpio_init_struct.Pin = USART_RX_GPIO_PIN;               /* RX���� */
        gpio_init_struct.Alternate = USART_RX_GPIO_AF;          /* ����ΪUSART1 */
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);   /* ��ʼ���������� */
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
        
        
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);              /* ��ռ���ȼ�3�������ȼ�3 */
        HAL_NVIC_EnableIRQ(USART1_IRQn);                      /* ʹ��USART1�ж�ͨ�� */
#endif        
        
    }
}


/**
 * @brief       Rx����ص�����
 * @param       huart: UART�������ָ��
 * @retval      ��
 */
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)             /* ����Ǵ���1 */
    {
        if((g_usart_rx_sta & 0x8000) == 0)      /* ����δ��� */
        {
            if(g_usart_rx_sta & 0x4000)         /* ���յ���0x0d */
            {
                if(g_rx_buffer[0] != 0x0a) 
                {
                    g_usart_rx_sta = 0;         /* ���մ���,���¿�ʼ */
                }
                else 
                {
                    g_usart_rx_sta |= 0x8000;   /* ��������� */
                }
            }
            else                                /* ��û�յ�0X0D */
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
                        g_usart_rx_sta = 0;     /* �������ݴ���,���¿�ʼ���� */
                    }
                }
            }
        }
    }
}



/**
 * @brief       ����1�жϷ�����
 * @param       ��
 * @retval      ��
 */
//���ڴ�����ɻص�����
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}
//���ڷ�������ص�����
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}

//���ڽ�����ɻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	g_sBspUart1.rxFlag = 1;
}

 void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
 {
    //HAL_UART_RECEPTION_TOIDLE ˵����DMA������ɣ����߰봫�����
    if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
    {
        
    }
  
     //HAL_UART_RECEPTION_STANDARD ˵���ǿ����жϴ����Ľ����¼�
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



