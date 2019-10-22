#include <stdlib.h>
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
	uint16_t value;
	uint16_t step_size;
	uint16_t min;
	uint16_t max;
} setting_t;

typedef struct {
	char *name;
	uint8_t n_settings;
	setting_t **settings;
} effect_t;

effect_t effects[2];

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

setting_t *create_setting(uint8_t id, char *name, char *unit, uint16_t value, uint16_t step_size, uint16_t min, uint16_t max) {
	setting_t *setting = malloc(sizeof(setting_t));

	setting->name = malloc(strlen(name) + 1);
	setting->unit = malloc(strlen(unit) + 1);
	setting->id = id;
	setting->value = value;
	setting->step_size = step_size;
	setting->min = min;
	setting->max = max;

	strcpy(setting->name, name);
	strcpy(setting->unit, unit);

	return setting;
}

effect_t create_effect(char *name, uint8_t n_settings) {
	effect_t e;
	e.n_settings = n_settings;
	e.name = malloc(strlen(name) + 1);
	strcpy(e.name, name);
	e.settings = malloc(n_settings * sizeof(setting_t *));
	return e;
}

void GPIO_Init() {
	CMU_ClockEnable(cmuClock_GPIO, true);

	for (int i = 0; i <= 6; i++) {
		GPIO_PinModeSet(gpioPortD, i, gpioModePushPull, 0);
		GPIO_PinOutClear(gpioPortD, i);
	}
	GPIO_PinOutSet(gpioPortD, 4); // Chip select
}

void setup_effects() {
	// Bitcrusher
	effect_t bitcrusher = create_effect("Bitcrusher", 3);
	bitcrusher.settings[0] = create_setting(0b00000000, "Enable", "", 0, 1, 0, 1);
	bitcrusher.settings[1] = create_setting(0b00000001, "Bits", "bits", 0, 1, 0, 8);
	bitcrusher.settings[2] = create_setting(0b00000010, "Sample interval", "", 1, 1, 1, 16);

	effect_t delay = create_effect("Delay", 2);
	delay.settings[0] = create_setting(0b00000100, "Enable", "", 0, 1, 0, 1);
	delay.settings[1] = create_setting(0b00000101, "Delay", "ms", 0, 50, 0, 500);

	effects[0] = bitcrusher;
	effects[1] = delay;
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

	setup_effects();
}
