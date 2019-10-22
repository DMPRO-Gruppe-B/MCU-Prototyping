#include "main.h"
#include "systime.h"

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
	GPIO_PinOutClear(gpioPortD, 4); // Chip select
	Delay(2);
	send_byte(setting->id);
	send_byte((byte) (setting->value & 0xFF00) >> 8);
	send_byte((byte) setting->value & 0xFF);
	GPIO_PinOutSet(gpioPortD, 4);
}

void GPIO_Init() {
	CMU_ClockEnable(cmuClock_GPIO, true);

	for (int i = 0; i <= 6; i++) {
		GPIO_PinModeSet(gpioPortD, i, gpioModePushPull, 0);
		GPIO_PinOutClear(gpioPortD, i);
	}
	GPIO_PinOutSet(gpioPortD, 4); // Chip select
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
	setup_effects();

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
