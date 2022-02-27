#
# Makefile setup for ASF
#
ifeq ($(CHIP_FAMILY),)
  $(error You must provide a CHIP_FAMILY parameter)
endif

INC_ASF4 = \
	-Iasf4/$(CHIP_FAMILY) \
	-Iasf4/$(CHIP_FAMILY)/include \
	-Iasf4/$(CHIP_FAMILY)/CMSIS/Include \
	-Iasf4/$(CHIP_FAMILY)/hal/include \
	-Iasf4/$(CHIP_FAMILY)/hal/utils/include \
	-Iasf4/$(CHIP_FAMILY)/hpl/core \
	-Iasf4/$(CHIP_FAMILY)/hpl/gclk \
	-Iasf4/$(CHIP_FAMILY)/hpl/pm \
	-Iasf4/$(CHIP_FAMILY)/hpl/port \
	-Iasf4/$(CHIP_FAMILY)/hpl/sysctrl \
	-Iasf4/$(CHIP_FAMILY)/hpl/systick \
	-Iasf4/$(CHIP_FAMILY)/hri

SRC_ASF4 = \
	asf4/$(CHIP_FAMILY)/gcc/gcc/startup_$(CHIP_FAMILY).c \
	asf4/$(CHIP_FAMILY)/gcc/system_$(CHIP_FAMILY).c \
	asf4/$(CHIP_FAMILY)/hal/src/hal_init.c \
	asf4/$(CHIP_FAMILY)/hal/src/hal_io.c \
	asf4/$(CHIP_FAMILY)/hal/src/hal_atomic.c \
	asf4/$(CHIP_FAMILY)/hal/src/hal_sleep.c \
	asf4/$(CHIP_FAMILY)/hal/src/hal_gpio.c \
	asf4/$(CHIP_FAMILY)/hal/src/hal_delay.c \
	asf4/$(CHIP_FAMILY)/hal/utils/src/utils_list.c \
	asf4/$(CHIP_FAMILY)/hal/utils/src/utils_syscalls.c \
	asf4/$(CHIP_FAMILY)/hal/utils/src/utils_assert.c \
	asf4/$(CHIP_FAMILY)/hal/utils/src/utils_event.c \
	asf4/$(CHIP_FAMILY)/hpl/core/hpl_init.c \
	asf4/$(CHIP_FAMILY)/hpl/gclk/hpl_gclk.c \
	asf4/$(CHIP_FAMILY)/hpl/pm/hpl_pm.c \
	asf4/$(CHIP_FAMILY)/hpl/systick/hpl_systick.c \
	asf4/$(CHIP_FAMILY)/hpl/sysctrl/hpl_sysctrl.c \
	asf4/$(CHIP_FAMILY)/hpl/core/hpl_core_m0plus_base.c \
