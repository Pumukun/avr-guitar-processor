#include "Encoder.h"
#include "pinMode.h"
#include "digitalRead.h"

typedef struct Encoder_flags {
    bool hold_f:        1;
    bool turn_f:        1;
    bool is_left_f:     1;
    bool is_right_f:    1;
    bool is_press_f:    1;
    bool is_release_f:  1;
    bool is_click_f:    1;
    bool is_holded_f:   1;
    bool is_single_f:   1;
    bool is_double_f:   1;
    bool reset_f:       1;
} Encoder_flags;

enum __state {
    NO_SPIN     = 0,
    LEFT        = 1,
    RIGHT       = 2,
    LEFT_HOLD   = 3,
    RIGHT_HOLD  = 4
};

typedef struct Private_Encoder {
    uint8_t __OUT_A;
    uint8_t __OUT_B;

#ifndef NO_SWITCH
    uint8_t __SW;
#endif

    Encoder_flags __flags;

    uint8_t __encoder_state;
    uint32_t __debounce_timer;

    uint8_t __prev_state;

} Private_Encoder;

void __set_pin_mode(uint8_t p_mode);
void __set_button_pin_mode(uint8_t p_mode);
void __set_direction(uint8_t p_direction);

bool __is_turn();
bool __is_right();
bool __is_left();

void __reset();

#ifndef NO_SWITCH
bool __is_right_hold();
bool __is_left_hold();

bool __is_press();
bool __is_release();
bool __is_click();
bool __is_holded();

bool __is_single();
bool __is_double();
#endif

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

    encoder->set_pin_mode           = &__set_pin_mode;
    encoder->set_button_pin_mode    = &__set_button_pin_mode;
    encoder->set_direction          = &__set_direction;

    encoder->is_turn                = &__is_turn;
    encoder->is_right               = &__is_right;
    encoder->is_left                = &__is_left;

#ifndef NO_SWITCH
    encoder->is_right_hold          = &__is_right_hold;
    encoder->is_left_hold           = &__is_left_hold;

    encoder->is_press               = &__is_press;
    encoder->is_release             = &__is_release;
    encoder->is_click               = &__is_click;
    encoder->is_holded              = &__is_holded;

    encoder->is_single              = &__is_single;
    encoder->is_double              = &__is_double;
#endif

    encoder->reset                  = &__reset;

    pinMode(prvt_encoder->__OUT_A, INPUT_PULLUP);
    pinMode(prvt_encoder->__OUT_B, INPUT_PULLUP);

    prvt_encoder->__prev_state = digitalRead(prvt_encoder->__OUT_A) | (digitalRead(prvt_encoder->__OUT_B) << 1);

    return encoder;
}

void __set_pin_mode(uint8_t p_mode) {

}

void __set_button_pin_mode(uint8_t p_mode) {

}

void __set_direction(uint8_t p_direction) {

}

bool __is_turn() {

}

bool __is_right() {

}

bool __is_left() {

}

void __reset() {

}

#ifndef NO_SWITCH
bool __is_right_hold() {

}

bool __is_left_hold() {

}

bool __is_press() {

}

bool __is_release() {

}

bool __is_click() {

}

bool __is_holded() {

}

bool __is_single() {

}

bool __is_double() {

}
#endif

