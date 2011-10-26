// Keyglove controller source code - Special hardware setup file
// 7/17/2011 by Jeff Rowberg <jeff@rowberg.net>

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



#ifndef _SETUP_FEEDBACK_VIBRATE_H_
#define _SETUP_FEEDBACK_VIBRATE_H_

#define KG_VIBRATE_OFF 0
#define KG_VIBRATE_LONGBUZZ 1
#define KG_VIBRATE_LONGPULSE 2
#define KG_VIBRATE_SHORTBUZZ 3
#define KG_VIBRATE_SHORTPULSE 4
#define KG_VIBRATE_TINYBUZZ 5
#define KG_VIBRATE_SOLID 6

uint8_t vibrateMode;
uint16_t vibrateTick;
uint16_t vibrateTickLimit;

void set_vibrate_mode(uint8_t mode, uint8_t duration) {
    vibrateMode = mode;
    if (mode == KG_VIBRATE_OFF) digitalWrite(KG_PIN_VIBRATE, HIGH);
    else if (mode == KG_VIBRATE_SOLID) digitalWrite(KG_PIN_VIBRATE, LOW);
    vibrateTick = 0;
    vibrateTickLimit = duration;
}

void set_vibrate_mode(uint8_t mode) {
    set_vibrate_mode(mode, 0);
}

void setup_feedback_vibrate() {
    pinMode(KG_PIN_VIBRATE, OUTPUT);
    digitalWrite(KG_PIN_VIBRATE, HIGH); // transistor switch makes it active-low
    // SELF-TEST
    //set_vibrate_mode(KG_VIBRATE_TINYBUZZ, 20);
}

void update_feedback_vibrate() {
    if (vibrateMode > 0) {
        if      (vibrateMode == 1 && vibrateTick % 50 == 0) digitalWrite(KG_PIN_VIBRATE, vibrateTick % 100 >= 50 ? HIGH : LOW);
        else if (vibrateMode == 2 && vibrateTick % 25 == 0) digitalWrite(KG_PIN_VIBRATE, vibrateTick % 100 >= 25 ? HIGH : LOW);
        else if (vibrateMode == 3 && vibrateTick % 10 == 0) digitalWrite(KG_PIN_VIBRATE, vibrateTick %  20 >= 10 ? HIGH : LOW);
        else if (vibrateMode == 4 && vibrateTick %  5 == 0) digitalWrite(KG_PIN_VIBRATE, vibrateTick %  20 >=  5 ? HIGH : LOW);
        else if (vibrateMode == 5 && vibrateTick %  5 == 0) digitalWrite(KG_PIN_VIBRATE, vibrateTick %  10 >=  5 ? HIGH : LOW);
        if (++vibrateTick >= vibrateTickLimit && vibrateTickLimit > 0) set_vibrate_mode(KG_VIBRATE_OFF);
    }
}

#endif // _SETUP_FEEDBACK_VIBRATE_H_


