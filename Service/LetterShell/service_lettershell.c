#include "service_lettershell.h"
#include "shell_port.h"
#include "uart1_wrapper.h"

void task_shell(void* argument){
    uint8_t shell_data;
    Wrapper_Uart1_Init();
    ShellInit();
    while(1){
        Wrapper_Uart1_Recv(&shell_data);
        shellHandler(&shell, shell_data);
    }
}
