#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_1 		PB0
#define PRZEKAZNIK	PD7					//przekaznik elektroniczny do rozpiêcia oryginalnej wiazki
#define INPUT		PC0
#define OUTPUT_1	PB1
#define OUTPUT_2	0
#define STARTER 	PD2

	float	start_procedury = 5;					//wartosc utrzymania napiêcia

void pwm_init(uint16_t stop)
	{
		DDRB|=(1<<OUTPUT_1);	//outpin A
			//|(1<<OUTPUT_2);		//outpin B

		TCCR1A|=(1<<COM1A1)					//Clear OC1A on CM, set at BOTTOM
					|(0<<COM1A0)			//Clear OC1A on CM, set at BOTTOM
					|(0<<COM1B1)			//Clear OC1A on CM, set at BOTTOM
					|(0<<COM1B0)			//Clear OC1A on CM, set at BOTTOM
					|(1<<WGM11)				//fast PWM with top=ICR1
					|(0<<WGM10);			//fast PWM with top=ICR1

			TCCR1B|=(0<<ICNC1)				//input capture noise canceler
					|(0<<ICES1)				//input capture edge select
					|(1<<WGM13)				//fast PWM with top=ICR1
					|(1<<WGM12)				//fast PWM with top=ICR1
					|(0<<CS12)				//NO PRESCALER
					|(0<<CS11)				//NO PRESCALER
					|(1<<CS10);				//NO PRESCALER

			ICR1 = 1024;					//TOP!!

			OCR1A = stop;				//STOP!

	}

void interupt_init()
	{
		MCUCR|=(0<<ISC01)
				|(0<<ISC00);		//low level int0 interupt

		GICR|=(1<<INT0);			//INT0 enable
	}

ISR(INT0_vect)
	{
		DDRD|=(1<<PRZEKAZNIK);
		PORTD|=(1<<PRZEKAZNIK);

		DDRB|=(1<<LED_1);
		PORTB|=(1<<LED_1);

		uint16_t chwilowa;
		chwilowa = ((start_procedury*1024)/5);
		pwm_init(chwilowa);

		//_delay_ms(300);		//dobrac!
	}
int main (void)
{
	interupt_init();
	sei();

	while(1)
	{
		pwm_init(0);		//normalnie non stop off
		PORTB&=~(1<<LED_1);		//to samo ni¿ej i wy¿ej
		PORTB&=~(1<<PRZEKAZNIK);
	}
}


