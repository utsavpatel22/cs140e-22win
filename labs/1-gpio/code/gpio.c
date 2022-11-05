/*
 * Implement the following routines to set GPIO pins to input or output,
 * and to read (input) and write (output) them.
 *
 * DO NOT USE loads and stores directly: only use GET32 and PUT32 
 * to read and write memory.  Use the minimal number of such calls.
 *
 * See rpi.h in this directory for the definitions.
 */
#include "rpi.h"

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x20200000
static const unsigned gpio_set0  = (GPIO_BASE + 0x1C);
static const unsigned gpio_clr0  = (GPIO_BASE + 0x28);
static const unsigned gpio_lev0  = (GPIO_BASE + 0x34);

//
// Part 1 implement gpio_set_on, gpio_set_off, gpio_set_output
//

// set <pin> to be an output pin.  
//
// note: fsel0, fsel1, fsel2 are contiguous in memory, so you 
// can (and should) use array calculations!
void gpio_set_output(unsigned pin) {
    // implement this
    // use <gpio_fsel0>
    const unsigned int n = pin/10;
    unsigned GPFSELn =  GPIO_BASE + (n * 4);
    unsigned int bit_mask = GET32(GPFSELn);

    // Clearning old values for a particular pin 
    bit_mask = bit_mask & ~(1<< (((pin - (n*10)) * 3)));
    bit_mask = bit_mask & ~(1<< (((pin - (n*10)) * 3) + 1));
    bit_mask = bit_mask & ~(1<< (((pin - (n*10)) * 3) + 2));

    // Setting the values for a particular functionality 
    bit_mask = bit_mask | (1<< (((pin - (n*10)) * 3)));
    bit_mask = bit_mask | (0<< (((pin - (n*10)) * 3) + 1));
    bit_mask = bit_mask | (0<< (((pin - (n*10)) * 3) + 2));
    PUT32(GPFSELn, bit_mask);
}

// set GPIO <pin> on.
void gpio_set_on(unsigned pin) {
    // implement this
    // use <gpio_set0>
    enum { GPFSET0  = 0x2020001C };
    PUT32(GPFSET0, (1 << pin));
}

// set GPIO <pin> off
void gpio_set_off(unsigned pin) {
    // implement this
    // use <gpio_clr0>
    enum { GPFSET0  = 0x20200028 };
    PUT32(GPFSET0, (1 << pin));
}

// set <pin> to <v> (v \in {0,1})
void gpio_write(unsigned pin, unsigned v) {
    if(v)
       gpio_set_on(pin);
    else
       gpio_set_off(pin);
}

//
// Part 2: implement gpio_set_input and gpio_read
//

// set <pin> to input.
void gpio_set_input(unsigned pin) {
    // implement.
    const unsigned int n = pin/10;
    unsigned GPFSELn =  GPIO_BASE + (n * 4);
    unsigned int bit_mask = GET32(GPFSELn);

    // Clearning old values for a particular pin and setting that pin to input by assigning 000
    bit_mask = bit_mask & ~(1<< (((pin - (n*10)) * 3)));
    bit_mask = bit_mask & ~(1<< (((pin - (n*10)) * 3) + 1));
    bit_mask = bit_mask & ~(1<< (((pin - (n*10)) * 3) + 2));

    PUT32(GPFSELn, bit_mask);
}

// return the value of <pin>
int gpio_read(unsigned pin) {
    unsigned v = 0;
    unsigned int bit_mask = 0;
    bit_mask = 1 << pin;
    v = GET32(gpio_lev0);
    v = v & bit_mask;

    // implement.
    return v;
}
