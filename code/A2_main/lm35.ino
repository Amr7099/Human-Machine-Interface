#include "lm35.h"
#include "adc.h"

void LM35_Init(void)
{
    ADC_Init();
}

float LM35_ReadTemperature(void)
{
    unsigned int ADC_Value;
    float Voltage;
    float Temperature;

    ADC_Value = ADC_Read(0);

    Voltage = (ADC_Value * 5.0) / 1023.0;

    Temperature = Voltage * 100.0;

    return Temperature;
}
