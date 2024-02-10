#ifndef ENCODER_H
#define ENCODER_H

#include "unolib.h"

#define NORMAL 0
#define REVERSE 1

#define true 1
#define false 0

typedef short unsigned int bool;

typedef struct Encoder {
    void* private_Encoder;

    void (*tick)(struct Encoder*);
    
    void (*set_pin_mode)(struct Encoder*, bool);
    void (*set_button_pin_mode)(struct Encoder*, bool);
    void (*set_direction)(struct Encoder*, bool);

    bool (*is_turn)(struct Encoder*);
    bool (*is_right)(struct Encoder*);
    bool (*is_left)(struct Encoder*);

#ifndef NO_SWITCH 
    bool (*is_right_hold)(struct Encoder*);
    bool (*is_left_hold)(struct Encoder*);

    bool (*is_press)(struct Encoder*);
    bool (*is_release)(struct Encoder*);
    bool (*is_click)(struct Encoder*);
    bool (*is_holded)(struct Encoder*);

    bool (*is_single)(struct Encoder*);
    bool (*is_double)(struct Encoder*);
#endif

    void (*reset)(struct Encoder*);
    
} Encoder;

#ifndef NO_SWITCH
Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b, uint8_t p_sw);
#else
Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b);
#endif

#endif // ENCODER_H
