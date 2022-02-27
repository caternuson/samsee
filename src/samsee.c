#include <hal_gpio.h>
#include <hal_delay.h>
#include <hal_init.h>

#include <peripheral_clk_config.h>
#include <hpl_pm_base.h>
#include <hpl_gclk_base.h>

#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)

#define LED0 GPIO(GPIO_PORTA, 27)

#define MAX_IRQ_COUNT  10
uint8_t irq_count = 0;
volatile uint16_t status[MAX_IRQ_COUNT];
volatile uint8_t intflag[MAX_IRQ_COUNT];
volatile uint8_t data[MAX_IRQ_COUNT];

void SERCOM1_Handler(void) {
  // for now, just storing some stuff
  if (irq_count < MAX_IRQ_COUNT) {
    status[irq_count] = SERCOM1->I2CS.STATUS.reg;
    intflag[irq_count] = SERCOM1->I2CS.INTFLAG.reg;
    data[irq_count] = SERCOM1->I2CS.DATA.reg;
    irq_count++;
  }
  // and clearing all interrupts
  hri_sercomi2cs_clear_INTFLAG_AMATCH_bit(SERCOM1);
  hri_sercomi2cs_clear_INTFLAG_PREC_bit(SERCOM1);
  hri_sercomi2cs_clear_INTFLAG_DRDY_bit(SERCOM1);
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
  NVIC_DisableIRQ((IRQn_Type)10);                     //
  NVIC_ClearPendingIRQ((IRQn_Type)10);                // NVIC setup
  NVIC_EnableIRQ((IRQn_Type)10);                      //

  hri_sercomi2cs_clear_CTRLA_ENABLE_bit(SERCOM1);     // disable to allow writing to protected registers
  hri_sercomi2cs_set_CTRLA_MODE_bf(SERCOM1, 0x04);    // write 0x04 to CTRLA.MODE to set I2C target mode
  hri_sercomi2cs_set_ADDR_ADDR_bf(SERCOM1, 0x42);     // set address
  hri_sercomi2cs_set_CTRLA_ENABLE_bit(SERCOM1);       // enable
  hri_sercomi2cs_set_INTEN_reg(SERCOM1, 0x87);        // enable all interrupts

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