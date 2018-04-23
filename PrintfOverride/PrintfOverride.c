/* Standard Includes */
#include <stdio.h>
#include <string.h>

/* DriverLib Includes */
#include "driverlib.h"

/* Project Includes */
#include "PrintfOverride.h"

int fputc(int _c, register FILE *_fp)
{
  while(!(UCA0IFG&UCTXIFG));
  UCA0TXBUF = (unsigned char) _c;

  return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++)
  {
    while(!(UCA0IFG&UCTXIFG));
    UCA0TXBUF = (unsigned char) _ptr[i];
  }

  return len;
}


