#include "imx6ull_it.h"

/* �ж�Ƕ�׿����� */
static uint16_t IrqNesting = 0;

/* �жϷ������� */
static sys_irq_handler IrqTable[NUMBER_OF_INT_VECTORS];

/* Ĭ���жϺ��� */
static void default_irq_handler(uint16_t GiccIar, void *Param)
{
    return;
}

/* ��ָ�����жϺ�ע���жϷ����� */
static void system_register_irq_handler(IRQn_Type irq, system_irq_handler handler, void *param)
{
    IrqTable[irq].Irq_Handler = handler;
    IrqTable[irq].Param = param;
}

/* �жϳ�ʼ�� */
void IntInit(void)
{
    GIC_Init(); // ��ʼ���жϿ�����

    for (uint16_t i = 0; i < NUMBER_OF_INT_VECTORS; i++)// �����е��жϷ���������ΪĬ��ֵ
    {
        system_register_irq_handler((IRQn_Type)i, default_irq_handler, NULL);
    }
    __set_VBAR((uint32_t)0x87800000); // �����ж�������ƫ��

    /* GPIO4-IO14 */
    GIC_EnableIRQ(GPIO4_Combined_0_15_IRQn);
    system_register_irq_handler(GPIO4_Combined_0_15_IRQn, gpio4_io0_15_exit, NULL);

}

/* IRQ����жϺ�������ô˺��� */
void IMX6ULL_IRQ_Handler(uint32_t GiccIar)
{
    uint16_t IntNum = GiccIar & 0x3FF;
    if(IntNum >= NUMBER_OF_INT_VECTORS)
    {
        return;
    }

    IrqNesting++;// �����жϣ��ж�Ƕ�׿���������

    /* ���ݴ��ݽ������жϺţ��� IrqTable �е���ȷ�����жϷ����� */
    IrqTable[IntNum].Irq_Handler(IntNum, IrqTable[IntNum].Param);

    IrqNesting--;// ����жϣ��ж�Ƕ�׿������ݼ�
}

/* GPIO4-IO14�жϷ����� */
void gpio4_io0_15_exit(uint16_t GiccIar, void *Param)
{
    led_tog();

    /* ����жϱ�־λ(дһ���) */
    GPIO4->ISR |= (1 << 14);
}
