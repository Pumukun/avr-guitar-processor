#ifndef ENCODER_H
#define ENCODER_H

#define NORMAL 0
#define REVERSE 1

typedef unsigned short bool;

typedef struct Encoder {
    void* private_Encoder;
    
    void (*set_pin_mode)(uint8_t);
    void (*set_button_pin_mode)(uint8_t);
    void (*set_direction)(uint8_t);

    bool (*is_turn)();
    bool (*is_right)();
    bool (*is_left)();

#ifndef NO_SWITCH 
    bool (*is_right_hold)();
    bool (*is_left_hold)();

    bool (*is_press)();
    bool (*is_release)();
    bool (*is_click)();
    bool (*is_holded)();

    bool (*is_single)();
    bool (*is_double)();
#endif

    void (*reset)();
    
} Encoder;

#ifndef NO_SWITCH
Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b, uint8_t p_sw);
#else
Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b);
#endif

#endif // ENCODER_H
