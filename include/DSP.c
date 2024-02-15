#include "DSP.h"

struct Clean_config {
	uint8_t state_f: 1;
} __Clean_conf = {true};

struct Distortion_config {
	uint8_t state_f: 1;
} __Distortion_conf = {false};

struct Delay_config {
	uint8_t state_f: 1;
} __Delay_conf = {false};

struct Bit_Crusher_config {
	uint8_t state_f: 1;
} __Bit_Crusher_conf = {false};


static DSP* __DSP = (DSP*)malloc(sizeof(DSP));


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

	__DSP->output_effect = CLEAN;
}

void __DSP_read_input(void) {
	__DSP->ADC_low = ADCL;
	__DSP->ADC_high = ADCH;
}

void __DSP_send_output(void) {
	OCR1AL = ((__DSP->input + 0x8000) >> 8);
	OCR1BL = __DSP->input;

}

ISR(TIMER1_CAPT_vect) {
	__DSP_read_input();

	__DSP_send_output();
}
