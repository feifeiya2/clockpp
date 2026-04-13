#include "service_lettershell.h"
#include "shell_port.h"
#include "shell_uart_wrapper.h"

void task_shell(void* argument){
    uint8_t shell_data;
    Wrapper_Shell_Uart_Init();
    ShellInit();
    while(1){
        Wrapper_Shell_Uart_Recv(&shell_data);
        shellHandler(&shell, shell_data);
    }
}
