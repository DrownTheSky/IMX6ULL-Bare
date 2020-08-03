#include "imx6ull_it.h"

/* 中断嵌套控制器 */
static uint16_t IrqNesting = 0;

/* 中断服务函数表 */
static sys_irq_handler IrqTable[NUMBER_OF_INT_VECTORS];

/* 默认中断函数 */
static void default_irq_handler(uint16_t GiccIar, void *Param)
{
    return;
}

/* 给指定的中断号注册中断服务函数 */
static void system_register_irq_handler(IRQn_Type irq, system_irq_handler handler, void *param)
{
    IrqTable[irq].Irq_Handler = handler;
    IrqTable[irq].Param = param;
}

/* 中断初始化 */
void IntInit(void)
{
    GIC_Init(); // 初始化中断控制器

    for (uint16_t i = 0; i < NUMBER_OF_INT_VECTORS; i++)// 将所有的中断服务函数设置为默认值
    {
        system_register_irq_handler((IRQn_Type)i, default_irq_handler, NULL);
    }
    __set_VBAR((uint32_t)0x87800000); // 设置中断向量表偏移

    /* GPIO4-IO14 */
    GIC_EnableIRQ(GPIO4_Combined_0_15_IRQn);
    system_register_irq_handler(GPIO4_Combined_0_15_IRQn, gpio4_io0_15_exit, NULL);

}

/* IRQ汇编中断函数会调用此函数 */
void IMX6ULL_IRQ_Handler(uint32_t GiccIar)
{
    uint16_t IntNum = GiccIar & 0x3FF;
    if(IntNum >= NUMBER_OF_INT_VECTORS)
    {
        return;
    }

    IrqNesting++;// 产生中断，中断嵌套控制器递增

    /* 根据传递进来的中断号，在 IrqTable 中调用确定的中断服务函数 */
    IrqTable[IntNum].Irq_Handler(IntNum, IrqTable[IntNum].Param);

    IrqNesting--;// 完成中断，中断嵌套控制器递减
}

/* GPIO4-IO14中断服务函数 */
void gpio4_io0_15_exit(uint16_t GiccIar, void *Param)
{
    led_tog();

    /* 清除中断标志位(写一清除) */
    GPIO4->ISR |= (1 << 14);
}
