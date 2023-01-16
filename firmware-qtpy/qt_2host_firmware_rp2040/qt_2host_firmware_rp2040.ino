/*  
 *  QT-2host - USBMIDI host-to-host adapter.
 *  steven noreyko - revised 2022 
 *  version 1.5
 *  
 *  Create a USBMIDI to HARDWARE MIDI interface.
 *  
 *  Select Adafruit QT Py RP2040 from the "Tools > Board" menu
 *  Select Adafruit TinyUSB from the "Tools > USB Stack" menu
 *  
 */

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <Adafruit_NeoPixel.h> 
#include <elapsedMillis.h>

const int NEOPIXPIN = 12;
const int NEOPWRPIN = 11;

Adafruit_USBD_MIDI usb_midi;

// Create the Serial and USB MIDI ports
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, USBMIDI);      // USBMIDI is USB MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, HWMIDI);           // HWMIDI is Hardware MIDI

Adafruit_NeoPixel onePixel = Adafruit_NeoPixel(1, NEOPIXPIN, NEO_GRB + NEO_KHZ800);

// A variable to know how long the LED has been turned on
elapsedMillis ledOnMillis;

bool activity = false;

// DEVICE INFO FOR ADAFRUIT M0 or M4 
char mfgstr[32] = "denki-oto";
char prodstr[32] = "QT-2host";

void setup() {
    USBDevice.setManufacturerDescriptor(mfgstr);
    USBDevice.setProductDescriptor(prodstr);
    
    pinMode(NEOPWRPIN, OUTPUT);	// NEOPWR Pin
    digitalWrite(NEOPWRPIN, HIGH);	// Turn NEOPWR ON
   
    pinMode(20, OUTPUT);
    // digitalWrite(20, HIGH);
    pinMode(5, INPUT);	
	  
    HWMIDI.begin(MIDI_CHANNEL_OMNI);
    USBMIDI.begin(MIDI_CHANNEL_OMNI);
    HWMIDI.turnThruOff();
    USBMIDI.turnThruOff();

    // wait until device mounted
    if (!TinyUSBDevice.mounted()){
    	delay(1000);
    }

    onePixel.begin();                           // Start the NeoPixel object
    onePixel.clear();                           // Set NeoPixel color to black (0,0,0)
    onePixel.setBrightness(50);     // Affects all subsequent settings

    onePixel.setPixelColor(0, 100, 0, 0);
    onePixel.show();
    delay(500);
    onePixel.clear();
    onePixel.show();                            // Update the pixel state
    Serial.begin(115200);
//     Serial.println("MIDI Test");

}


void loop() {
    activity = false;
    
    int r=0, g=0, b=0;	// QTPy LED color for activity

    // READ HARDWARE MIDI
    if   (HWMIDI.read()) {
        // get a MIDI IN1 (Serial) message
        midi::MidiType type = HWMIDI.getType();
        byte channel = HWMIDI.getChannel();
        byte data1 = HWMIDI.getData1();
        byte data2 = HWMIDI.getData2();

        if (type == midi::Clock) {
            // no led activity on clock
//             Serial.println("clock");
            activity = false;
            // ledOnMillis = 0;
        } else {
//             Serial.println("read activity");
            r=0, g=100, b=0;
            activity = true;
        }

		// send the message to USB MIDI 
        if (type != midi::SystemExclusive) {
            // Normal messages, simply give the data to the USBMIDI.send()
            USBMIDI.send(type, data1, data2, channel);
//             Serial.println("usbmidi send");
        } else {
            // SysEx messages are special.  The message length is given in data1 & data2
            unsigned int SysExLength = data1 + data2 * 256;
            USBMIDI.sendSysEx(SysExLength, HWMIDI.getSysExArray(), true);
        }
    }

    // READ USBMIDI
    while(usb_midi.available()){
        if (USBMIDI.read()) {
            // get the USB MIDI message  (except SysEX)
            midi::MidiType type = USBMIDI.getType();
            byte channel = USBMIDI.getChannel();
            byte data1 = USBMIDI.getData1();
            byte data2 = USBMIDI.getData2();
 
            if (type == midi::Clock) {
                // no led activity on clock
                activity = false;
                // ledOnMillis = 0;
            } else {
                r=0, g=0, b=100;
                activity = true;
            }

            // send this message to Serial MIDI OUT
            if (type != midi::SystemExclusive) {
                // Normal messages, first we must convert USBMIDI's type (an ordinary
                // byte) to the MIDI library's special MidiType.
                midi::MidiType mtype = (midi::MidiType)type;

                // Then simply give the data to the MIDI library send()
                HWMIDI.send(mtype, data1, data2, channel);
            } else {
                // SysEx messages are special.  The message length is given in data1 & data2
                unsigned int SysExLength = data1 + data2 * 256;
                HWMIDI.sendSysEx(SysExLength, USBMIDI.getSysExArray(), true);
            }
        }
    }

    // blink the LED when any activity has happened
    if (activity) {
        onePixel.setPixelColor(0, r, g, b);
        onePixel.show();
        ledOnMillis = 0;
    }
    if (ledOnMillis > 50 ) {
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
