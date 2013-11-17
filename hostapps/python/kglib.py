#!/usr/bin/env python

""" Keyglove protocol parser/generator library

Changelog:
    2013-11-16 - Initial release

============================================
Keyglove Protocol Python interface library
2013-11-16 by Jeff Rowberg <jeff@rowberg.net>
Updates should (hopefully) always be available at https://github.com/jrowberg/keyglove

============================================
Keyglove Protocol Python interface library code is placed under the MIT license
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

"""

__author__ = "Jeff Rowberg"
__license__ = "MIT"
__version__ = "2013-11-16"
__email__ = "jeff@rowberg.net"

import struct



# thanks to Masaaki Shibata for Python event handler code
# http://www.emptypage.jp/notes/pyevent.en.html

class KeygloveEvent(object):

    def __init__(self, doc=None):
        self.__doc__ = doc

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return KeygloveEventHandler(self, obj)

    def __set__(self, obj, value):
        pass



class KeygloveEventHandler(object):

    def __init__(self, event, obj):

        self.event = event
        self.obj = obj

    def _getfunctionlist(self):

        """(internal use) """

        try:
            eventhandler = self.obj.__eventhandler__
        except AttributeError:
            eventhandler = self.obj.__eventhandler__ = {}
        return eventhandler.setdefault(self.event, [])

    def add(self, func):

        """Add new event handler function.

        Event handler function must be defined like func(sender, earg).
        You can add handler also by using '+=' operator.
        """

        self._getfunctionlist().append(func)
        return self

    def remove(self, func):

        """Remove existing event handler function.

        You can remove handler also by using '-=' operator.
        """

        self._getfunctionlist().remove(func)
        return self

    def fire(self, earg=None):

        """Fire event and call all handler functions

        You can call EventHandler object itself like e(earg) instead of
        e.fire(earg).
        """

        for func in self._getfunctionlist():
            func(self.obj, earg)

    __iadd__ = add
    __isub__ = remove
    __call__ = fire



class KGLib(object):

    def kg_cmd_system_ping(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x01, 0x01)
    def kg_cmd_system_reset(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x01, 0x02)
    
    def kg_cmd_touch_get_mode(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x02, 0x01)
    def kg_cmd_touch_set_mode(self, mode):
        return struct.pack('<4BB', 0xC0, 0x01, 0x02, 0x02, mode)
    
    def kg_cmd_feedback_get_blink_mode(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x03, 0x01)
    def kg_cmd_feedback_set_blink_mode(self, mode):
        return struct.pack('<4BB', 0xC0, 0x01, 0x03, 0x02, mode)
    def kg_cmd_feedback_get_piezo_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x03, 0x03, index)
    def kg_cmd_feedback_set_piezo_mode(self, index, mode, duration, frequency):
        return struct.pack('<4BBBBH', 0xC0, 0x05, 0x03, 0x04, index, mode, duration, frequency)
    def kg_cmd_feedback_get_vibe_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x03, 0x05, index)
    def kg_cmd_feedback_set_vibe_mode(self, index, mode, duration):
        return struct.pack('<4BBBB', 0xC0, 0x03, 0x03, 0x06, index, mode, duration)
    def kg_cmd_feedback_get_rgb_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x03, 0x07, index)
    def kg_cmd_feedback_set_rgb_mode(self, index, mode_red, mode_green, mode_blue):
        return struct.pack('<4BBBBB', 0xC0, 0x04, 0x03, 0x08, index, mode_red, mode_green, mode_blue)
    
    def kg_cmd_motion_get_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x04, 0x01, index)
    def kg_cmd_motion_set_mode(self, index, mode):
        return struct.pack('<4BBB', 0xC0, 0x02, 0x04, 0x02, index, mode)
    
    kg_rsp_system_ping = KeygloveEvent()
    kg_rsp_system_reset = KeygloveEvent()
    
    kg_rsp_touch_get_mode = KeygloveEvent()
    kg_rsp_touch_set_mode = KeygloveEvent()
    
    kg_rsp_feedback_get_blink_mode = KeygloveEvent()
    kg_rsp_feedback_set_blink_mode = KeygloveEvent()
    kg_rsp_feedback_get_piezo_mode = KeygloveEvent()
    kg_rsp_feedback_set_piezo_mode = KeygloveEvent()
    kg_rsp_feedback_get_vibe_mode = KeygloveEvent()
    kg_rsp_feedback_set_vibe_mode = KeygloveEvent()
    kg_rsp_feedback_get_rgb_mode = KeygloveEvent()
    kg_rsp_feedback_set_rgb_mode = KeygloveEvent()
    
    kg_rsp_motion_get_mode = KeygloveEvent()
    kg_rsp_motion_set_mode = KeygloveEvent()
    
    kg_evt_protocol_error = KeygloveEvent()
    
    kg_evt_system_boot = KeygloveEvent()
    kg_evt_system_ready = KeygloveEvent()
    kg_evt_system_error = KeygloveEvent()
    
    kg_evt_touch_mode = KeygloveEvent()
    kg_evt_touch_status = KeygloveEvent()
    
    kg_evt_feedback_blink_mode = KeygloveEvent()
    kg_evt_feedback_piezo_mode = KeygloveEvent()
    kg_evt_feedback_vibe_mode = KeygloveEvent()
    kg_evt_feedback_rgb_mode = KeygloveEvent()
    
    kg_evt_motion_mode = KeygloveEvent()
    kg_evt_motion_data = KeygloveEvent()
    
    on_busy = KeygloveEvent()
    on_idle = KeygloveEvent()
    on_timeout = KeygloveEvent()
    on_before_tx_command = KeygloveEvent()
    on_tx_command_complete = KeygloveEvent()

    kgapi_rx_buffer = []
    kgapi_rx_expected_length = 0
    busy = False
    debug = False

    def send_command(self, ser, packet):
        if self.debug: print('=>[ ' + ' '.join(['%02X' % ord(b) for b in packet ]) + ' ]')
        self.on_before_tx_command()
        self.busy = True
        self.on_busy()
        ser.write(packet)
        self.on_tx_command_complete()

    def check_activity(self, inobj, timeout=0):
        # inobj should be PySerial "serial" object
        if timeout > 0:
            inobj.timeout = timeout
            while 1:
                x = inobj.read()
                if len(x) > 0:
                    self.parse(ord(x))
                else: # timeout
                    self.busy = False
                    self.on_idle()
                    self.on_timeout()
                if not self.busy: # finished
                    break
        else:
            while inobj.inWaiting(): self.parse(ord(inobj.read()))
        return self.busy

    def parse(self, b):
        if len(self.kgapi_rx_buffer) == 0 and (b == 0xC0 or b == 0x80):
            self.kgapi_rx_buffer.append(b)
        elif len(self.kgapi_rx_buffer) == 1:
            self.kgapi_rx_buffer.append(b)
            self.kgapi_rx_expected_length = 4 + (self.kgapi_rx_buffer[0] & 0x07) + self.kgapi_rx_buffer[1]
        elif len(self.kgapi_rx_buffer) > 1:
            self.kgapi_rx_buffer.append(b)

        """
        Keyglove packet structure (as of 2012-11-07):
            Byte 0:     8 bits, Message Type             0xC0 = command/response, 0x80 = event
            Byte 1:     8 bits, Length                   Payload length
            Byte 2:     8 bits, Class ID (CID)           Packet class
            Byte 3:     8 bits, Command ID (CMD)         Packet ID
            Bytes 4-n:  0 - 250 Bytes, Payload (PL)      Up to 250 bytes of payload
        """

        #print('%02X: %d, %d' % (b, len(self.kgapi_rx_buffer), self.kgapi_rx_expected_length))
        if self.kgapi_rx_expected_length > 0 and len(self.kgapi_rx_buffer) == self.kgapi_rx_expected_length:
            if self.debug: print('<=[ ' + ' '.join(['%02X' % b for b in self.kgapi_rx_buffer ]) + ' ]')
            packet_type, payload_length, packet_class, packet_command = self.kgapi_rx_buffer[:4]
            self.kgapi_rx_payload = b''.join(chr(i) for i in self.kgapi_rx_buffer[4:])
            self.kgapi_rx_buffer = []
            if packet_type & 0xC0 == 0xC0:
                # 0xC0 = response packet after a command that has just been sent
                if packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_rsp_system_ping
                        runtime, = struct.unpack('<L', self.kgapi_rx_payload[:4])
                        self.kg_rsp_system_ping({ 'runtime': runtime })
                    elif packet_command == 2: # kg_rsp_system_reset
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_rsp_system_reset({ 'result': result })
                elif packet_class == 2: # TOUCH
                    if packet_command == 1: # kg_rsp_touch_get_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.kg_rsp_touch_get_mode({ 'mode': mode })
                    elif packet_command == 2: # kg_rsp_touch_set_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_rsp_touch_set_mode({ 'result': result })
                elif packet_class == 3: # FEEDBACK
                    if packet_command == 1: # kg_rsp_feedback_get_blink_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.kg_rsp_feedback_get_blink_mode({ 'mode': mode })
                    elif packet_command == 2: # kg_rsp_feedback_set_blink_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_rsp_feedback_set_blink_mode({ 'result': result })
                    elif packet_command == 3: # kg_rsp_feedback_get_piezo_mode
                        mode, duration, frequency, = struct.unpack('<BBH', self.kgapi_rx_payload[:4])
                        self.kg_rsp_feedback_get_piezo_mode({ 'mode': mode, 'duration': duration, 'frequency': frequency })
                    elif packet_command == 4: # kg_rsp_feedback_set_piezo_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_rsp_feedback_set_piezo_mode({ 'result': result })
                    elif packet_command == 5: # kg_rsp_feedback_get_vibe_mode
                        mode, duration, = struct.unpack('<BB', self.kgapi_rx_payload[:2])
                        self.kg_rsp_feedback_get_vibe_mode({ 'mode': mode, 'duration': duration })
                    elif packet_command == 6: # kg_rsp_feedback_set_vibe_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_rsp_feedback_set_vibe_mode({ 'result': result })
                    elif packet_command == 7: # kg_rsp_feedback_get_rgb_mode
                        mode_red, mode_green, mode_blue, = struct.unpack('<BBB', self.kgapi_rx_payload[:3])
                        self.kg_rsp_feedback_get_rgb_mode({ 'mode_red': mode_red, 'mode_green': mode_green, 'mode_blue': mode_blue })
                    elif packet_command == 8: # kg_rsp_feedback_set_rgb_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_rsp_feedback_set_rgb_mode({ 'result': result })
                elif packet_class == 4: # MOTION
                    if packet_command == 1: # kg_rsp_motion_get_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.kg_rsp_motion_get_mode({ 'mode': mode })
                    elif packet_command == 2: # kg_rsp_motion_set_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_rsp_motion_set_mode({ 'result': result })
                self.busy = False
                self.on_idle()
            elif packet_type & 0xC0 == 0x80:
                # 0x80 = event packet
                if packet_class == 0: # PROTOCOL
                    if packet_command == 1: # kg_evt_protocol_error
                        code, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_evt_protocol_error({ 'code': code })
                elif packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_evt_system_boot
                        self.kg_evt_system_boot({  })
                    elif packet_command == 2: # kg_evt_system_ready
                        self.kg_evt_system_ready({  })
                    elif packet_command == 3: # kg_evt_system_error
                        code, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.kg_evt_system_error({ 'code': code })
                elif packet_class == 2: # TOUCH
                    if packet_command == 1: # kg_evt_touch_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.kg_evt_touch_mode({ 'mode': mode })
                    elif packet_command == 2: # kg_evt_touch_status
                        status_len, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        status_data = [ord(b) for b in self.kgapi_rx_payload[1:]]
                        self.kg_evt_touch_status({ 'status': status_data })
                elif packet_class == 3: # FEEDBACK
                    if packet_command == 1: # kg_evt_feedback_blink_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.kg_evt_feedback_blink_mode({ 'mode': mode })
                    elif packet_command == 2: # kg_evt_feedback_piezo_mode
                        index, mode, duration, frequency, = struct.unpack('<BBBH', self.kgapi_rx_payload[:5])
                        self.kg_evt_feedback_piezo_mode({ 'index': index, 'mode': mode, 'duration': duration, 'frequency': frequency })
                    elif packet_command == 3: # kg_evt_feedback_vibe_mode
                        index, mode, duration, = struct.unpack('<BBB', self.kgapi_rx_payload[:3])
                        self.kg_evt_feedback_vibe_mode({ 'index': index, 'mode': mode, 'duration': duration })
                    elif packet_command == 4: # kg_evt_feedback_rgb_mode
                        index, mode_red, mode_green, mode_blue, = struct.unpack('<BBBB', self.kgapi_rx_payload[:4])
                        self.kg_evt_feedback_rgb_mode({ 'index': index, 'mode_red': mode_red, 'mode_green': mode_green, 'mode_blue': mode_blue })
                elif packet_class == 4: # MOTION
                    if packet_command == 1: # kg_evt_motion_mode
                        index, mode, = struct.unpack('<BB', self.kgapi_rx_payload[:2])
                        self.kg_evt_motion_mode({ 'index': index, 'mode': mode })
                    elif packet_command == 2: # kg_evt_motion_data
                        index, flags, data_len, = struct.unpack('<BBB', self.kgapi_rx_payload[:3])
                        data_data = [ord(b) for b in self.kgapi_rx_payload[3:]]
                        self.kg_evt_motion_data({ 'index': index, 'flags': flags, 'data': data_data })

# ============================ EOF ===============================
