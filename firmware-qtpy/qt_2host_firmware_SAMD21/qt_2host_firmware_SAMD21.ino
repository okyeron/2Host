/*  
 *  QT-2host - USBMIDI host-to-host adapter.
 *  steven noreyko - updated March 2023
 *  version 1.6
 *  
 *  Create a USBMIDI to HARDWARE MIDI interface.
 *  
 *  Select QT PY M0 (SAMD21) from the "Tools > Board > Adafruit SAMD (32 bits ARM Cortex...)" menu
 *  
 *  Select TinyUSB from the "Tools > USB Stack" menu
 *  
 */

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <Adafruit_NeoPixel.h> 
#include <elapsedMillis.h>

Adafruit_USBD_MIDI usb_midi;

// Create the Serial and USB MIDI ports
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);           // MIDI1 is Hardware MIDI
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI2);      // MIDI2 is USB MIDI

Adafruit_NeoPixel onePixel = Adafruit_NeoPixel(1, 11, NEO_GRB + NEO_KHZ800);

// A variable to know how long the LED has been turned on
elapsedMillis ledOnMillis;

bool activity = false;

// DEVICE INFO FOR TinyUSB
char mfgstr[32] = "denki-oto";
char prodstr[32] = "QT-2host";

void setup() {
    USBDevice.setManufacturerDescriptor(mfgstr);
    USBDevice.setProductDescriptor(prodstr);
    
    MIDI1.begin(MIDI_CHANNEL_OMNI);
    MIDI2.begin(MIDI_CHANNEL_OMNI);
    MIDI1.turnThruOff();
    MIDI2.turnThruOff();

    Serial.begin(115200);

    // wait until device mounted
    // while ( !USBDevice.mounted() ) delay(1);
    if (!TinyUSBDevice.mounted()){
    	delay(1000);
    }

    onePixel.begin();                           // Start the NeoPixel object
    onePixel.clear();                           // Set NeoPixel color to black (0,0,0)
    onePixel.setBrightness(50);     // Affects all subsequent settings

    onePixel.setPixelColor(0, 100, 0, 0);
    onePixel.show();
    delay(200);
    onePixel.clear();
    onePixel.show();                            // Update the pixel state

}


void loop() {
    activity = false;
    
    int r=0, g=0, b=100;	// QTPy LED color for activity

    // READ HARDWARE MIDI
    if (MIDI1.read()) {
        // get a MIDI IN1 (Serial) message
        midi::MidiType type = MIDI1.getType();
        byte channel = MIDI1.getChannel();
        byte data1 = MIDI1.getData1();
        byte data2 = MIDI1.getData2();

        if (type == midi::Clock) {
            // no led activity on clock
            //Serial.println("clock");
            activity = false;
            ledOnMillis = 0;
        } else {
            activity = true;
        }

		// send the message to USB MIDI 
        if (type != midi::SystemExclusive) {
            // Normal messages, simply give the data to the MIDI2.send()
            MIDI2.send(type, data1, data2, channel);
        } else {
            // SysEx messages are special.  The message length is given in data1 & data2
            // unsigned int SysExLength = data1 + data2 * 256;
            
            //MIDI2.sendSysEx(SysExLength, MIDI.getSysExArray(), true, 0);
        }
    }

    // READ USBMIDI
    while(usb_midi.available()){
        if (MIDI2.read()) {
            // get the USB MIDI message  (except SysEX)
            midi::MidiType type = MIDI2.getType();
            byte channel = MIDI2.getChannel();
            byte data1 = MIDI2.getData1();
            byte data2 = MIDI2.getData2();
        
            if (type == midi::Clock) {
                // no led activity on clock
                activity = false;
                ledOnMillis = 0;
            } else {
                activity = true;
            }

            // send this message to Serial MIDI OUT
            if (type != midi::SystemExclusive) {
                // Normal messages, first we must convert MIDI2's type (an ordinary
                // byte) to the MIDI library's special MidiType.
                midi::MidiType mtype = (midi::MidiType)type;

                // Then simply give the data to the MIDI library send()
                MIDI1.send(mtype, data1, data2, channel);
            
            } else {
                // SysEx messages are special.  The message length is given in data1 & data2
                // unsigned int SysExLength = data1 + data2 * 256;
                //MIDI1.sendSysEx(SysExLength, MIDI2.getSysExArray(), true);
            }
        }
    }

    // blink the LED when any activity has happened
    if (activity) {
        onePixel.setPixelColor(0, r, g, b);
        onePixel.show();
        ledOnMillis = 0;
    }
    if (ledOnMillis > 20 ) {
        onePixel.clear(); 
        onePixel.show();
    }
}

// ***************************************************************************
// ** HELPERS                                                               **
// ***************************************************************************

// Pad a string of length 'len' with nulls
void pad_with_nulls(char* s, int len) {
    int l = strlen(s);
    for( int i=l;i<len; i++) {
        s[i] = '\0';
    }
}
