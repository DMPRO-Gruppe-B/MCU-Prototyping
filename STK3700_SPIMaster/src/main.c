#include <string.h>
#include <stdint.h>
#include <stdbool.h>
//#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
//#include "em_emu.h"
//#include "bsp.h"
//#include "bsp_trace.h"

#include "systime.h"

#define BIT(x) (1U << (x))

typedef unsigned char byte;

typedef struct {
	uint8_t id;
	char *name;
	char *unit;
	uint16_t step_size;
	uint16_t value;
} setting_t;

void send_byte(byte b) {
	for (int j = 0; j < 8; j++) {
		if (b & BIT(7 - j)) {
			GPIO_PinOutSet(gpioPortD, 5);
		} else {
			GPIO_PinOutClear(gpioPortD, 5);
		}
		Delay(2);
		GPIO_PinOutSet(gpioPortD, 6);
		Delay(2);
		GPIO_PinOutClear(gpioPortD, 6);
	}
}

void send_setting(setting_t *setting) {
	byte bytes[3] = { setting->id, (setting->value & 0xFF00) >> 8,
			setting->value & 0xFF };
	for (int i = 0; i < 3; i++) {
		byte b = bytes[i];
		send_byte(b);
	}
}

void GPIO_Init() {
	CMU_ClockEnable(cmuClock_GPIO, true);

	for (int i = 0; i <= 6; i++) {
		GPIO_PinModeSet(gpioPortD, i, gpioModePushPull, 0);
		GPIO_PinOutClear(gpioPortD, i);
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
	Delay(20);

	/*
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
	 */

	/*
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
	 */
	setting_t test1 = { .id = 1, .name = "oof", .unit = "ms", .value = 10 };
	send_setting(&test1);
}
