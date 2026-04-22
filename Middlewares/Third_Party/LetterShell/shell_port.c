#include "shell_port.h"
#include "shell_uart_wrapper.h"
#include <stdio.h>
#include "osal.h"

Shell shell;
static char shellBuffer[512];
static osal_mutex_hdl_t shellMutex;


static int ShellLock(Shell* shell){
    osal_mutex_lock(shellMutex, OSAL_WAIT_FOREVER);
    return 0;
}

static int ShellUnlock(Shell* shell){
    osal_mutex_unlock(shellMutex);
    return 0;
}

void ShellInit(void){
    osal_mutex_create(shellMutex);
    if(shellMutex == NULL) {
        printf("Error: Failed to create shell mutex.\n");
        return;
    }
    shell.write = Wrapper_Shell_Uart_Send;
    shell.lock = ShellLock;
    shell.unlock = ShellUnlock;
    shellInit(&shell, shellBuffer, 512);
}


