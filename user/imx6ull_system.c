#include "imx6ull_system.h"

void SysClkInit(void)
{
    uint32_t Value = 0; // 配置 PLL2_PFDx 时直接复位寄存器会发生错误

    CCM->CCSR &= ~(1 << 8); // 配置 step_clk 使用24M晶振
    CCM->CCSR |= (1 << 2);  // 暂时使用 step_clk 以便配置 pll1_main_clk

    /* PLL1(ARM 650M ~ 1.3G)= Fref * DIV_SEL / 2 */
    /*            1056M =  24M *      88 / 2 */
    CCM_ANALOG->PLL_ARM |= (1 << 13); // 使能PLL
    CCM_ANALOG->PLL_ARM &= ~(0x7F << 0);
    CCM_ANALOG->PLL_ARM |= (88 << 0); // 写入倍频值

    /* ARM_CLK_ROOT = PLL1  / divide */
    /*         528M = 1056M /      2 */
    CCM->CACRR &= ~(7 << 0);
    CCM->CACRR |= (1 << 0); // 设置为 2 分频
    CCM->CCSR &= ~(1 << 2); // 切换为 pll1_main_clk

    /* PFDx = Fvco * 18 / N(12 ~ 35)*/
    /* PLL2(System)默认 528M */
    Value = CCM_ANALOG->PFD_528;
    Value &= ~(0x3F3F3F3F);
    Value |= (27 << 0);  // PLL2_PFD0 352M
    Value |= (16 << 8);  // PLL2_PFD1 594M
    Value |= (24 << 16); // PLL2_PFD2 396M
    Value |= (32 << 24); // PLL2_PFD3 297M
    CCM_ANALOG->PFD_528 = Value;

    /* PLL3(USB1)默认 480M */
    Value = CCM_ANALOG->PFD_480; 
    Value &= ~(0x3F3F3F3F);
    Value |= (12 << 0);  // PLL3_PFD0 720.0M
    Value |= (16 << 8);  // PLL3_PFD1 540.0M
    Value |= (17 << 16); // PLL3_PFD2 508.2M
    Value |= (19 << 24); // PLL3_PFD3 454.7M
    CCM_ANALOG->PFD_480 = Value;

    /* AHB_CLK_ROOT(6M ~ 132M)*/
    CCM->CBCMR &= ~(3 << 18);
    CCM->CBCMR |= (1 << 18); // PLL2_PFD2(1分频) —> PRE_PERIPH_CLK
    CCM->CBCDR &= ~(1 << 25); // PRE_PERIPH_CLK(1分频) —> PERIPH_CLK
    while (CCM->CDHIPR & (1 << 5))
        ; // PERIPH_CLK_SEL_BUSY

    /* 修改 AHB_PODF 位的时候需要先禁止 AHB_CLK_ROOT 的输出 */
    CCM->CBCDR &= ~(7 << 10);
    CCM->CBCDR |= (2 << 10); // PERIPH_CLK(3分频) —> AHB_CLK_ROOT(132M)
    while (CCM->CDHIPR & (1 << 1))
        ; // AHB_PODF_BUSY

    /* IPG_CLK_ROOT(3M ~ 66M) */
    CCM->CBCDR &= ~(3 << 8);
    CCM->CBCDR |= (1 << 8); // AHB_CLK_ROOT(2分频) —> IPG_CLK_ROOT(66M)

    /* PERCLK_CLK_ROOT(3M ~ 66M)*/
    CCM->CSCMR1 &= ~(1 << 6);
    CCM->CSCMR1 &= ~(0x3F << 0); // IPG_CLK_ROOT(1分频) —> PERCLK_CLK_ROOT(66M)
}