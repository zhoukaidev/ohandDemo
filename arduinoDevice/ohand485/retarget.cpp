
#include "retarget.h"

#include <Arduino.h>


#ifdef ARDUINO_ARCH_STM32F1

extern "C" int _write(int fd, char *ptr, int len)
{
  int i = 0;
  
  /*
   * write "len" of char from "ptr" to file id "fd"
   * Return number of char written.
   *
   * Only work for STDOUT, STDIN, and STDERR
   */
  if (fd > 2)
  {
    return -1;
  }
  
  while (*ptr && (i < len))
  {
    Serial.write(*ptr);
    
    if (*ptr == '\n')
    {
      Serial.write('\r');
    }
    
    i++;
    ptr++;
  }
  
  return i;
}


void retarget(void)
{
  // NOTHING
}

#elif defined(ARDUINO_ARCH_AVR)

extern "C" int serial_putc(char c, struct __file *)
{
  Serial.write(c);
  return c;
}


void retarget(void)
{
  fdevopen(&serial_putc, 0);
}

#else

#error UNSUPPORTED ARCH

#endif
