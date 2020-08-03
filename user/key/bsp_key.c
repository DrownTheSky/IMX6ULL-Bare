#include "bsp_key.h"

void key_init(void)
{	
	/* GPIO时钟 */
	CCM->CCGR1 &= ~(3 << 30);
	CCM->CCGR1 |= (3 << 30);
	CCM->CCGR3 &= ~(3 << 12);
	CCM->CCGR3 |= (3 << 12);

	/* 复用和电气属性 */
	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0);
	IOMUXC_SetPinMux(IOMUXC_NAND_CE1_B_GPIO4_IO14,0);
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0xF080);
	IOMUXC_SetPinConfig(IOMUXC_NAND_CE1_B_GPIO4_IO14,0xF080);

	/* 方向 */
	GPIO5->GDIR &= ~(1 << 1);
	GPIO4->GDIR &= ~(1 << 14);
	/* 高低电平 */
	GPIO5->DR |= (1 << 3);
	GPIO4->DR |= (1 << 14);

	/* 设置为上升沿触发 */
	GPIO4->ICR1 &= ~(3 << 28);
	GPIO4->ICR1 |= (2 << 28);

	/* 开启中断 */
	GPIO4->IMR |= (1 << 14);
}

uint8_t KEY1_Scan(void)
{
	if((GPIO5->DR & 0x02) == 0)
	{
		while((GPIO5->DR & 0x02) == 1);
		return KEY_ON;
    }
	else
    {
        return KEY_OFF;
    }
}

uint8_t KEY2_Scan(void)
{
	if((GPIO4->DR & 0x4000) == 0)
	{
		while((GPIO4->DR & 0x4000) == 1);
		return KEY_ON;
    }
	else
    {
        return KEY_OFF;
    }
}