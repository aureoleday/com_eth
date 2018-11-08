#define _WATCHDOG_H
#include "drv_wtd.h"
#include "stm32f4xx.h"

// mode  0--����״̬�µ�ʱ��   1--sleep �µ�ʱ��
void watchdog_init(void)
{    
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    /* IWDG counter clock: 40KHz(LSI)       128��Ƶ һ������3.2ms*/
    IWDG_SetPrescaler(IWDG_Prescaler_128);
    /* Set counter reload value to 300  1s */
    IWDG_SetReload(800);   //�12λ [0,4096] 800*3.2=2.56S 
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
     /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}

void dog(void)
{
    IWDG_ReloadCounter();
}
