/*****************************************************************
file: skeleton_code_hal.h

Starting point for YOUR hardware abstraction library (rho4).

This header gives you:
  1. The abstract base classes your library must build on.
  2. ONE fully worked derived class (DigitalOut) as a pattern.

Rules (see the lab notes):
  - Your Serial/PWM/ADC classes must program the peripheral
    registers directly (TCCR1x / OCR1A, ADMUX / ADCSRA, ...).
    Downloading and calling another library's analogWrite() from inside your 
    class scores nothing. Your C8/C9 code is the place to steal registers from.

Author: Ivan Ling, 2026     Licence: GPL version 2
*****************************************************************/

#ifndef HAL_SKELETON_2026_H
#define HAL_SKELETON_2026_H

#include <stdint.h>
#include <avr/io.h>
#include <util/setbaud.h>

class Peripheral {
public:
    virtual void        begin();  /* configure the hardware   */
    virtual const char* name();  /* for debug printing       */
};



//Sample -- Digital output
class DigitalOut : public Peripheral 
{
public:
    DigitalOut(uint8_t& ddr, uint8_t& port,
               uint8_t bit)
        : _ddr(ddr), _port(port), _bit(bit){}
    void set(bool on){
        if (on) _port |=  (1 << _bit);
        else    _port &= ~(1 << _bit);
    }
    void toggle(){
        _port ^= (1 << _bit); 
    }

private:
    volatile uint8_t& _ddr;
    volatile uint8_t& _port;
    uint8_t           _bit;
};

class PWMOutput : public Peripheral
{

public:
    const char* name() {return "PWM Name";}// debug shit

    void begin()        //sets pins to be PWM at frequency 12e6 / 256 = 
    {
        DDRB  |= (1 << PB3); // mae it output
        TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); //make it PWM
        TCCR0B = (1 << CS00); // prescaler
    }

    void update(uint8_t duty)           //takes in a duty cycle as a percentage and sets the duty cycle of the pin to be that duty cycle
    {
        if (duty > 255) duty = 255;
        OCR0A = duty;
    }
};

class ADCin : public Peripheral
{
public:
    const char* name() { return "ADC Name";}// debug shit

    void begin() //initialise the ADC. stolen from ilMattoQR
    {
        ADCSRA |= _BV(ADPS2) | _BV(ADPS1); /* F_ADC = F_CPU/64 */
        ADMUX = 0; /* Select channel 0 */
        ADMUX |= _BV(REFS0); /* AVCC reference */
        ADMUX |= _BV(ADLAR); /* ADCH contains 8 MSBs */
        ADCSRA |= _BV(ADATE); /* Automatic Trigger Enable */
        ADCSRA |= _BV(ADEN); /* Enable ADC */
        ADCSRA |= _BV(ADIE); /* Enable interrupt */
        ADCSRA |= _BV(ADSC); /* Start Conversions */
        sei();
    }
    uint8_t read()
    {
        return ADC; //retuns an 8 bit number 0-256 of the input voltage
    }

}

class serial : public Peripheral
{
    private:
        uint32_t baudeRate;

    public:

        const char* name() {return "ADC Name";}// debug shit
        Serial(uint32_t baud = 9600)
        : baudeRate(baud)
        {}    
        void begin // initialise UART connection. again, stolen from QR
        {
            UBRR0H = UBRRH_VALUE; 
            UBRR0L = UBRRL_VALUE;
            UCSR0A = USE_2X << U2X0;
            UCSR0B = _BV(RXEN0) | _BV(TXEN0);
            UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
        }

        void tx(uint8_t b) //from QR
        {
            while(!(UCSR0A & _BV(UDRE0))); 
            UDR0 = b; 
        } 
        uint8_t rx() //from QR
        { 
            while(!(UCSR0A & _BV(RXC0))); 
            return UDR0; 
        }
}

#endif
