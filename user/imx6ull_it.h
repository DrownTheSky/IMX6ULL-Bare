#ifndef __IMX6ULL_INT_H
#define __IMX6ULL_INT_H

#include "../imx6ull/MCIMX6Y2.h"
#include "../imx6ull/fsl_iomuxc.h"
#include "../imx6ull/core_ca7.h"
#include "./led/bsp_led.h"

/* 中断处理函数形式 */
typedef void (*system_irq_handler)(uint16_t GiccIar, void *Param);

/* 中断处理函数结构体 */
typedef struct irq_handler
{
    system_irq_handler Irq_Handler;
    void *Param;    // 用户参数，可不用
}sys_irq_handler;

void IntInit(void);
void gpio4_io0_15_exit(uint16_t GiccIar, void *Param);

#endif  /* __IMX6ULL_INT_H */