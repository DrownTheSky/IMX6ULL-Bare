#ifndef __KEY_H
#define __KEY_H

#include "../../imx6ull/MCIMX6Y2.h"
#include "../../imx6ull/fsl_iomuxc.h"
#include "../../imx6ull/core_ca7.h"

#define KEY_ON     1
#define KEY_OFF    0

void key_init(void);
uint8_t KEY1_Scan(void);
uint8_t KEY2_Scan(void);

#endif /* __KEY_H */
