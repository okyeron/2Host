# 2host

2host (pronounced “Toast”) is a DIY USBMIDI host-to-host adapter.

2host uses 2 inexpensive microcontrollers to create a really short hardware MIDI bridge between two USBMIDI Host devices.

The first version of 2Host used [TeensyLC](https://www.pjrc.com/store/teensylc.html)’s. A new revised version uses two [Adafruit QtPy's](https://www.adafruit.com/qtpy) 

Here are a few examples of USBMIDI hosts which could be connected together:

hardware    
- norns 
- organelle 
- ansible 
- ipad (with Camera Connection kit) 
- raspberry pi (pisound, etc.) 
- any computer

software on any computer  
- ableton 
- orca  
- VCV Rack 
- etc. 

# QTPy version

SMD assembled PCBs are available [for sale in my shop](https://www.denki-oto.com/store/p67/qtpy2host.html#/). 

Requires two [Adafruit QtPy's](https://www.adafruit.com/qtpy) or [QtPy RP2040's](https://www.adafruit.com/product/4900).

Alternately you can use the [Seeeduino XIAO](https://www.seeedstudio.com/Seeeduino-XIAO-Arduino-Microcontroller-SAMD21-Cortex-M0+-p-4426.html) but this is not well tested.


### Firmware

Firmware is available as a pre-compiled UF2 file. 

To load the firmware, connect the QTPy to your computer and then double click the reset button on the QTPy to drop it into bootloader mode. This should now show you a QTPY_BOOT drive on your computer desktop. Drag the UF2 file to QTPY_BOOT and it should dismount/reboot and come back as the QT-2Host.

See the [firmware-qtpy directory](/firmware-qtpy) for the appropriate firmware.


# Teensy version 

[BOM](/hardware-teensy/BOM.md)

See the [firmware-teensy directory](/firmware-teensy/2host_firmware) for firmware


