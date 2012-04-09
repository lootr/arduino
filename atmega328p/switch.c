#include <avr/io.h>
#include <avr/eeprom.h>

uint8_t EEMEM p;

int main(void)
{
  uint8_t v;

  DDRB = _BV(PB5);

  while (PIND & _BV(PD2)) {
    eeprom_busy_wait();
    v = eeprom_read_byte(&p);
    if (v)
      PORTB |= _BV(PB5);
    else
      PORTB &= ~_BV(PB5);
    eeprom_write_byte(&p, v ? 0 : 1);
  }

  return 0;
}
