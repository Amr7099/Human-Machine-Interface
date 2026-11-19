#include <avr/io.h>
#include <util/delay.h>
#include "INPUT_OUTPUT_DEFINE.h"
#include "uart.h"
#include "lm35.h"
#include "lcd.h"

// these are the initial temperature limits that we will use when the system starts
unsigned int HighLimit = 30;
unsigned int LowLimit = 20;

// the highest value that the user can set for the high temperature limit
#define MAX_LIMIT 150

// the main loop has a delay of 10 ms, so 100 iterations will approximately equal 1 second
// we use this value to determine how long a button has been held
#define HOLD_INTERVAL_COUNT 100

// function declarations
signed char LH_ButtonPressed(void);
signed char LL_ButtonPressed(void);

void LCD_WriteNumber(unsigned int Number);
void UART_WriteNumber(unsigned int Number);

void DisplayTemperature(float Temperature);
void SendTemperature(float Temperature);

int main(void)
{
    unsigned int LoopCounter = 0; // a counter that we use to update the temperature measurements every 500 ms

    float Temperature; // a variable to store the temperature read from the LM35 sensor

    // these variables store the actions returned by the two buttons
    // 1 means that the button was pressed and released quickly, while -1 means that it was held
    signed char LH_Action;
    signed char LL_Action;

    // initialize UART so we can send the temperature readings and system information to the serial monitor
    Uart_Init();

    // initialize the LM35 sensor and the ADC to start reading the analog temperature signal
    LM35_Init();

    // configure the LH button as an input using PB0
    IO_SelectPin(IO_PORT_B, PB0);
    IO_SetPinMode(IO_INPUT);

    // configure the LL button as an input using PB1
    IO_SelectPin(IO_PORT_B, PB1);
    IO_SetPinMode(IO_INPUT);

    // configure the LED as an output using PB2
    IO_SelectPin(IO_PORT_B, PB2);
    IO_SetPinMode(IO_OUTPUT);

    // set the initial state of the LED to LOW so it remains OFF when the system starts
    PORTB &= ~(1 << PB2);

    // initialize the LCD
    LCD_Init();

    // display a startup message on the LCD when the system is turned on
    LCD_SetCursor(0, 0);
    LCD_WriteString("Temperature HMI");
    LCD_SetCursor(1, 0);
    LCD_WriteString("System Starting");
    // send a startup message through UART to indicate that the system has started
    Uart_SendString("\r\n");
    Uart_SendString("========================\r\n");
    Uart_SendString("Temperature HMI Started\r\n");
    Uart_SendString("========================\r\n");

    // wait 1.5 seconds before clearing the startup message
    _delay_ms(1500);
    LCD_Clear();
    // display the initial temperature limits on the serial monitor
    Uart_SendString("Initial LH: 30 C\r\n");
    Uart_SendString("Initial LL: 20 C\r\n");
    Uart_SendString("------------------------\r\n");

    // the main loop runs continuously to keep checking the buttons and monitoring the temperature
    while (1)
    {

        // check the LH button to see if the user wants to change the high temperature limit
        LH_Action = LH_ButtonPressed();

        if (LH_Action == 1)
        {
            // if the button was pressed and released quickly, increase the high limit by 1 degree
            // we also check that it has not reached the maximum allowed value
            if (HighLimit < MAX_LIMIT)
            {
                HighLimit++;

                // display the new high limit on the serial monitor
                Uart_SendString("High Limit Increased: ");
                UART_WriteNumber(HighLimit);
                Uart_SendString(" C\r\n");
            }
        }
        else if (LH_Action == -1)
        {
            // if the button was held, decrease the high limit by 1 degree
            // we make sure that the high limit always stays at least 1 degree above the low limit
            if (HighLimit > LowLimit + 1)
            {
                HighLimit--;

                // display the new high limit on the serial monitor
                Uart_SendString("High Limit Decreased: ");
                UART_WriteNumber(HighLimit);
                Uart_SendString(" C\r\n");
            }
        }

        // check the LL button to see if the user wants to change the low temperature limit
        LL_Action = LL_ButtonPressed();

        if (LL_Action == 1)
        {
            // if the button was pressed and released quickly, increase the low limit by 1 degree
            // we make sure that the low limit does not reach or exceed the high limit
            if (LowLimit + 1 < HighLimit)
            {
                LowLimit++;

                // display the new low limit on the serial monitor
                Uart_SendString("Low Limit Increased: ");
                UART_WriteNumber(LowLimit);
                Uart_SendString(" C\r\n");
            }
        }
        else if (LL_Action == -1)
        {
            // if the button was held, decrease the low limit by 1 degree
            // the low limit cannot go below zero
            if (LowLimit > 0)
            {
                LowLimit--;

                // display the new low limit on the serial monitor
                Uart_SendString("Low Limit Decreased: ");
                UART_WriteNumber(LowLimit);
                Uart_SendString(" C\r\n");
            }
        }
        // add a 10 ms delay to control the speed of the main loop and the button timing
        _delay_ms(10);

        LoopCounter++;

        // update the temperature measurement every 50 iterations (approximately 500 ms)
        if (LoopCounter >= 50)
        {
            // reset the counter so the next measurement can be taken after another 50 iterations
            LoopCounter = 0;

            // read the current temperature from the LM35 sensor
            Temperature = LM35_ReadTemperature();

            // compare the measured temperature with the limits set by the user
            if (Temperature > HighLimit || Temperature < LowLimit)
            {
                // if the temperature is outside the allowed range, turn the LED ON to indicate an alert
                PORTB |= (1 << PB2);
            }
            else
            {
                // if the temperature is within the allowed range, turn the LED OFF
                PORTB &= ~(1 << PB2);
            }

            // display the latest temperature and the configured limits on the LCD
            DisplayTemperature(Temperature);

            // send the latest temperature reading and its status to the serial monitor
            SendTemperature(Temperature);
        }
    }

    return 0;
}

// this function checks the LH button connected to PB0 and determines whether it was pressed quickly or held
// it returns 1 for a quick press, -1 when the button has been held for approximately one second, and 0 if there is no action
signed char LH_ButtonPressed(void)
{
    // static variables keep their values between function calls, which allows me to track the button state
    static unsigned char LastReading = 0; // stores the previous reading of the button
    static unsigned char StableState = 0; // stores the confirmed state of the button after debouncing
    static unsigned char DebounceCounter = 0; // counts how many times the reading stays unchanged
    static unsigned int HoldCounter = 0; // counts how long the button has been held
    static unsigned char HasDecreased = 0; // a flag to prevent a long press from being treated as a quick press after release
    unsigned char Reading;

    // read the current state of PB0
    // if the button is pressed, the reading will be 1 because we are using an external pull-down resistor
    Reading = (PINB & (1 << PB0)) ? 1 : 0;

    // check whether the current reading is different from the previous reading
    // if it changes, reset the debounce counter because we need to make sure the new state is stable
    if (Reading != LastReading)
    {
        LastReading = Reading;
        DebounceCounter = 0;
    }
    else if (DebounceCounter < 3)
    {
        // if the reading stays the same, increase the counter until it reaches 3
        // this helps prevent false readings caused by the mechanical bouncing of the button
        DebounceCounter++;
    }

    // accept the new button state only after the reading has remained stable
    if (DebounceCounter >= 3 && StableState != Reading)
    {
        StableState = Reading;

        if (StableState == 1)
        {
            // the button has been pressed, so reset the holding counter and the long-press flag
            HoldCounter = 0;
            HasDecreased = 0;
        }
        else
        {
            // the button has been released

            if (HasDecreased == 0)
            {
                // if the long-press flag is still 0, it means the button was only pressed quickly
                // return 1 so the high temperature limit can be increased
                return 1;
            }
        }
    }

    // as long as the button remains pressed, keep counting how long it has been held
    if (StableState == 1)
    {
        HoldCounter++;

        if (HoldCounter >= HOLD_INTERVAL_COUNT)
        {
            // approximately one second has passed, so reset the counter and activate the long-press flag
            HoldCounter = 0;
            HasDecreased = 1;

            // return -1 to decrease the high temperature limit
            return -1;
        }
    }

    // return 0 if no button action has been detected
    return 0;
}

// this function does the same job as the previous one, but it checks the LL button connected to PB1
// it returns 1 for a quick press, -1 when the button has been held for approximately one second, and 0 if there is no action
signed char LL_ButtonPressed(void)
{
    // static variables keep their values between function calls to track the state of the LL button
    static unsigned char LastReading = 0; // stores the previous reading of the button
    static unsigned char StableState = 0; // stores the confirmed button state after debouncing
    static unsigned char DebounceCounter = 0; // counts how many times the reading stays unchanged
    static unsigned int HoldCounter = 0; // counts how long the button has been held
    static unsigned char HasDecreased = 0; // prevents a long press from being treated as a quick press after release
    unsigned char Reading;

    // read the current state of PB1
    // the reading will be 1 when the button is pressed because we are using an external pull-down resistor
    Reading = (PINB & (1 << PB1)) ? 1 : 0;


    // check whether the current reading is different from the previous reading
    // if it changes, reset the debounce counter to make sure the new state is stable
    if (Reading != LastReading)
    {
        LastReading = Reading;
        DebounceCounter = 0;
    }
    else if (DebounceCounter < 3)
    {
        // increase the counter if the reading stays unchanged
        // this helps eliminate false readings caused by button bouncing
        DebounceCounter++;
    }

    // accept the new button state only after the reading has remained stable
    if (DebounceCounter >= 3 && StableState != Reading)
    {
        StableState = Reading;

        if (StableState == 1)
        {
            // the button has been pressed, so reset the holding counter and the long-press flag
            HoldCounter = 0;
            HasDecreased = 0;
        }
        else
        {
            // the button has been released
            if (HasDecreased == 0)
            {
                // if the long-press flag is still 0, it means the button was only pressed quickly
                // return 1 so the low temperature limit can be increased
                return 1;
            }
        }
    }

    // as long as the button remains pressed, keep counting how long it has been held
    if (StableState == 1)
    {
        HoldCounter++;

        if (HoldCounter >= HOLD_INTERVAL_COUNT)
        {
            // approximately one second has passed, so reset the counter and activate the long-press flag
            HoldCounter = 0;
            HasDecreased = 1;

            // return -1 to decrease the low temperature limit
            return -1;
        }
    }

    // return 0 if no button action has been detected
    return 0;
}

// this function converts an unsigned integer into individual digits and displays them on the LCD
// we use this instead of a pre-developed number-printing function
void LCD_WriteNumber(unsigned int Number)
{
    if (Number >= 100)
    {
        // extract and display the hundreds digit
        LCD_WriteChar('0' + (Number / 100));

        // remove the hundreds digit so we can display the remaining two digits
        Number = Number % 100;

        // display the tens and units digits
        LCD_WriteChar('0' + (Number / 10));
        LCD_WriteChar('0' + (Number % 10));
    }
    else if (Number >= 10)
    {
        // display the tens and units digits if the number has at least two digits
        LCD_WriteChar('0' + (Number / 10));
        LCD_WriteChar('0' + (Number % 10));
    }
    else
    {
        // display a single digit if the number is less than 10
        LCD_WriteChar('0' + Number);
    }
}

// this function converts an unsigned integer into individual digits and sends them through UART
// we use it to display numerical values on the serial monitor
void UART_WriteNumber(unsigned int Number)
{
    if (Number >= 100)
    {
        // send the hundreds digit
        Uart_SendChar('0' + (Number / 100));

        // remove the hundreds digit
        Number = Number % 100;

        // send the tens and units digits
        Uart_SendChar('0' + (Number / 10));
        Uart_SendChar('0' + (Number % 10));
    }
    else if (Number >= 10)
    {
        // send the tens and units digits
        Uart_SendChar('0' + (Number / 10));
        Uart_SendChar('0' + (Number % 10));
    }
    else
    {
        // send a single digit if the number is less than 10
        Uart_SendChar('0' + Number);
    }
}

// this function displays the measured temperature, its status, and the configured limits on the LCD
void DisplayTemperature(float Temperature)
{
    unsigned int TemperatureTenths;
    unsigned int WholePart;
    unsigned int DecimalPart;
    // multiply the temperature by 10 and round it to the nearest tenth
    // this allows us to display one decimal place on the LCD
    TemperatureTenths = (unsigned int)(Temperature * 10.0 + 0.5);

    // separate the whole-number part from the decimal part
    WholePart = TemperatureTenths / 10;
    DecimalPart = TemperatureTenths % 10;

    // display the temperature and its status on the first row of the LCD
    LCD_SetCursor(0, 0);
    LCD_WriteString("T:");
    LCD_WriteNumber(WholePart);
    LCD_WriteChar('.');
    LCD_WriteNumber(DecimalPart);
    LCD_WriteString(" ");

    // check whether the measured temperature is outside the configured limits
    if (Temperature > HighLimit || Temperature < LowLimit)
    {
        // display NOK if the temperature is above the high limit or below the low limit
        LCD_WriteString("NOK");
    }
    else
    {
        // display OK if the temperature is within the allowed range
        LCD_WriteString("OK ");
    }

    // display the high and low temperature limits on the second row of the LCD
    LCD_SetCursor(1, 0);
    LCD_WriteString("LH:");
    LCD_WriteNumber(HighLimit);
    LCD_WriteString(" LL:");
    LCD_WriteNumber(LowLimit);
    // write a few spaces to clear any remaining characters from previous readings
    LCD_WriteString("   ");
}

// this function sends the measured temperature, configured limits, and system status through UART
void SendTemperature(float Temperature)
{
    unsigned int TemperatureTenths;
    unsigned int WholePart;
    unsigned int DecimalPart;

    // multiply the temperature by 10 and round it to the nearest tenth
    TemperatureTenths = (unsigned int)(Temperature * 10.0 + 0.5);

    // separate the whole-number part from the decimal part
    WholePart = TemperatureTenths / 10;
    DecimalPart = TemperatureTenths % 10;

    // send the measured temperature to the serial monitor
    Uart_SendString("Temperature: ");

    UART_WriteNumber(WholePart);

    Uart_SendChar('.');
    UART_WriteNumber(DecimalPart);

    // send the configured high and low limits
    Uart_SendString(" C | LH: ");

    UART_WriteNumber(HighLimit);

    Uart_SendString(" C | LL: ");

    UART_WriteNumber(LowLimit);

    // send the system status based on the measured temperature
    Uart_SendString(" C | Status: ");


    if (Temperature > HighLimit)
    {
        // indicate that the temperature has exceeded the high limit
        Uart_SendString("ALERT - ABOVE HIGH LIMIT");
    }
    else if (Temperature < LowLimit)
    {
        // indicate that the temperature has dropped below the low limit
        Uart_SendString("ALERT - BELOW LOW LIMIT");
    }
    else
    {
        // indicate that the temperature is within the allowed range
        Uart_SendString("NORMAL");
    }

    // move to a new line in the serial monitor
    Uart_SendString("\r\n");
}
