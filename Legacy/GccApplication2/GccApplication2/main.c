#include <avr/io.h>
#include <util/delay.h>


/*

LEDs should go on PC0, PC1, PC2, and PC3 with a 470 ohm resistor in series

lmk if this code doesn't work

*/

int main();


int main() {
	DDRC = 0xFF;

	while (1) {
		uint8_t* led_pins = malloc(4*1);
		for (uint8_t i = 0; i < 4; i++) {
			led_pins[i] = i;
		}

		for (uint8_t i = 0; i < 4; i++) {
			PORTC |= (1 << led_pins[i]);
			_delay_ms(500);

			PORTC &= ~(1 << led_pins[i]);
			_delay_ms(500);
		}


		free(led_pins);

	}


	return 0;
}
