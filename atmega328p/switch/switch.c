#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

uint8_t p EEMEM;

int main(void)
{
  uint8_t v;

  DDRB = _BV(PB5);

  eeprom_busy_wait();
  eeprom_write_byte(&p, 0);
  while (1) {
    if (PIND & _BV(PD2)) {
      eeprom_busy_wait();
      v = eeprom_read_byte(&p);
      if (!v)
	PORTB |= _BV(PB5);
      else
	PORTB &= ~_BV(PB5);
      eeprom_write_byte(&p, ~v);
      while (PIND & _BV(PD2));
    }
  }

  return 0;
}
