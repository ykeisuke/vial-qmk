#include "quantum.h"

__attribute__ ((weak))
void matrix_init_kb(void) {
	// Turn status LED on, with the exception of THK
	#if defined(__AVR_ATmega32U4__)
	gpio_set_pin_output(E6);
	gpio_write_pin_high(E6);
	#endif

	matrix_init_user();
}
