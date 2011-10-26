// Keyglove Controller source code - Main setup/loop controller
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

// =============================================================================
// specific build hardware/debug options (EDIT THIS FILE FOR YOUR HARDWARE)
// =============================================================================

#include "config.h"

//#include <Wire.h>

// =============================================================================
// !!! NO EDITING SHOULD BE NECESSARY BEYOND THIS POINT !!!
// =============================================================================

/* ===============================================
 * UNIVERSAL CONTROLLER DECLARATIONS
=============================================== */

uint8_t keygloveTick = 0;   // increments every ~10ms (100hz), loops at 100
uint32_t keygloveTock = 0;  // increments ever 100 ticks, loops as 2^32 (~4 billion)
uint32_t keygloveTickTime = 0, keygloveTickTime0 = 0;

bool activeMagnetometer;
bool activeGestures;
bool activeTouch;
bool activeKeyboard;
bool activeMouse;
bool activeJoystick;
bool activeSerial;

uint16_t opt_touch_detect_threshold = 20;   // number of milliseconds required for a touch to register as real
uint8_t  opt_motion_sampling_div = 12;      // how many ticks in between each accel/gyro reading
uint8_t  opt_enable_calibration = 0;        // whether calibration should be enabled

// debug constant definitions
#include "debug.h"

// hardware constant option definitions
#include "hardware.h"

// minimal Arduino support wrapper if we're outside of the Arduino environment
#ifndef ARDUINO
    #include "ArduinoWrapper.h"
#endif

// pin assigment constants
#include "pins.h"

// pre-processor controlled global variable initializations and setup() calls
#include "setup.h"

/* ===============================================
 * MAIN SETUP/LOOP ROUTINES
=============================================== */

volatile uint8_t timer1Overflow = 0;
ISR(TIMER1_OVF_vect) {
    timer1Overflow++;
}

void setup() {
    // call main setup function (see setup.h for details)
    keyglove_setup();
    
    // setup internal 100Hz "tick" interrupt
    // thanks to http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1212098919 (and 'bens')
    // also, lots of timer info here and here:
    //    http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=50106
    //    http://www.avrbeginners.net/architecture/timers/timers.html
    TCCR1A = 1; // set TIMER1 overflow at 8-bit max (0xFF)
    TCCR1B = 1; // no prescaler
    TCNT1 = 0; // clear TIMER1 counter
    TIFR1 |= (1 << TOV1); // clear the TIMER1 overflow flag
    TIMSK1 |= (1 << TOIE1); // enable TIMER1 overflow interrupts*/
}

float g[3];
float tx = 0, ty = 0, tz = 0, tx2, ty2, tz2;
uint32_t testCount = 0, tb1, tb2;

void loop() {
    // read all defined and active motion sensors
    #ifdef ENABLE_ACCELEROMETER
        if (activeAccelerometer && readyAccelerometerData) update_motion_accelerometer();
    #endif
    #ifdef ENABLE_GYROSCOPE
        if (activeGyroscope && readyGyroscopeData) update_motion_gyroscope();
    #endif
    #ifdef ENABLE_MAGNETOMETER
        if (activeMagnetometer && readyMagnetometerData) update_motion_magnetometer();
    #endif
    #ifdef ENABLE_FUSION
        if (activeFusion && readyFusionData) update_motion_fusion();
    #endif

    #ifdef ENABLE_SERIAL
        update_hostif_serial();
    #endif

    // check for TIMER1 overflow limit and increment tick (should be every 10 ms)
    // 156 results in 9.937 ms, 157 results in 10.001 ms
    if (timer1Overflow >= 157) {
        timer1Overflow = 0;
        keygloveTick++;
        //keygloveTickTime += micros() - keygloveTickTime0;
        //keygloveTickTime0 = micros();

        // check for 100 ticks and reset counter (should be every 1 second)
        if (keygloveTick == 100) {
            //Serial.println(keygloveTickTime / 100);
            //keygloveTickTime = 0;
            keygloveTick = 0;
            keygloveTock++;
        }

            #ifdef ENABLE_ACCELEROMETER
                if (activeAccelerometer && activeGyroscope) {
                    //tb1 = micros();
                    filterUpdate(gxv * (PI/(180*14.375*4)), gyv * (PI/(180*14.375*4)), gzv * (PI/(180*14.375*4)), axa, aya, aza);
                    //filterUpdate(gxv, gyv, gzv, axa, aya, aza);
                    //Serial.println(micros() - tb1);
                    if (++testCount % 20 == 0) {
                        /*Serial.print(q0); Serial.print("\t");
                        Serial.print(q1); Serial.print("\t");
                        Serial.print(q2); Serial.print("\t");
                        Serial.print(q3); Serial.print("\t");

                        tx = 2 * (q1*q3 - q0*q2);
                        ty = 2 * (q0*q1 + q2*q3);
                        tz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

                        tx2 = atan2(2 * q1 * q2 - 2 * q0 * q3, 2 * q0*q0 + 2 * q1 * q1 - 1) * 180/M_PI;
                        ty2 = atan(tx / sqrt(ty*ty + tz*tz))  * 180/M_PI;
                        tz2 = atan(ty / sqrt(tx*tx + tz*tz))  * 180/M_PI;*/

                        g[0] = 2 * (q1 * q3 - q0 * q2);
                        g[1] = 2 * (q0 * q1 + q2 * q3);
                        g[2] = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
                        /*Serial.print(gxv * (PI/(180*14.375*4))); Serial.print("\t");
                        Serial.print(gyv * (PI/(180*14.375*4))); Serial.print("\t");
                        Serial.print(gzv * (PI/(180*14.375*4))); Serial.print("\t");
                        Serial.print(axa); Serial.print("\t");
                        Serial.print(aya); Serial.print("\t");
                        Serial.print(aza); Serial.print("\t");
                        Serial.print(axa - g[0]*250); Serial.print("\t");
                        Serial.print(aya - g[1]*250); Serial.print("\t");
                        Serial.println(aza - g[2]*250);*/
                        Serial.print(g[0]*250); Serial.print("\t");
                        Serial.print(g[1]*250); Serial.print("\t");
                        Serial.println(g[2]*250);
                        readyAccelerometerData = true;
                    }

                    //Serial.print(degrees(axTilt)); Serial.print("\t");
                    //Serial.print(degrees(ayTilt)); Serial.print("\t");
                    //Serial.println(degrees(azTilt));
        
                    /*tx += (float)gxv / 5750;
                    ty += (float)gyv / 5750;
                    tz += (float)gzv / 5750;
        
                    Serial.print(tx); Serial.print("\t");
                    Serial.print(ty); Serial.print("\t");
                    Serial.println(tz);*/
                }
            #endif

        // process motion into gestures
        //if (activeGestures) update_gestures();

        // process touch sensor status
        /////if (activeTouch) update_touch();
    
        // keyboard is almost always done in real time, this is usually not necessary
        //if (activeKeyboard) update_keyboard();
    
        // send any unsent mouse control data
        /////if (activeMouse) update_mouse();

        // send any unsent joystick control data
        /////if (activeJoystick) update_joystick();

        // check for blink condition (should be every 1/2 second)
        #if (KG_FEEDBACK & KG_FEEDBACK_BLINK) > 0
            if (keygloveTick % 50 == 0) {
                // update simple blinking LED
                update_feedback_blink();
            }
        #endif /* KG_FEEDBACK_BLINK */

        // update other feedback (should be every 10ms, sliced according to particular design)
        #if (KG_FEEDBACK & KG_FEEDBACK_RGB) > 0
            update_feedback_rgb();
        #endif /* KG_FEEDBACK_RGB */
        #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO) > 0
            update_feedback_piezo();
        #endif /* KG_FEEDBACK_PIEZO */
        #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE) > 0
            update_feedback_vibrate();
        #endif /* KG_FEEDBACK_PIEZO */
    }
}