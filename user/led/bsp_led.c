#include "bsp_led.h"

void led_init(void)
{
/* 
 * 1.使能GPIO5时钟
 * bit[31:30] = 0b11
 */
CCM->CCGR1 &= ~(3 << 30);
CCM->CCGR1 |= (3 << 30);

/* 
 * 2.设置GPIO5_IO03为GPIO模式
 * bit[3:0] = 0b0101
 */
IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER3_GPIO5_IO03,0);
/* 
 * 3.设置GPIO5_IO03的电气属性
 */
IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER3_GPIO5_IO03,0x10B0);

/* 
 * 4.设置GPIO5_IO03方向
 * bit[3] = 0(IN) or 1(OUT)
 */
GPIO5->GDIR &= ~(1 << 3);
GPIO5->GDIR |= (1 << 3);

/* 
 * 5.设置GPIO5_IO03读写
 * bit[3] = 0 or 1
 */
GPIO5->DR &= ~(1 << 3);
GPIO5->DR |= (1 << 3);
}

void led_ctl(uint8_t value)
{
    if (value)
    {
        GPIO5->DR &= ~(1 << 3);
    }
    else
    {
        GPIO5->DR |= (1 << 3);
    } 
}

void led_tog(void)
{
    GPIO5->DR ^= (1 << 3);
}