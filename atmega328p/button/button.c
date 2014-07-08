#include <avr/io.h>

int main(void)
{
  DDRB = _BV(PB5);

  while (1) {
    if (PIND & _BV(PD2))
      PORTB |= _BV(PB5);
    else
      PORTB &= ~_BV(PB5);
  }

  return 0;
}
