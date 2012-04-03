#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define OC1 PB1
#define DDROC DDRB
#define OCR OCR1A
#define TIMSK TIMSK1

int main(void)
{
  /* Timer 1 is 8-bit PWM. */
  TCCR1A = _BV(WGM10) | _BV(COM1A1);

  /*
   * Start timer 1.
   *
   * NB: TCCR1A and TCCR1B could actually be the same register, so
   * take care to not clobber it.
   */
  TCCR1B = _BV(CS11) | _BV(CS10); // full clock

  /* Set PWM value to 0. */
  OCR = 0;

  /* Enable OC1 as output. */
  DDROC |= _BV(OC1) | _BV(PB5);

  /* Enable timer 1 overflow interrupt. */
  TIMSK = _BV (TOIE1);
  sei ();

  while (1) {
    _delay_ms(50);
  }

  return 0;
}

enum { UP, DOWN };

ISR (TIMER1_OVF_vect)           /* Note [2] */
{
    static uint8_t pwm;         /* Note [3] */
    static uint8_t direction;

    switch (direction)          /* Note [4] */
    {
        case UP:
	  if (++pwm >= 255) {
	    direction = DOWN;
	    PORTB &= ~_BV(PB5);
	  }
	  break;

        case DOWN:
	  if (--pwm <= 0) {
	    direction = UP;
	    PORTB |= _BV(PB5);
	  }
	  break;
    }

    OCR = pwm;                  /* Note [5] */
}
