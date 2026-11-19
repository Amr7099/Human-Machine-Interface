#include <avr/io.h>
#include "adc.h"

void ADC_Init(void)
{
    // Configure PC0 (ADC0) as input
    DDRC &= ~(1 << PC0);

    // Disable digital input on ADC0
    DIDR0 |= (1 << ADC0D);

    // Select AVCC as the voltage reference
    ADMUX = (1 << REFS0);

    // Enable ADC and set prescaler to 128
    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);
}

unsigned int ADC_Read(unsigned char channel)
{
    // Select the ADC channel
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait until conversion is complete
    while (ADCSRA & (1 << ADSC));

    // Read the ADC result
    unsigned int ADC_Value = ADCL;

    ADC_Value |= ((unsigned int)ADCH << 8);

    return ADC_Value;
}
