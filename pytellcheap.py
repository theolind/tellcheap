"""
    pytellcheap is a set of Python functions to construct and send commands
    to 433Mhz RF controlled wall outlets. The commands are transmitted
    using an Arduino board with a 433Mhz transmitter connected.    

    Copyright (C) 2015 Theodor Lindquist

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
"""

import serial
import time
import threading

#
# Command format: STTTTTTTTTTTTTTTTTTTTTTTTTTGOCCNNE
#

# settings
set_delay = 1.0         # delay between commands

# state vars
_transmitters = []   # store configured transmitters
_ser = None          # serial object

# table for device number lookup
_device_number = ["00", "01", "10", "11"]

# add transmitter, id = transmitter code, channel = channel (01 for proove)
def add_transmitter(transmitter_id, channel = "01"):
    global _transmitters
    _transmitters.append(Transmitter(transmitter_id, channel))

# return a count of all added devices
def count_devices():
    global _transmitters
    return len(_transmitters)*3; # We have 3 devices per transmitter

# connect serial
def connect(port):
    global _ser
    _ser = serial.Serial(port, 115200)
    
# send command, data is a string
def send(data):
    global _ser
    _ser.write(data.encode())
    time.sleep(set_delay)

# turn device on
def on(device):
    command = build_command(True, device)
    send(command)

# trun device off
def off(device):
    command = build_command(False, device)
    send(command)

# construct a command string
def build_command(on, device):
    command = "S"
    command += _transmitters[device//3].id
    command += "1"
    command += str(int(not on))
    command += _transmitters[device//3].channel
    command += _device_number[device%3]
    command += "P"
    return command

# Store transmitter data
# 26 bit transmitter id
# 2 bit channel, 01 = Proove ?
class Transmitter:
    def __init__(self, transmitter_id = None, channel = None):
        self.id = transmitter_id
        self.channel = channel

# run tests
def run_test():
    add_transmitter("11100110000100110010000001", "01")
    connect("/dev/ttyACM0")
    time.sleep(3)
    on(0)
    on(1)
    on(2)
    off(0)
    off(1)
    off(2)
    add_transmitter("11100110000100110010000001", "01")
    on(3)
    on(4)
    on(5)
    off(3)
    off(4)
    off(5)
    print("Test completed")

# if file is executed, run tests
if __name__ == "__main__":
    run_test()

