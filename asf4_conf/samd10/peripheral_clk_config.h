/* Auto-generated config file peripheral_clk_config.h */
#ifndef PERIPHERAL_CLK_CONFIG_H
#define PERIPHERAL_CLK_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

/**
 * \def CONF_CPU_FREQUENCY
 * \brief CPU's Clock frequency
 */
#ifndef CONF_CPU_FREQUENCY
#define CONF_CPU_FREQUENCY 48000000 //1000000
#endif

// <<< end of configuration section >>>

#ifndef CONF_GCLK_SERCOM1_CORE_SRC
#define CONF_GCLK_SERCOM1_CORE_SRC GCLK_CLKCTRL_GEN_GCLK0_Val
#endif
#ifndef CONF_GCLK_SERCOM1_CORE_FREQUENCY
#define CONF_GCLK_SERCOM1_CORE_FREQUENCY 48000000
#endif
#ifndef CONF_GCLK_SERCOM1_SLOW_SRC
#define CONF_GCLK_SERCOM1_SLOW_SRC GCLK_CLKCTRL_GEN_GCLK4_Val
#endif
#ifndef CONF_GCLK_SERCOM1_SLOW_FREQUENCY
#define CONF_GCLK_SERCOM1_SLOW_FREQUENCY 400000
#endif

#endif // PERIPHERAL_CLK_CONFIG_H
