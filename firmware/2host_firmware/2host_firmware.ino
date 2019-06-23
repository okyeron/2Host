/* Create a "class compliant " USB to 1 MIDI IN and 1 MIDI OUT interface.

   MIDI receive (6N138 optocoupler) input circuit and series resistor
   outputs need to be connected to Serial1, Serial2 and Serial3.

   You must select MIDIx4 from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#include <MIDI.h>

// Create the Serial MIDI ports
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);

// A variable to know how long the LED has been turned on
elapsedMillis ledOnMillis;


void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT); // LED pin
  digitalWrite(13, LOW);
  MIDI1.begin(MIDI_CHANNEL_OMNI);
  MIDI1.turnThruOff();
  digitalWriteFast(13, HIGH); // LED blink on startup
  delay(100);
  digitalWriteFast(13, LOW);  // LED off
  
}


void loop() {
  bool activity = false;

  if (MIDI1.read()) {
    // get a MIDI IN1 (Serial) message
    byte type = MIDI1.getType();
    byte channel = MIDI1.getChannel();
    byte data1 = MIDI1.getData1();
    byte data2 = MIDI1.getData2();

    if (type == midi::Clock) {
      // no led activity on clock
      Serial.println("clock");
      activity = false;
      ledOnMillis = 0;
    } else {
      activity = true;
    }

   // forward the message to USB MIDI virtual cable #0
    if (type != midi::SystemExclusive) {
      // Normal messages, simply give the data to the usbMIDI.send()
      usbMIDI.send(type, data1, data2, channel, 0);
    } else {
      // SysEx messages are special.  The message length is given in data1 & data2
      unsigned int SysExLength = data1 + data2 * 256;
      //usbMIDI.sendSysEx(SysExLength, MIDI1.getSysExArray(), true, 0);
    }
  }

 
  if (usbMIDI.read()) {
    // get the USB MIDI message, defined by these 5 numbers (except SysEX)
    byte type = usbMIDI.getType();
    byte channel = usbMIDI.getChannel();
    byte data1 = usbMIDI.getData1();
    byte data2 = usbMIDI.getData2();
    //byte cable = usbMIDI.getCable();
    
    if (type == usbMIDI.Clock) {
      // no led activity on clock
      activity = false;
      ledOnMillis = 0;
    } else {
      activity = true;
    }

   // forward this message to Serial MIDI OUT ports
    if (type != usbMIDI.SystemExclusive) {
      // Normal messages, first we must convert usbMIDI's type (an ordinary
      // byte) to the MIDI library's special MidiType.
      midi::MidiType mtype = (midi::MidiType)type;

      // Then simply give the data to the MIDI library send()
      MIDI1.send(mtype, data1, data2, channel);
      
    } else {
      // SysEx messages are special.  The message length is given in data1 & data2
      unsigned int SysExLength = data1 + data2 * 256;
          //MIDI1.sendSysEx(SysExLength, usbMIDI.getSysExArray(), true);

    }
  }

  // blink the LED when any activity has happened
  if (activity) {
    digitalWriteFast(13, HIGH); // LED on
    ledOnMillis = 0;
  }
  if (ledOnMillis > 15) {
    digitalWriteFast(13, LOW);  // LED off
  }

}
