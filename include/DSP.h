/** \file DSP.h
 * Digital Signal Processor. \n
 * Contains main functional for audio signal processing.
*/ 

#ifndef DSP_H
#define DSP_H

// PWM setup
#define PWM_FREQ 0x00FF 					///< pwm frequency - 31.3KHz
#define PWM_MODE 0							///< Fast (1) or Phase Correct (0)
#define PWM_QTY 2							///< 2 PWMs in parallel

#define ON 1;
#define OFF 0;

/** DSP effects enum */
typedef enum Effect : uint8_t {
	CLEAN,									///< Clean effect
	DISTORTION,								///< Distortion effect
	DELAY,									///< Delay effect
	BIT_CRUSHER								///< Bit Crusher effect
} Effect;

typedef struct DSP {
	volatile uint16_t ADC_low;				///< Low output byte
	volatile uint16_t ADC_high;				///< High output byte
	int16_t input;						///< Input signal
	volatile Effect output_effect			///< output Effect
	volatile uint8_t master_volume;			///< Master Volume
} DSP;

/** \fn void DSP_init(void)
 * Initialize DSP inputs/outputs
*/ 
void DSP_init(void);

#endif // DSP_H
