/*
  Interfaces for RP2040-based PCB
  See https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf for GPIO <--> interface mapping
*/   



/*
  GPIOs for the speaker, using PIO-based I2S:
  https://arduino-pico.readthedocs.io/en/latest/i2s.html
*/
#define SPEAKER_I2S_BCLK 17
#define SPEAKER_I2S_WS (pBCLK+1) /* This is technically not needed. The WS pin is assumed to be one greater than SPEAKER_BCLK */
#define SPEAKER_I2S_DOUT 19

#define I2S_BITS_PER_SAMPLE 16



/*
  GPIOs for the SD card. Uses the SPI1 (not SPI0) interface on the RP2040. 
*/
#define SD_SPI_IF SPI1

#define SD_SPI_CS 25
#define SD_SPI_SCK 26
#define SD_SPI_MOSI 27
#define SD_SPI_MISO 24



/*
  GPI0s for interfacing with the IO board. Uses the RP2040 I2C0 interface.
  https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
*/
#define IO_I2C_IF Wire
#define IO_I2C_SDA 20
#define IO_I2C_SCL 21
#define IO_I2C_INT 22 /* Interrupt pin for lower power mode, not tied to any interface */
