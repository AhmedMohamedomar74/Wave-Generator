#define F_CPU 8000000UL			/* Define frequency here its 8MHz */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "../MCAL/GPIO/GPIO.h"
#define VREFF 12
#define Factor 12
void SEQUARE (u8 AMP,u8 Freq);
void Staircase (u8 AMP,u8 Freq);
void Triangular (u8 AMP,u8 Freq);
void sin (u8 AMP,u8 Freq);

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << RXCIE);/* Turn on the transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */

	UBRRL = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate */
	UBRRH = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits */
}

unsigned char counter = 0;
unsigned char Rec_arr[3];
int main()
{
	UART_init(9600);
	sei();
	DDRB=0xFF;
    DDRA = 0xFF;
	while(1)
    {
        if (Rec_arr[0] > 0)
        {
            if (1 == Rec_arr[0])
            {
                SEQUARE(Rec_arr[1],Rec_arr[2]);
            }
            else if (2 == Rec_arr[0])
            {
                Staircase(Rec_arr[1],Rec_arr[2]);
            }
            else if (3 == Rec_arr[0])
            {
                sin(Rec_arr[1],Rec_arr[2]);
            }
            else if (4 == Rec_arr[0])
            {
                Triangular(Rec_arr[1],Rec_arr[2]);
            }
        }
        
        
    }
}

ISR(USART_RXC_vect)
{
	
    if (counter < 3)
    {
        Rec_arr[counter]=UDR;
        counter++;
    }
    else
    {
        counter = 0;
    }
    
    
}

void SEQUARE (u8 AMP,u8 Freq)
{               
    GPIO_setPortValue(PORTA,((AMP*0xff)/VREFF));
    _delay_ms(((1/Freq)*1000)/2);
    GPIO_setPortValue(PORTA,0);
    _delay_ms(((1/Freq)*1000)/2);
}


void Triangular (u8 AMP,u8 Freq)
{
    // for (u16 i = 0; i <= 255; i++) {
    //     GPIO_setPortValue(PORTA, ((AMP * i) / VREFF));
    //     _delay_ms(((1.0 / Freq) * 1000) / (2 * 255));
    // }

    // for (int i = 254; i >= 0; i--) {
    //     GPIO_setPortValue(PORTA, ((AMP * i) / VREFF));
    //     _delay_ms(((1.0 / Freq) * 1000) / (2 * 255));
    // }
    u8 i = 0;
    while (i<255)
    {
        GPIO_setPortValue(PORTA, ((AMP * i) / VREFF));
        _delay_ms(((1.0 / Freq) * 1000) / (2 * 255));
        i++;
    }


    for (u8 i = 255; i > 0; i--) {
        GPIO_setPortValue(PORTA, ((AMP * i) / VREFF));
        _delay_ms(((1.0 / Freq) * 1000) / (2 * 255));
    }

}

void Staircase (u8 AMP,u8 Freq)
{
    GPIO_setPortValue(PORTA,((AMP*0x33)/VREFF));
    _delay_ms(((1/Freq)*1000)/5);
    GPIO_setPortValue(PORTA,((AMP*0x66)/VREFF));
    _delay_ms(((1/Freq)*1000)/5);
    GPIO_setPortValue(PORTA,((AMP*0x99)/VREFF));
    _delay_ms(((1/Freq)*1000)/5);
    GPIO_setPortValue(PORTA,((AMP*0xcc)/VREFF));
    _delay_ms(((1/Freq)*1000)/5);
    GPIO_setPortValue(PORTA,((AMP*0xff)/VREFF));
    _delay_ms(((1/Freq)*1000)/5);
}

void sin (u8 AMP,u8 Freq)
{
    float sin_values[91] = {
        0.0000, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564,
        0.1736, 0.1908, 0.2079, 0.2249, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256,
        0.3420, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848,
        0.5000, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293,
        0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547,
        0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572,
        0.8660, 0.8746, 0.8829, 0.8910, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336,
        0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816,
        0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998,
        1.0000
    };

    for (u8 i = 0; i < 90; i++)
    {
        GPIO_setPortValue(PORTA,(((6+(AMP * sin_values[i])) * 255)/ VREFF));
        _delay_ms(((1/Freq)*1000)/360);
    }
    for (u8 i = 90; i > 0; i--)
    {
        GPIO_setPortValue(PORTA,(((6+(AMP * sin_values[i])) * 255)/ VREFF));
        _delay_ms(((1/Freq)*1000)/360);
    }
    for (u8 i = 0; i < 90; i++)
    {
        GPIO_setPortValue(PORTA,(((6-(AMP * sin_values[i])) * 255)/ VREFF));
        _delay_ms(((1/Freq)*1000)/360);
    }
    for (u8 i = 90; i > 0; i--)
    {
        GPIO_setPortValue(PORTA,(((6-(AMP * sin_values[i])) * 255)/ VREFF));
        _delay_ms(((1/Freq)*1000)/360);
    }
}
