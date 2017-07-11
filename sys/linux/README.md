U8g2: Linux support code
========================

This directory contains code to allow U8g2 to be run on GNU/Linux
systems.

It provides:

 * `u8x8_byte_linux_hw_i2c()` - I2C support implemented using the
   i2c-dev interfaces (currently write-only)
 * `u8x8_getch_linux()` - Read a single character from the terminal.
 * `u8x8_gpio_and_delay_linux()` - Provides delay capabilities.

Currently this is a basic port with support for relatively few message
codes. All unimplemented message codes result in diagnostic messages
on stderr.

Examples
--------

Initializing an SSD1306 using the U8g2 C API:

    u8g2_t u8g2;

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
                &u8g2, U8G2_R0, u8x8_byte_linux_hw_i2c,
		u8x8_gpio_and_delay_linux);
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));

Initializing an SSD1306 using the u8x8 C API:

    u8x8_t u8x8;

    u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c,
               u8x8_byte_linux_hw_i2c, u8x8_gpio_and_delay_linux);
    u8x8_InitDisplay(&u8x8);
