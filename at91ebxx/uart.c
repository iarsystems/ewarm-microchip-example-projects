
/*
 * $Revision: 10 $
 */

#include <string.h>
#include "config.h"
#include "uart.h"
#include "timer.h"

#define RD_TIMEOUT 10

static char rbuf[RXBUF_SIZE];
static volatile int rptr = 0;
static int(*getchar_function)();
static void(*putchar_function)(int);


void UartInit(int(*getchar_func)(), void(*putchar_func)(int))
{
  getchar_function = getchar_func;
  putchar_function = putchar_func;
}


void UartRxrdy()
{
  unsigned char value;

  value = (*getchar_function)();

  if (rptr >= RXBUF_SIZE)
    return;
    
  rbuf[rptr++] = value;  
}


int ReceiveLine(char* line, int timeout)
{
  int n;
  int elapsed = 0;
  
  while (rptr == 0)
  {
    Sleep(RD_TIMEOUT);
    elapsed += RD_TIMEOUT;
    if (timeout && elapsed > timeout)
      return 0;
  }
  
  for (;;)
  {
    n = rptr;
    Sleep(RD_TIMEOUT);
    elapsed += RD_TIMEOUT;
    if (n == rptr)
      break;
    if (timeout && elapsed > timeout)
      return 0;
  }
  
  rptr = 0;
  memcpy(line, rbuf, n);
  rbuf[n] = 0;

  return n;
}


void SendLine(char* line)
{
  for ( ; *line; line++)
  {
    (*putchar_function)(*line);
  }
}


