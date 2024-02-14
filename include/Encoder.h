/** \file Encoder.h
 * \brief Main Encoder file \n
 * Usage example:
 * \code
int main(void) {
	Encoder* encoder = new_encoder(A_PIN, B_PIN, SWITCH_PIN);

	while(1) {
		// for normal encoder processing
		// call tick function in main loop
		enc_tick(encoder);
		if (enc_is_turn(encoder)) {
			// Your code
		}
	}
}
 * \endcode
*/

#ifndef ENCODER_H
#define ENCODER_H

#include "unolib.h"
#include "pinMode.h"
#include "digitalRead.h"
#include "sysclock.h"

#define NORMAL 0
#define REVERSE 1

#define ENCODER_DEBOUNCE 200
#define ENCODER_SW_DEBOUNCE 500

#define true 1
#define false 0

typedef short unsigned int bool;

typedef struct Encoder {
    void* private_Encoder;
} Encoder;

#ifndef NO_SWITCH
/** \fn Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b, uint8_t p_out_sw)
 * \brief Create new encoder 
 * \param p_out_a A encoder pin
 * \param p_out_b B encoder pin
 * \param p_out_sw Encoder button pin, if NO_SWITCH is not defined
 * \return Encoder* p_Encoder - Pointer to Encoder to new Encoder
*/ 
Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b, uint8_t p_sw);
#else
Encoder* new_encoder(uint8_t p_out_a, uint8_t p_out_b);
#endif

/** \fn void enc_tick(Encoder* p_Encoder)
 * \brief Encoder tick function 
 * \param Encoder* p_Encoder - Pointer to Encoder
*/
void enc_tick(Encoder* p_Encoder);
	
/** \fn void enc_set_pin_mode(Encoder* p_Encoder, bool p_mode)
 * \brief Encoder pinmode setter 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \param p_mode **INPUT** or **INPUT_PULLUP**
*/	
void enc_set_pin_mode(Encoder* p_Encoder, bool p_mode);

/** \fn void enc_set_button_pin_mode(Encoder* p_Encoder, bool p_mode) 
 * \brief Encoder button pinmode setter
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \param p_mode **INPUT** or **INPUT_PULLUP**
*/
void enc_set_button_pin_mode(Encoder* p_Encoder, bool p_mode);

/** \fn void enc_set_direction(Encoder* p_Encoder, bool p_direction) 
 * \brief Encoder direction change method 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \param p_direction Encoder direction **NORMAL** or **REVERSE**
*/
void enc_set_direction(Encoder* p_Encoder, bool p_direction);

	
/** \fn bool enc_is_turn(Encoder* p_Encoder) 
 * \brief Check encoder turn
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return If Encoder turns returns **true** else returns **false**
*/
bool enc_is_turn(Encoder* p_Encoder);
	
/** \fn bool enc_is_right(Encoder* p_Encoder) 
 * \brief Check encoder turn right 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return If Encoder turns right returns **true** else returns **false**
*/
bool enc_is_right(Encoder* p_Encoder);
	
/** \fn bool enc_is_left(Encoder* p_Encoder) 
 * \brief Check encoder turn left 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return If Encoder turns left returns **true** else returns **false**
*/
bool enc_is_left(Encoder* p_Encoder);


#ifndef NO_SWITCH
/** \fn bool enc_is_right_hold(Encoder* p_Encoder) 
 * \brief Check right turn with button hold 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return If Encoder turns right with pressed button **true** else returns **false**
*/
bool enc_is_right_hold(Encoder* p_Encoder);

/** \fn bool enc_is_left_hold(Encoder* p_Encoder) 
 * \brief Check left turn with button hold 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return If Encoder turns left with pressed button **true** else returns **false**
*/
bool enc_is_left_hold(Encoder* p_Encoder);

/** \fn bool enc_is_release(Encoder* p_Encoder) 
 * \brief Check if button is released 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return If button released returns **true** else returns **false**
*/
bool enc_is_release(Encoder* p_Encoder);

/** \fn bool enc_is_click(Encoder* p_Encoder) 
 * \brief Check encoder button click 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return Returns **true** if encoder button was clicked else returns **false**
*/
bool enc_is_click(Encoder* p_Encoder);

/** \fn bool enc_is_holded(Encoder* p_Encoder)
 * \brief Check if button is holded 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return Returns **true** if encoder button is holded else returns **false**
*/
bool enc_is_holded(Encoder* p_Encoder);

/** \fn bool enc_is_single(Encoder* p_Encoder)
 * \brief Check encoder button single click 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return Returns **true** if button was clicked once else returns **false**
*/
bool enc_is_single(Encoder* p_Encoder);

/** \fn bool enc_is_double(Encoder* p_Encoder)
 * \brief Check encoder button double click 
 * \param Encoder* p_Encoder - Pointer to Encoder
 * \return Returns **true** if button was double clicked else returns **false**
*/
bool enc_is_double(Encoder* p_Encoder);
#endif

/** \fn void enc_reset(Encoder* p_Encoder)
 * \brief Reset encoder flags and state 
 * \param Encoder* p_Encoder - Pointer to Encoder
*/
void enc_reset(Encoder* p_Encoder);

#endif // ENCODER_H
