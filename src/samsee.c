#include <hal_gpio.h>
#include <hal_delay.h>
#include <hal_init.h>

#include <peripheral_clk_config.h>
#include <hpl_pm_base.h>
#include <hpl_gclk_base.h>

#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)

#define LED0 GPIO(GPIO_PORTA, 27)

uint8_t reg_buffer[256];
uint8_t reg_addr;
uint8_t dir;
bool first_byte = true;

void SERCOM1_Handler(void) {
  //
  // ADDRESS MATCH
  //
  if (SERCOM1->I2CS.INTFLAG.bit.AMATCH) {
    dir = SERCOM1->I2CS.STATUS.bit.DIR;
    hri_sercomi2cs_clear_INTFLAG_AMATCH_bit(SERCOM1);
  }

  //
  // DATA READY
  //
  else if (SERCOM1->I2CS.INTFLAG.bit.DRDY) {
    if (dir) {
      // READ controller is reading from target
      SERCOM1->I2CS.DATA.reg = reg_buffer[reg_addr];
    } else {
      // WRITE controller is writing to target
      if (first_byte) {
        reg_addr = SERCOM1->I2CS.DATA.reg;
        first_byte = false;
      } else {
        reg_buffer[reg_addr] = SERCOM1->I2CS.DATA.reg;
      }
    }
    hri_sercomi2cs_clear_INTFLAG_DRDY_bit(SERCOM1);
  }

  //
  // STOP
  //
  else if (SERCOM1->I2CS.INTFLAG.bit.PREC) {
    first_byte = true;
    hri_sercomi2cs_clear_INTFLAG_PREC_bit(SERCOM1);
  }
}

int main() {
  init_mcu();

  //------------------------------------------
  // Configure SERCOM1 to be I2C target device
  // SAMD10 specific
  //------------------------------------------

  // clocks (I2C_0_CLOCK_init)
  _pm_enable_bus_clock(PM_BUS_APBC, SERCOM1);
  _gclk_enable_channel(SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC);
  _gclk_enable_channel(SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC);

  // sercom (i2c_s_async_init + i2c_s_async_set_addr)
  NVIC_DisableIRQ((IRQn_Type)10);                 // NVIC setup
  NVIC_ClearPendingIRQ((IRQn_Type)10);            // see table 10-3 in SAMD10 datasheet
  NVIC_EnableIRQ((IRQn_Type)10);                  // SERCOM1 NVIC Line = 10

  SERCOM1->I2CS.CTRLA.bit.ENABLE = 0x00;          // disable to allow writing to protected registers
  SERCOM1->I2CS.CTRLA.bit.MODE = 0x04;            // write 0x04 to CTRLA.MODE to set I2C target mode
  SERCOM1->I2CS.ADDR.bit.ADDR = 0x42;             // set address
  SERCOM1->I2CS.CTRLA.bit.ENABLE = 0x01;          // enable, done writing to protected registers
  SERCOM1->I2CS.INTENSET.bit.ERROR = 0x01;        // enable ERROR interrupt
  SERCOM1->I2CS.INTENSET.bit.DRDY = 0x01;         // enable DRDY interrupt
  SERCOM1->I2CS.INTENSET.bit.AMATCH = 0x01;       // enable AMATCH interrupt
  SERCOM1->I2CS.INTENSET.bit.PREC = 0x01;         // enable PREC (stop) interrupt

  // pins (I2C_0_PORT_init)
  gpio_set_pin_pull_mode(PA22, GPIO_PULL_OFF);             // SDA pin setup
  gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM1_PAD0);  //  "   "    "
  gpio_set_pin_pull_mode(PA23, GPIO_PULL_OFF);             // SCL pin setup
  gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM1_PAD1);  //  "   "    "

  //------------------------------------------
  // LED blink
  //------------------------------------------
  delay_init(SysTick);

  // config GPIO
  gpio_set_pin_level(LED0, false);
  gpio_set_pin_direction(LED0, GPIO_DIRECTION_OUT);
  gpio_set_pin_function(LED0, GPIO_PIN_FUNCTION_OFF);

  // blink forever
  while (1) {
    gpio_set_pin_level(LED0, true);
    delay_ms(100);
    gpio_set_pin_level(LED0, false);
    delay_ms(100);
  }

  return 0;
}