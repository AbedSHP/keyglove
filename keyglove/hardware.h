// Keyglove controller source code - Overall architecture/hardware option definitions
// 9/7/2013 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg

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

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

/** Main controller board options. Only one board may be selected at compile time. (defined in KG_BOARD) */

#define KG_BOARD_USER                   0           // You better know what you're doing
#define KG_BOARD_TEENSYPP2              100         // AT90USB1286, 46 I/O
#define KG_BOARD_ARDUINO_DUE            200         // SAM3X8E, bzillion I/O and Arduino design (prototype)
#define KG_BOARD_KEYGLOVE100            300         // NOT SUPPORTED YET: SAM3X8E, bzillion I/O and custom design



/** Flex sensor options. Only one choice may be selected at the same time. (defined in KG_FLEX) */

#define KG_FLEX_NONE                    0           // No flex sensors used
#define KG_FLEX_FINGERS                 1           // NOT SUPPORTED YET: Full 5-finger complement



/** Pressure sensor options. Only one choice may be selected at the same time. (defined in KG_PRESSURE) */

#define KG_PRESSURE_NONE                0           // No pressure sensors used
#define KG_PRESSURE_TIPS                1           // NOT SUPPORTED YET: Full 5-fingertip complement



/** Hand selection options. Only one choice may be selected at the same time. (defined in KG_HAND) */

#define KG_HAND_RIGHT                   1           /* Right-hand sensor connection orientation (default) */
#define KG_HAND_LEFT                    2           /* Left-hand sensor connection orientation (relevant for kit, others automatic) */



/** Dual glove communication options. Only one choice may be selected at the same time. (defined in KG_DUALGLOVE) */

#define KG_DUALGLOVE_NONE               0           /* No dual-glove support */
#define KG_DUALGLOVE_RFM22B             1           /* NOT SUPPORTED YET: Wireless connection with RFM22B */


/** Host interface options. Multiple interfaces may be enabled at the same time. (defined in KG_HOSTIF) */

#define KG_HOSTIF_NONE                  0           /* Don't communicate (weird, maybe you have a reason) */
#define KG_HOSTIF_USB_SERIAL            1           /* Hardware USB serial (requires ARM, AT90USB* or ATMega32U* MCU) */
#define KG_HOSTIF_USB_RAWHID            2           /* Hardware USB raw HID (requires ARM, AT90USB* or ATMega32U* MCU) */
#define KG_HOSTIF_USB_HID               4           /* Hardware USB HID (requires ARM, AT90USB* or ATMega32U* MCU) */
#define KG_HOSTIF_BT2_SERIAL            8           /* Bluetooth v2 serial (requires Bluegiga WT12 w/iWRAP) */
#define KG_HOSTIF_BT2_RAWHID            16          /* Bluetooth v2 raw HID (requires Bluegiga WT12 w/iWRAP v5) */
#define KG_HOSTIF_BT2_HID               32          /* Bluetooth v2 HID (requires Bluegiga WT12 w/iWRAP) */



/** Motion sensor options. Multiple sensors may be enabled, provided they do not compete. (defined in KG_MOTION) */

#define KG_MOTION_MPU6050_HAND          1           /* 6-axis I2C digital accel/gyro on back of hand */
#define KG_MOTION_MPU6050_INDEXTIP      2           /* 6-axis I2C digital accel/gyro on index fingertip */



/** Sensory feedback. Multiple options may be enabled. (defined in KG_FEEDBACK) */

#define KG_FEEDBACK_BLINK               1           /* Single LED (e.g. on Arduino/Teensy boards) */
#define KG_FEEDBACK_PIEZO               2           /* Piezo buzzer for sound */
#define KG_FEEDBACK_VIBRATE             4           /* Vibration motor for haptic feedback */
#define KG_FEEDBACK_RGB                 8           /* Combined RGB LED for visual feedback */



/** Sensory feedback connection. Only one choice may be selected at a time. (defined in KG_FEEDBACKCONN) */

#define KG_FEEDBACKCONN_NONE            0           /* Don't do anything with feedback */
#define KG_FEEDBACKCONN_DIRECT          1           /* Direct connection to I/O pins */
#define KG_FEEDBACKCONN_I2C             2           /* Custom I2C-based RGB/vibe/piezo module */



/** Debug settings. Multiple options may be enabled. (defined in KG_DEBUG) */

#define KG_DEBUG_NONE                   0
#define KG_DEBUG_BENCHMARK              1
#define KG_DEBUG_FEEDBACK               2
#define KG_DEBUG_TOUCH                  4
#define KG_DEBUG_TOUCHSET               8
#define KG_DEBUG_MOTION                 16
#define KG_DEBUG_HOSTIF_USB             1024
#define KG_DEBUG_HOSTIF_BT2             2048
#define KG_DEBUG_HID_KEYBOARD           4096
#define KG_DEBUG_HID_MOUSE              8192



#endif // _HARDWARE_H_


