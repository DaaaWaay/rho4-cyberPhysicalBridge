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


class Peripheral {
public:
    virtual void        begin();  /* configure the hardware   */
    virtual const char* name();  /* for debug printing       */
};


//Sample -- Digital output
class DigitalOut : public Peripheral {
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

//Write your own class here

#endif
