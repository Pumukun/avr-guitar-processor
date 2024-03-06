#include "DSP.h"

static volatile uint32_t isr_timer = 0;

static volatile DSP __DSP;

struct Clean_config {
	uint8_t state_f: 1;
	uint16_t volume;
} __Clean_conf = {true, 10000};

struct Distortion_config {
	uint8_t state_f: 1;
	uint16_t threshold;
} __Distortion_conf = {false, 0};

struct Delay_config {
	uint8_t state_f: 1;
	uint8_t buffer[MAX_DELAY];
	uint8_t counter;
	uint8_t depth;
} __Delay_conf = {false, {}, 0, MAX_DELAY};

struct Bit_Crusher_config {
	uint8_t state_f: 1;
	uint8_t bit_depth;
} __Bit_Crusher_conf = {false, 0};

void DSP_init(void) {
	// setup ADC- configured to be reading automatically the hole time.
	ADMUX = 0x60;	// left adjust, adc0, internal vcc
	ADCSRA = 0xe5;	// turn on adc, ck/32, auto trigger
	ADCSRB = 0x07;	// t1 capture for trigger
	DIDR0 = 0x01;	// turn off digital inputs for adc0

	TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1));
	TCCR1B = ((PWM_MODE << 3) | 0x11);	// ck/1
	TIMSK1 = 0x20;						// interrupt on capture interrupt
	ICR1H = (PWM_FREQ >> 8);
	ICR1L = (PWM_FREQ & 0xff);
	DDRB |= ((PWM_QTY << 1) | 0x02);	// turn on outputs

	__DSP.output_effect = CLEAN;
	__DSP.master_volume = 10000;

	for (uint8_t i = 0; i < EFFECTS_COUNT; ++i) {
		__DSP.effects_order[i] = i;
	}
}

void DSP_set_effect_state(Effect p_effect, uint8_t p_state) {
	switch (p_effect) {
		case CLEAN:
			__Clean_conf.state_f = p_state;
			break;

		case DISTORTION:
			__Distortion_conf.state_f = p_state;
			break;

		case DELAY:
			__Delay_conf.state_f = p_state;
			break;

		case BIT_CRUSHER:
			__Bit_Crusher_conf.state_f = p_state;
			break;
	}
}

void DSP_change_effect(Effect p_effect) {
	__DSP.output_effect = p_effect;
}

void __DSP_read_input(void) {
	__DSP.ADC_low = ADCL;
	__DSP.ADC_high = ADCH;
}

void __DSP_send_output(void) {
	switch (__DSP.output_effect) {
		case CLEAN:
			__DSP.input = ((__DSP.ADC_high << 8) | __DSP.ADC_low) + 0x8000;
			__DSP.input = map(__DSP.input, -INT32_MAX, INT32_MAX, -__Clean_conf.volume, __Clean_conf.volume);
			break;

		case DISTORTION:
			__DSP.input = ((__DSP.ADC_high << 8) | __DSP.ADC_low) + 0x8000;
			if (__DSP.input > __Distortion_conf.threshold) {
				__DSP.input = __Distortion_conf.threshold;
			}
			break;

		case DELAY:
			__DSP.ADC_low = 0;
			__Delay_conf.buffer[__Delay_conf.counter] = __DSP.ADC_high;

			__Delay_conf.counter++;
			if (__Delay_conf.counter >= __Delay_conf.depth) {
				__Delay_conf.counter = 0;
			}
			__DSP.input = (((__Delay_conf.buffer[__Delay_conf.counter] << 8) | __DSP.ADC_low) + 0x8000) + (((__DSP.ADC_high << 8) | __DSP.ADC_low) + 0x8000);
			break;

		case BIT_CRUSHER:
			__DSP.input = ((__DSP.ADC_high << 8) | __DSP.ADC_low) + 0x8000;
			__DSP.input = __DSP.input >> __Bit_Crusher_conf.bit_depth;
			break;

		default:
			__DSP.input = ((__DSP.ADC_high << 8) | __DSP.ADC_low) + 0x8000;
			break;
	}


	__DSP.input = map(__DSP.input, -INT32_MAX, INT32_MAX, -__DSP.master_volume, __DSP.master_volume);

	OCR1AL = ((__DSP.input + 0x8000) >> 8);
	OCR1BL = __DSP.input;
}

ISR(TIMER1_CAPT_vect) {
	__DSP_read_input();

	__DSP_send_output();

	isr_timer = isr_timer + 1;
}
