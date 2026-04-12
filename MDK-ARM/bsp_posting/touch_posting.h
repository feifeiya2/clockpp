#ifndef __TOUCH_POSTING_H
#define __TOUCH_POSTING_H

#include "stdint.h"

#define FT6336_I2C_HANDLE hi2c1
#define FT6336_RST_GPIO_Port T_RST_GPIO_Port
#define FT6336_RST_Pin       T_RST_Pin

void Touch_Posting_Init(void);

#endif /* __TOUCH_POSTING_H */
