#include <string.h>
//#include <stdint.h>
#include <stdbool.h>
//#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
//#include "em_emu.h"
//#include "bsp.h"
//#include "bsp_trace.h"

#include "systime.h"

#define BIT(x) (1U << (x))

void GPIO_Init() {
	CMU_ClockEnable(cmuClock_GPIO, true);

	for (int i = 0; i <= 6; i++) {
		GPIO_PinModeSet(gpioPortD, i, gpioModePushPull, 0);
	}
}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* Setup SysTick Timer for 1 msec interrupts  */
	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
		// yikes
		while (1)
			;
	}

	GPIO_Init();

	char *yeeters = "Hello from EFM32\nlmao yeet\n";
	for (int i = 0; i < strlen(yeeters); i++) {
		char c = yeeters[i];
		for (int j = 0; j < 8; j++) {
			GPIO_PinOutClear(gpioPortD, 6);
			Delay(2);
			GPIO_PinOutSet(gpioPortD, 6);

			if (c & BIT(7 - j)) {
				GPIO_PinOutSet(gpioPortD, 5);
			} else {
				GPIO_PinOutClear(gpioPortD, 5);
			}
			Delay(2);
		}
	}

	int yeet = 0;
	while (1) {
		for (int i = 0; i < 5; i++) {
			if (yeet & BIT(i)) {
				GPIO_PinOutSet(gpioPortD, i);

			} else {
				GPIO_PinOutClear(gpioPortD, i);
			}
		}
		yeet++;
		Delay(10);
	}
}
