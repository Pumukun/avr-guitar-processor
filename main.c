#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "uart.h"
#include "pinMode.h"
#include "digitalRead.h"
#include "sysclock.h"
#include "unolib.h"

#include "SSD1306.h"

#include "Encoder.h"

typedef uint8_t byte;

#define USE_MICRO_WIRE

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define PWM_FREQ 0x00FF // pwm frequency - 31.3KHz
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // 2 PWMs in parallel

uint16_t ADC_low, ADC_high;
int16_t input;

#define PUSHBUTTON_1 4
#define PUSHBUTTON_2 2

// ---- Encoder ----
#define OUT_A 6
#define OUT_B 5
#define SW 7

// ---- Encoder vars ----
volatile int counter = 0;

typedef enum Effect : uint8_t {
    CLEAN,
    DISTORTION,
    DELAY,
    BIT_CRUSHER
} Effect;

volatile Effect output_effect; 

// Distortion effect variables
uint16_t distortion_threshold = 6000;

// Delay effect variables
#define MAX_DELAY 200
byte DelayBuffer[MAX_DELAY];
uint8_t DelayCounter = 0;
uint8_t Delay_Depth = MAX_DELAY;

// Bit crusher variables
byte bit_depth = 2;


void print_effect() {
    char* effect_label = "";
    switch (output_effect) {
        case CLEAN:
            effect_label = "clean";
            break;
        case DISTORTION:
            effect_label = "distortion";
            break;
        case DELAY:
            effect_label = "delay";
            break;
        case BIT_CRUSHER:
            effect_label = "bit crusher";
            break;
    }
    
    OLED_Clear();
    OLED_SetCursor(0, 0);
    OLED_Printf("%s\n%d\n", effect_label, counter);
    
    printf("e: %s\nc: %d\n", effect_label, counter);
}

int main(void) {
    init_serial();
    init_sysclock_2();

    OLED_Init();
    OLED_Clear();

    pinMode(PUSHBUTTON_1, INPUT_PULLUP);
    pinMode(PUSHBUTTON_2, INPUT_PULLUP);

    Encoder* enc = new_encoder(OUT_A, OUT_B, SW);

    // setup ADC- configured to be reading automatically the hole time.
    ADMUX = 0x60; // left adjust, adc0, internal vcc
    ADCSRA = 0xe5; // turn on adc, ck/32, auto trigger
    ADCSRB = 0x07; // t1 capture for trigger
    DIDR0 = 0x01; // turn off digital inputs for adc0

    TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
    TCCR1B = ((PWM_MODE << 3) | 0x11); // ck/1
    TIMSK1 = 0x20; // interrupt on capture interrupt
    ICR1H = (PWM_FREQ >> 8);
    ICR1L = (PWM_FREQ & 0xff);
    DDRB |= ((PWM_QTY << 1) | 0x02); // turn on outputs
    
    sei();

    output_effect = CLEAN;
    print_effect();

    while(1) {
        enc->tick(enc);

        static uint32_t btn_tmr;
        if (!digitalRead(PUSHBUTTON_1) && millis() - btn_tmr >= 500) {
            btn_tmr = millis();
            output_effect = (output_effect + 1) % 4;
            print_effect();
        }
        if (!digitalRead(PUSHBUTTON_2) && millis() - btn_tmr >= 500) {
            btn_tmr = millis();
            output_effect = output_effect - 1;
            output_effect = output_effect % 4;
            print_effect();
        }

        if (enc->is_left(enc)) { 
            counter = counter - 1;
            output_effect = output_effect - 1;
            output_effect = output_effect % 4;
            print_effect();
        }
        if (enc->is_right(enc)) {
            counter = counter + 1;
            output_effect = output_effect + 1;
            output_effect = output_effect % 4;
            print_effect();
        }

        if (enc->is_left_hold(enc)) {
            counter = counter - 5;
            output_effect = output_effect - 1;
            output_effect = output_effect % 4;
            print_effect();
        }
        if (enc->is_right_hold(enc)) {
            counter = counter + 5;
            output_effect = output_effect + 1;
            output_effect = output_effect % 4;
            print_effect();
        }

        if (enc->is_single(enc)) printf("single\n");
        if (enc->is_double(enc)) printf("double\n");
        
        /*
        if (enc->is_press(enc)) {
            output_effect = CLEAN;
            counter = 0;
            print_effect();
        }
        */
    }
}

ISR(TIMER1_CAPT_vect) {
    // read the ADC input signal data: 2 bytes Low and High.
    ADC_low = ADCL; // Low byte need to be fetched first
    ADC_high = ADCH;

    switch (output_effect) {
        case CLEAN:
            input = ((ADC_high << 8) | ADC_low) + 0x8000;
            break;

        case DISTORTION:
            input = ((ADC_high << 8) | ADC_low) + 0x8000;
            if (input > distortion_threshold) {
                input = distortion_threshold;
            }
            break;

        case DELAY:
            DelayBuffer[DelayCounter] = ADC_low;

            DelayCounter++;
            if (DelayCounter >= Delay_Depth) {
                DelayCounter = 0;
            }
            input = (((DelayBuffer[DelayCounter] << 8) | ADC_low) + 0x8000) + (((ADC_high << 8) | ADC_low) + 0x8000);
            break;

        case BIT_CRUSHER:
            input = ((ADC_high << 8) | ADC_low) + 0x8000;
            input = input >> bit_depth;
            break;

        default:
            input = ((ADC_high << 8) | ADC_low) + 0x8000;
            break;
    }

    OCR1AL = ((input + 0x8000) >> 8);
    OCR1BL = input;
}

