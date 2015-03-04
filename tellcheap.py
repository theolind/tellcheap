""" Support for Arduino Controlled Proove switch """
import logging
from time import sleep

from homeassistant.const import ATTR_FRIENDLY_NAME
from homeassistant.helpers import ToggleDevice

import config.custom_components.switch.pytellcheap as tellcheap

def setup_platform(hass, config, add_devices_callback, discovery_info = None):
    # Open a connection to the serialport
    tellcheap.connect(config['serialport'])

    # find and add all configured transmitters
    for transmitter in config['transmitters']:
        tellcheap.add_transmitter(transmitter)

    # find and add all configured switches
    switches = []
    for switch in config['devices']:
        switch_id = next(iter(switch.keys()))
        switch_name = next(iter(switch.values()))
        switches.append(TellCheapSwitch(int(switch_id), switch_name))

    add_devices_callback(switches)

class TellCheapSwitch(ToggleDevice):
    """ represents a TellCheap switch within home assistant. """

    def __init__(self, device, name):
        self.device_id = device
        self.device_state = False
        self.device_name = name

    @property
    def unique_id(self):
        return self.device_id

    @property
    def name(self):
        return self.device_name

    @property
    def is_on(self):
        return self.device_state

    def turn_on(self, **kwargs):
        logging.getLogger(__name__).info("Turning device on")
        self.device_state = True
        tellcheap.on(self.device_id)

    def turn_off(self, **kwargs):
        logging.getLogger(__name__).info("Turning device off")
        self.device_state = False
        tellcheap.off(self.device_id)

