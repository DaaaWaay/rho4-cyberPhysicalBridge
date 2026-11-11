#include "HAL.h"

int main()
{
    serial UART;
    PWMOutput output;
    ADCin analIn;

    UART.begin();
    output.begin();
    analIn.begin();
    
    uint8_t dutyCycle = UART.rx();

    while (true)
    {
        output.update(dutyCycle);
        uint8_t measure = analIn.read();
        UART.tx(measure);
    }
    

}