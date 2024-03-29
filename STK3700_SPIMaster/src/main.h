#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

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

#endif /* SRC_MAIN_H_ */
