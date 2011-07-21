// Keyglove controller source code - PS/2 keyboard class library
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef LUFA
    #include "WProgram.h"
#endif

#include "ps2dev.h"
#include "ps2keyboard.h"
#include "debug.h"

#ifdef round
    #undef round
#endif

PS2keyboard::PS2keyboard(PS2dev *k) {
    keyboard = k;
}

void PS2keyboard::keydown(uint16_t keycode) { cmd_keypress(keycode, true); }
void PS2keyboard::keyup(uint16_t keycode) { cmd_keypress(keycode, false); }
void PS2keyboard::keypress(uint16_t keycode) {
    cmd_keypress(keycode, true);
    delay(20);
    cmd_keypress(keycode, false);
}

// get/set keyboard typematic delay in ms (250, 500, 750, 1000)
uint16_t PS2keyboard::get_typematic_delay() { return typematic_delay; }
void PS2keyboard::set_typematic_delay(uint16_t d) {
    if (d < 250 || d > 1000) d = 500;
    typematic_delay = d;
}

// get/set keyboard typematic rate
float PS2keyboard::get_typematic_rate() { return typematic_rate; }
void PS2keyboard::set_typematic_rate(float r) {
    typematic_rate = r;
}

// get/set keyboard scan code set
uint8_t PS2keyboard::get_scancode_set() { return scancode_set; }
void PS2keyboard::set_scancode_set(uint8_t s) {
    if (s < 1 || s > 3) s = 2;
    scancode_set = s;
}

// get/set all keys to all states
uint8_t PS2keyboard::get_allkeys_mode() { return keys_mode; }
void PS2keyboard::set_allkeys_mode(uint8_t mode) {
    keys_mode = mode;
}

bool PS2keyboard::get_led_numlock() { return led_numlock; }
void PS2keyboard::set_led_numlock(bool state) { led_numlock = state; }

bool PS2keyboard::get_led_capslock() { return led_capslock; }
void PS2keyboard::set_led_capslock(bool state) { led_capslock = state; }

bool PS2keyboard::get_led_scrolllock() { return led_scrolllock; }
void PS2keyboard::set_led_scrolllock(bool state) { led_scrolllock = state; }

void PS2keyboard::initialize() {
    DEBUG_PRNL_PS2("Sending PS/2 keyboard initialization codes...");
    while (keyboard -> write(0xAA) != 0);
    delay(10);
    run_bat();
    DEBUG_PRNL_PS2("PS/2 keyboard initialization complete");
}

void PS2keyboard::run_bat() {
    DEBUG_PRNL_PS2("Running keyboard Basic Assurance Test (BAT)...");
    set_led_numlock(true);
    set_led_capslock(true);
    set_led_scrolllock(true);
    set_defaults();
    enabled = true;
    delay(520);
    DEBUG_PRNL_PS2("Keyboard BAT completed, sending success code...");
    cmd_success();
    DEBUG_PRNL_PS2("Finished");
}

void PS2keyboard::set_defaults() {
    DEBUG_PRNL_PS2("Setting PS/2 keyboard defaults...");
    set_typematic_delay(500);
    set_typematic_rate(10.9);
    set_scancode_set(2);
    set_allkeys_mode(0);
    buf_length = 0;
    last_sent_byte = 0;
    DEBUG_PRNL_PS2("PS/2 keyboard defaults set");
}

// send PS2 "acknowledge" code
void PS2keyboard::cmd_ack() { while (keyboard -> write(0xFA) != 0); last_sent_byte = 0xFA; }

// send PS2 "resend" code
void PS2keyboard::cmd_resend() { while (keyboard -> write(0xFE) != 0); last_sent_byte = 0xFE; }

// send PS2 "success" code
void PS2keyboard::cmd_success() { while (keyboard -> write(0xAA) != 0); last_sent_byte = 0xAA; }

// send PS2 "error" code
void PS2keyboard::cmd_error() { while (keyboard -> write(0xFC) != 0); last_sent_byte = 0xFC; }

// send PS2 "echo" code
void PS2keyboard::cmd_echo() { while (keyboard -> write(0xEE) != 0); last_sent_byte = 0xEE; }

// send key scancodes (make or break)
void PS2keyboard::cmd_keypress(uint16_t keycode, bool make) {
    uint8_t *bytes;
    if (keycode == PKEY_PRTSC) {
        bytes = scancode_prtsc;
    } else if (keycode == PKEY_PAUSE) {
        bytes = scancode_pause;
    } else {
        bytes = scancodes[keycode];
    }
    
    // determine make/break mode
    if (make) {
        //DEBUG_PRN_PS2("\nDEBUG: make scancode index ");
        //DEBUG_PRNL_PS2(keycode);
        for (uint8_t k = 0; bytes[k] > 0; k++) {
            //DEBUG_PRNF_PS2(bytes[k], HEX);
            while (keyboard -> write(bytes[k]) != 0);
            last_sent_byte = bytes[k];
        }
    } else {
        if (bytes[0] != 0xE1) {
            //DEBUG_PRN_PS2("\nDEBUG: break scancode index ");
            //DEBUG_PRNL_PS2(keycode);
            for (uint8_t k = 0; bytes[k] > 0; k++) {
                if (bytes[k] == 0xE0) {
                    //DEBUG_PRNF_PS2(0xE0, HEX);
                    while (keyboard -> write(bytes[k]) != 0);
                }
                //DEBUG_PRNF_PS2(0xF0, HEX);
                while (keyboard -> write(0xF0) != 0);
                //DEBUG_PRNF_PS2(bytes[k], HEX);
                while (keyboard -> write(bytes[k]) != 0);
                last_sent_byte = bytes[k];
            }
        }
    }
}

// process command from host
void PS2keyboard::process_command() {
    uint8_t command = 0, b = 0;
    while ((command = keyboard -> read()));
    DEBUG_PRN_PS2("Received keyboard host command 0x");
    DEBUG_PRNLF_PS2(command, HEX);
    switch (command) {
        case 0xED: // set/reset LEDs
            cmd_ack();
            b = keyboard -> read();
            set_led_scrolllock(bitRead(b, 0) == 1);
            set_led_numlock(bitRead(b, 1) == 1);
            set_led_capslock(bitRead(b, 2) == 1);
            break;
        case 0xEE: // echo
            while (keyboard -> write(0xEE) != 0);
            last_sent_byte = 0xEE;
            break;
        case 0xF0: // set scan code set
            cmd_ack();
            b = keyboard -> read();
            if (b == 0) {
                while (keyboard -> write(scancode_set));
            } else {
                set_scancode_set(b);
            }
            break;
        case 0xF2: // read ID
            cmd_ack();
            break;
        case 0xF3: // set typematic rate/delay
            uint8_t td, tr_a, tr_b;
            cmd_ack();
            b = keyboard -> read();
            td = b >> 5;
            tr_a = (b & 0x00011000) >> 3;
            tr_b = (b & 0x00000111);
            if (td == 3) set_typematic_delay(1000);
            else if (td == 2) set_typematic_delay(750);
            else if (td == 1) set_typematic_delay(500);
            else set_typematic_delay(250);
            set_typematic_delay((pow(2, tr_a) * (8 + tr_b)) / 240);
            break;
        case 0xF4: // enable
            cmd_ack();
            enabled = true;
            buf_length = 0;
            break;
        case 0xF5: // disable and set defaults (not full BAT)
            cmd_ack();
            set_defaults();
            enabled = false;
            break;
        case 0xF6: // set defaults and reset w/full test
            cmd_ack();
            run_bat();
            break;
        case 0xF7: // SC MODE 3 ONLY: set all keys type typematic
            cmd_ack();
            break;
        case 0xF8: // SC MODE 3 ONLY: set all keys type make/break
            cmd_ack();
            break;
        case 0xF9: // SC MODE 3 ONLY: set all keys type make
            cmd_ack();
            break;
        case 0xFA: // SC MODE 3 ONLY: set all keys type make/break/typematic
            cmd_ack();
            break;
        case 0xFB: // SC MODE 3 ONLY: set key type typematic
            cmd_ack();
            break;
        case 0xFC: // SC MODE 3 ONLY: set key type make/break
            cmd_ack();
            break;
        case 0xFD: // SC MODE 3 ONLY: set key type make
            cmd_ack();
            break;
        case 0xFE: // resend
            while (keyboard -> write(last_sent_byte) != 0);
            break;
        case 0xFF: // reset w/full test
            cmd_ack();
            run_bat();
            break;
        default: // unknown, hmm
            //cmd_ack();
            cmd_resend();
    }
}

