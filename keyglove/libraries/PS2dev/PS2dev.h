/*
 * ps2.h - a library to interface with ps2 hosts.
 * Written by Chris J. Kiick, January 2008.
 * modified by Gene E. Scogin, August 2008.
 * Release into public domain.
 *
 * limitations:
 *      we do not handle parity errors.
 *      The timing constants are hard coded from the spec. Data rate is
 *      not impressive. Probably lots of room for optimization.
 */

#ifndef ps2dev_h
#define ps2dev_h

// minimal Arduino support wrapper if we're outside of the Arduino environment
#ifndef ARDUINO
    #include "ArduinoWrapper.h"
#else
    #if ARDUINO < 100
        #include "WProgram.h"
    #else
        #include "Arduino.h"
    #endif
#endif

class PS2dev {
    public:
        PS2dev(uint8_t clockPin, uint8_t dataPin);
        int8_t write(uint8_t data);
        uint8_t read();

    private:
        uint8_t _ps2clk;
        uint8_t _ps2data;
        void golo(uint8_t pin);
        void gohi(uint8_t pin);
};

#endif /* ps2dev_h */


