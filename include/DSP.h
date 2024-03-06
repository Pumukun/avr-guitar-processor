/** \file DSP.h
 * Digital Signal Processor. \n
 * Contains main functional for audio signal processing.
*/ 

#ifndef DSP_H
#define DSP_H

#include "unolib.h"
#include "map.h"
#include "sysclock.h"
#include <stdint.h>

// PWM setup
#define PWM_FREQ 0x00FF 					///< pwm frequency - 31.3KHz
#define PWM_MODE 0							///< Fast (1) or Phase Correct (0)
#define PWM_QTY 2							///< 2 PWMs in parallel

#define MAX_DELAY 200

#define EFFECTS_COUNT 4

enum {OFF, ON};

/** DSP effects enum */
typedef enum Effect : uint8_t {
	CLEAN,									///< Clean effect
	DISTORTION,								///< Distortion effect
	DELAY,									///< Delay effect
	BIT_CRUSHER								///< Bit Crusher effect
} Effect;

typedef struct DSP {
	uint32_t ADC_low;						///< Low output byte
	uint32_t ADC_high;						///< High output byte
	int32_t input;							///< Input signal
	Effect output_effect;					///< output Effect
	uint32_t master_volume;					///< Master Volume
	Effect effects_order[EFFECTS_COUNT];	///< Effects order 
} DSP;

/** \fn void DSP_init(void)
 * Initialize DSP inputs/outputs
*/ 
void DSP_init(void);

void DSP_set_effect_state(Effect p_effect, uint8_t p_state);

void DSP_change_effect(Effect p_effect);

#endif // DSP_H
