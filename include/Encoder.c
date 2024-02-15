#include "Encoder.h"

#define ENC_PRIVATE ((Private_Encoder*)(p_Encoder->private_Encoder))

/** Encoder flags
 * Flags for encoder logic
*/
typedef struct Encoder_flags {
	bool is_turn_f:	 1;
	bool is_left_f:	 1;
	bool is_right_f:	1;
	bool is_release_f:  1;
	bool is_click_f:	1;
	bool is_holded_f:   1;
	bool is_single_f:   1;
	bool is_double_f:   1;
	bool direction_f:   1;
	bool reset_f:	   1;
} Encoder_flags;

/// Encoder states
enum __state {
	NO_SPIN	 = 0,
	LEFT		= 1,
	RIGHT	   = 2,
	LEFT_HOLD   = 3,
	RIGHT_HOLD  = 4
};

/** Private Encoder
 * Private encoder struct contains main encoder fields
*/ 
typedef struct Private_Encoder {
	uint8_t __OUT_A; 				/**< A out of encoder */
	uint8_t __OUT_B; 				/**< B out of encoder */

#ifndef NO_SWITCH
	uint8_t __SW; 					/**< Button out of encoder */
#endif

	Encoder_flags __flags; 			/**< Encoder flags */

	uint8_t __encoder_state; 		/**< Encoder state */
	uint32_t __debounce_timer; 		/**< Encoder spin debounce timer */

	uint8_t __prev_state; 			/**< Previous encoder state */

} Private_Encoder;

#ifndef NO_SWITCH
Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b, uint8_t p_sw) {
#else
Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b) {
#endif
	Private_Encoder* prvt_encoder = (Private_Encoder*)malloc(sizeof(Private_Encoder));
	Encoder* encoder = (Encoder*)malloc(sizeof(Encoder));

	prvt_encoder->__OUT_A = p_out_a;
	prvt_encoder->__OUT_B = p_out_b;

	prvt_encoder->__encoder_state = NO_SPIN;
	prvt_encoder->__debounce_timer = 0;

#ifndef NO_SWITCH
	prvt_encoder->__SW = p_sw;
	if (p_sw != -1) {
		pinMode(prvt_encoder->__SW, INPUT_PULLUP);
	}
#endif

	encoder->private_Encoder = (Private_Encoder*)prvt_encoder;

	pinMode(prvt_encoder->__OUT_A, INPUT_PULLUP);
	pinMode(prvt_encoder->__OUT_B, INPUT_PULLUP);

	prvt_encoder->__prev_state = (digitalRead(prvt_encoder->__OUT_A) << 1) | digitalRead(prvt_encoder->__OUT_B);

	return encoder;
}

void enc_set_pin_mode(Encoder* p_Encoder, bool p_mode) {
	pinMode(ENC_PRIVATE->__OUT_A, (p_mode) ? INPUT : INPUT_PULLUP);
	pinMode(ENC_PRIVATE->__OUT_B, (p_mode) ? INPUT : INPUT_PULLUP);
}

void enc_set_button_pin_mode(Encoder* p_Encoder, bool p_mode) {
	pinMode(ENC_PRIVATE->__SW, (p_mode) ? INPUT : INPUT_PULLUP);
}

void enc_set_direction(Encoder* p_Encoder, bool p_direction) {
	ENC_PRIVATE->__flags.direction_f = p_direction;
}

bool enc_is_turn(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__flags.is_turn_f) {
		ENC_PRIVATE->__flags.is_turn_f = false;
		return true;
	}
	return false;
}

bool enc_is_right(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__encoder_state == RIGHT) {
		printf("right\n");

		ENC_PRIVATE->__encoder_state = NO_SPIN;
		return true;
	}
	return false;
}

bool enc_is_left(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__encoder_state == LEFT) {
		printf("left\n");

		ENC_PRIVATE->__encoder_state = NO_SPIN;
		return true;
	}
	return false;
}

void enc_reset(Encoder* p_Encoder) {
	ENC_PRIVATE->__flags.is_turn_f = false;
	ENC_PRIVATE->__flags.is_left_f = false;
	ENC_PRIVATE->__flags.is_right_f = false;
	ENC_PRIVATE->__flags.is_release_f = false;
	ENC_PRIVATE->__flags.is_click_f = false;
	ENC_PRIVATE->__flags.is_holded_f = false;
	ENC_PRIVATE->__flags.is_single_f = false;
	ENC_PRIVATE->__flags.is_double_f = false;
	ENC_PRIVATE->__flags.direction_f = false;
	ENC_PRIVATE->__flags.reset_f = false;
}


/* button flags handling */
#ifndef NO_SWITCH
bool enc_is_right_hold(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__encoder_state == RIGHT_HOLD) {
		ENC_PRIVATE->__encoder_state = NO_SPIN;
		return true;
	}
	return false;
}

bool enc_is_left_hold(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__encoder_state == LEFT_HOLD) {
		ENC_PRIVATE->__encoder_state = NO_SPIN;
		return true;
	}
	return false;
}

bool enc_is_release(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__flags.is_release_f) {
		ENC_PRIVATE->__flags.is_release_f = false;
		return true;
	}
	return false;
}

bool enc_is_click(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__flags.is_click_f) {
		ENC_PRIVATE->__flags.is_click_f = false;
		return true;
	}
	return false;
}

bool enc_is_holded(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__flags.is_holded_f) {
		ENC_PRIVATE->__flags.is_holded_f = false;
		return true;
	}
	return false;
}

bool enc_is_single(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__flags.is_single_f) {
		ENC_PRIVATE->__flags.is_single_f = false;
		return true;
	}
	return false;
}

bool enc_is_double(Encoder* p_Encoder) {
	enc_tick(p_Encoder);
	if (ENC_PRIVATE->__flags.is_double_f) {
		ENC_PRIVATE->__flags.is_double_f = false;
		return true;
	}
	return false;
}
#endif // NO_SWITCH


/* ==== main encoder logic ==== */
void enc_tick(Encoder* p_Encoder) {
	uint32_t tmp_millis = millis();
	uint32_t debounce_delta = tmp_millis - ENC_PRIVATE->__debounce_timer;

	// reset flags
	ENC_PRIVATE->__flags.is_release_f = true;
	ENC_PRIVATE->__flags.is_click_f = false;
	ENC_PRIVATE->__flags.is_holded_f = false;

	// read encoder
	uint8_t MSB = digitalRead(ENC_PRIVATE->__OUT_A);
	uint8_t LSB = digitalRead(ENC_PRIVATE->__OUT_B);

	uint8_t encoded = (MSB << 1) | LSB;
	uint8_t sum = (ENC_PRIVATE->__prev_state << 2) | encoded;

	// button states
	uint8_t btn_click_count = 0;
	
	if (!digitalRead(ENC_PRIVATE->__SW) && (debounce_delta > ENCODER_SW_DEBOUNCE)) {
		btn_click_count++;
		ENC_PRIVATE->__flags.is_click_f = true;

		ENC_PRIVATE->__debounce_timer = tmp_millis;
		ENC_PRIVATE->__flags.is_turn_f = false;

		if (btn_click_count == 1) {
			ENC_PRIVATE->__flags.is_single_f = true;
		}
		if (btn_click_count == 2) {
			ENC_PRIVATE->__flags.is_double_f = true;
		}
		btn_click_count = 0;
	}

	// check direction
	if ((sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) && (debounce_delta > ENCODER_DEBOUNCE)) {
		ENC_PRIVATE->__flags.is_turn_f = true;
		ENC_PRIVATE->__flags.is_left_f = true;
		
		ENC_PRIVATE->__debounce_timer = tmp_millis;
		ENC_PRIVATE->__encoder_state = LEFT;
		if (!digitalRead(ENC_PRIVATE->__SW)) {
			ENC_PRIVATE->__flags.is_holded_f = true;
			ENC_PRIVATE->__encoder_state = LEFT_HOLD;
		}
	} else if ((sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) && (debounce_delta > ENCODER_DEBOUNCE)) {
		ENC_PRIVATE->__flags.is_turn_f = true; 
		ENC_PRIVATE->__flags.is_right_f = true;
		
		ENC_PRIVATE->__debounce_timer = tmp_millis;
		ENC_PRIVATE->__encoder_state = RIGHT;
		if (!digitalRead(ENC_PRIVATE->__SW)) {
			ENC_PRIVATE->__flags.is_holded_f = true;
			ENC_PRIVATE->__encoder_state = RIGHT_HOLD;
		}
	}
	
	// save new last state
	ENC_PRIVATE->__prev_state = encoded;
}

