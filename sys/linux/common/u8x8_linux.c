/*

  u8x8_linux.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2017, daniel@redfelineninja.org.uk
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "u8x8_linux.h"

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <linux/i2c-dev.h>

static int i2cfd;
static uint8_t i2cbuf[256];
static uint8_t i2clen;

static uint8_t u8x8_byte_linux_init(u8x8_t *u8x8)
{
  const char *devname;
  char dev[24];

  devname = getenv("U8X8_I2C_DEVICE");
  if (!devname)
  {
    devname = "0";
  }

  if (devname[0] != '/')
  {
    sprintf(dev, "/dev/i2c-%d", atoi(devname));
    devname = dev;
  }

  i2cfd = open(devname, O_RDWR);
  if (i2cfd < 0)
  {
    fprintf(stderr, "cannot open %s (%s)\n", dev, strerror(errno));
    return 0;
  }

  if (ioctl(i2cfd, I2C_SLAVE, u8x8_GetI2CAddress(u8x8)>>1) < 0)
  {
    fprintf(stderr, "cannot set slave address (%s)\n", strerror(errno));
    return 0;
  }

  return 1;
}

uint8_t u8x8_byte_linux_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t *data;
  int res;

  switch(msg)
  {
    case U8X8_MSG_BYTE_INIT:
      return u8x8_byte_linux_init(u8x8);

    case U8X8_MSG_BYTE_SEND:
      data = arg_ptr;
      while (arg_int--)
      {
	assert(i2clen < 255);
	i2cbuf[i2clen++] = *data++;
      }
      break;

    case U8X8_MSG_BYTE_START_TRANSFER:
      i2clen = 0;
      break;

    case U8X8_MSG_BYTE_END_TRANSFER:
      res = write(i2cfd, i2cbuf, i2clen);
      if (res < 0)
      {
	fprintf(stderr, "I2C write failed (%s)\n", strerror(errno));
      }
      else if (res != i2clen)
      {
	fprintf(stderr, "incomplete I2C write (%d of %d)\n", res, i2clen);
      }
      return res == i2clen;

    default:
      fprintf(stderr, "I2C: Unexpected message: %d\n", msg);
      return 0;
  }

  return 1;
}

int u8x8_getch_linux(void)
{
  int ch;
  struct termios oldt, newt;

  tcgetattr ( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

  return ch;
}

uint8_t u8x8_gpio_and_delay_linux(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      break;
    case U8X8_MSG_DELAY_MILLI:
      usleep(1000 * arg_int);
      break;
    case U8X8_MSG_GPIO_RESET:
      /* no capability to reset... ignore */
      break;
    default:
      fprintf(stderr, "GPIO/DELAY: Unexpected message: %d\n", msg);
      return 0;
  }

  return 1;
}
