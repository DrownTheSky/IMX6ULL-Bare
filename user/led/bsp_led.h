#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "../../imx6ull/MCIMX6Y2.h"
#include "../../imx6ull/fsl_iomuxc.h"

#define LED_ON 1
#define LED_OFF 0

/* GPIO5_IO03 */
// #define CCM_CCGR1 *((volatile unsigned int *)(0x020C406C))//时钟
// #define MUX_CTL *((volatile unsigned int *)(0x02290014))//模式
// #define PAD_CTL *((volatile unsigned int *)(0x02290058))//属性
// #define GPIO5_GDIR *((volatile unsigned int *)(0x020AC004))//方向
// #define GPIO5_DR *((volatile unsigned int *)(0x020AC000))//读写

void led_init(void);
void led_ctl(uint8_t value);
void led_tog(void);

#endif /* __BSP_LED_H */