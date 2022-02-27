# 2host

2host (pronounced “Toast”) is a DIY USBMIDI host-to-host adapter. It creates a really short hardware MIDI bridge between two USBMIDI Host devices.

The first version of 2host uses 2 [TeensyLC](https://www.pjrc.com/store/teensylc.html).

The second version uses 2 Adafruit QtPy (SAMD21) microcontrollers.


Here are a few examples of USBMIDI hosts which could be connected together:

hardware    
- norns 
- organelle 
- ansible (needs testing) 
- ipad (with Camera Connection kit) 
- raspberry pi (pisound, etc.) 

software on any computer  
- ableton 
- orca  
- VCV Rack 
- etc. 


## QT-2host

First generation Qt-2host include 1/8in hardware midi jacks - these are set to the `Type B` 1/8in minijack configuration.


## Build and BOM

[Teensy version BOM](hardware/BOM.md)

The QtPy version is only available as a pre-assembled PCB. You just need to add QtPy's.


## PCBs

PCBs are available for sale in my shop
