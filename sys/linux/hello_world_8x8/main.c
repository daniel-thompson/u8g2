#include "u8x8.h"
#include "u8x8_linux.h"

u8x8_t u8x8;

int main(void)
{
  u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c,
             u8x8_byte_linux_hw_i2c, u8x8_gpio_and_delay_linux);
  u8x8_InitDisplay(&u8x8);
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetPowerSave(&u8x8, 0);

  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_f);

  u8x8_DrawString(&u8x8, 0, 0, "Hello World!");

  u8x8_Draw2x2String(&u8x8, 0, 3, "Hello World!");

  return 0;
}

