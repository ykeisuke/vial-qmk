#include "quantum.h"

__attribute__ ((weak))
void matrix_init_kb(void) {
	// Turn status LED on, with the exception of THK
	#if defined(__AVR_ATmega32U4__)
	setPinOutput(E6);
	writePinHigh(E6);
	#endif

	matrix_init_user();
}
