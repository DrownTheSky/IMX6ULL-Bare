#include "imx6ull_system.h"
#include "imx6ull_it.h"
#include "./led/bsp_led.h"
#include "./key/bsp_key.h"

void Delay(int temp)
{
    for (uint32_t i = 0; i < temp; i++)
        ;
}

int main(void)
{
    SysClkInit();
    IntInit();
    led_init();
    key_init();

    while (1)
    {
        if (KEY1_Scan())
        {
            while (KEY1_Scan())
                ;
            led_tog();
        }
    }
}