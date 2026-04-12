#include "shell_port.h"

Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;

short ShellWrite(char* data, unsigned short len){
    HAL_UART_Transmit(&huart1, (uint8_t*)data, len, 0x1FF);
    return len;
}

short ShellRead(char* data, unsigned short len){
    if(HAL_UART_Receive(&huart1, (uint8_t*)data, len, 0x1FF) != HAL_OK){
        return 0;
    }else{
        return 1;
    }
}

int ShellLock(Shell* shell){
    xSemaphoreTake(shellMutex, portMAX_DELAY);
    return 0;
}

int ShellUnlock(Shell* shell){
    xSemaphoreGive(shellMutex);
    return 0;
}

void ShellInit(void){
    shellMutex = xSemaphoreCreateMutex();

    shell.write = ShellWrite;
    shell.read = ShellRead;
    shell.lock = ShellLock;
    shell.unlock = ShellUnlock;
    shellInit(&shell, shellBuffer, 512);
}


