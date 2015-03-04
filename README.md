Home Assistant TellCheap
===

Use [Home Assistant](https://home-assistant.io), an Arduino and a cheap 433Mhz transmitter to control Proove Wall Outlets.

1. Arduino sketch
---
The Arduino Sketch should be uploaded to an Arduino board, you may need to change the output pins depending on your transmitter. I use a common three-pin 433Mhz transmitter with a ~17cm wire soldered to the transmitter as antenna.

If you want support for other devices than Proove Outlets, you need to change the pulse times in the Arduino sketch. More information can be found [here](http://tech.jolowe.se/home-automation-rf-protocols/)

2. Python Library
---
The Python library contains a set of functions to interface with the Arduino and transmitter.

An example of the usage follows:
    import pytellcheap
    pytellcheap.add_transmitter("11100110000100110010000001", "01")
    pytellcheap.connect("/dev/ttyACM0")
    pytellcheap.on(0)
    pytellcheap.off(0)

3. Home Assistant Platform
---
A platform for [Home Assistant](https://home-assistant.io) is also provided. To use it, move the files pytellcheap.py and tellcheap.py to the folder [home assistant root]/config/custom_components/switch/

The component must then be configured, this is done by the component to [home assistant root]/config/configuration.yaml. An example configuration to be appended to configuration.yaml is provided below:

    switch:
      platform: tellcheap
      serialport: '/dev/ttyACM0'
      devices:
        - 0: 'Living room'
        - 1: 'Bedside table'
        - 2: 'Kitchen'
      transmitters:
        - '11100110000100110010000001'

