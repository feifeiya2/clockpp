#include "shell_port.h"
#include "shell_uart_wrapper.h"
#include <stdio.h>

Shell shell;
static char shellBuffer[512];
static SemaphoreHandle_t shellMutex;


static int ShellLock(Shell* shell){
    xSemaphoreTake(shellMutex, portMAX_DELAY);
    return 0;
}

static int ShellUnlock(Shell* shell){
    xSemaphoreGive(shellMutex);
    return 0;
}

void ShellInit(void){
    shellMutex = xSemaphoreCreateMutex();
    if(shellMutex == NULL) {
        printf("Error: Failed to create shell mutex.\n");
        return;
    }
    shell.write = Wrapper_Shell_Uart_Send;
    shell.lock = ShellLock;
    shell.unlock = ShellUnlock;
    shellInit(&shell, shellBuffer, 512);
}


