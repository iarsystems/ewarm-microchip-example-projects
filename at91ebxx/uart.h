
/*
 * $Revision: 10 $
 */

void UartInit(int(*getchar_func)(), void(*putchar_func)(int));
void UartRxrdy();

int ReceiveLine(char* line, int timeout);
void SendLine(char* line);

