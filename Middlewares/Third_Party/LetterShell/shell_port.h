#ifndef __SHELL_PORT_H__
#define __SHELL_PORT_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
#include "semphr.h"

extern UART_HandleTypeDef huart1;
extern Shell shell;

void ShellInit(void);

#endif /* __SHELL_PORT_H__ */

