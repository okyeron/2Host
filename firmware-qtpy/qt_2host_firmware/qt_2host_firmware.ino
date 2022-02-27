/* Create a "class compliant " USB to 1 MIDI IN and 1 MIDI OUT interface.

   You must select TinyUSB from the  "Tools > USB Stack" menu

*/

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <Adafruit_NeoPixel.h> 
#include <elapsedMillis.h>
#include <WiiChuck.h>

// DEVICE INFO FOR ADAFRUIT M0 or M4 
char mfgstr[32] = "denki-oto";
char prodstr[32] = "QT-2host";

Accessory nunchuck1;

Adafruit_USBD_MIDI usb_midi;

// Create the Serial and USB MIDI ports
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);       // MIDI1 is Hardware MIDI
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI2);  // MIDI2 is USB MIDI

Adafruit_NeoPixel onePixel = Adafruit_NeoPixel(1, 11, NEO_GRB + NEO_KHZ800);

// A variable to know how long the LED has been turned on
elapsedMillis ledOnMillis;

bool activity = false;

// NUNCHUCK
int joyX;
int joyY;
int rollAngle;
int pitchAngle;
int accelX;
int accelY;
int accelZ;
int buttonZ;
int buttonY;
int buttonX;

// WIICLASSIC
int joyXLeft;
int joyYLeft;
int joyXRight;
int joyYRight;
int padLeftRight;
int padUpDown;
int buttonZR;
int buttonZL;
int buttonA;
int buttonB;
int buttonPlusMinus;
int buttonHome;
int triggerLeft;
int triggerRight;

// GUITAR
int whammyBar;
int minusButton;  // drop an octave w each press
int plusButton;  // up and octave w each press
int strumDown;
int strumUp;
int fretButtons[5];
bool minusButtonOn = 0;
bool plusButtonOn = 0;
bool strumDownOn = 0;
bool strumUpOn = 0;
bool fretButtonOn[] = {0, 0, 0, 0, 0};

int octave = 12;  // note offset value, used to change octaves

const char* controller[] = {"unknown ", "NUNCHUCK", "WIICLASSIC", "GuitarHeroController","GuitarHeroWorldTourDrums","DrumController","DrawsomeTablet","Turntable"};

void setup() {
//	pad_with_nulls(mfgstr, 32);
//	pad_with_nulls(prodstr, 32);
	USBDevice.setManufacturerDescriptor(mfgstr);
	USBDevice.setProductDescriptor(prodstr);
  
	nunchuck1.begin();
	Serial.println(nunchuck1.type);
	
//	if (nunchuck1.type == Unknown) {
//		nunchuck1.type = NUNCHUCK;
//	}

	MIDI1.begin(MIDI_CHANNEL_OMNI);
	MIDI2.begin(MIDI_CHANNEL_OMNI);
	MIDI1.turnThruOff();
	MIDI2.turnThruOff();

	Serial.begin(115200);

	// wait until device mounted
	while ( !USBDevice.mounted() ) delay(1);

	onePixel.begin();             // Start the NeoPixel object
	onePixel.clear();             // Set NeoPixel color to black (0,0,0)
	onePixel.setBrightness(50);   // Affects all subsequent settings

	onePixel.setPixelColor(0, 100, 0, 0);
	onePixel.show();
	delay(200);
	onePixel.clear();
	onePixel.show();              // Update the pixel state
}


void loop() {
  activity = false;
  int r=0, g=0, b=100;
  
  	nunchuck1.readData();    // Read inputs and update maps
	nunchuck1.printInputs(); // Print all inputs

	switch(nunchuck1.type){
		case 0: 	// "unknown
			break;
		case 1:		// "NUNCHUCK"
			joyX = nunchuck1.values[0];
			joyY = nunchuck1.values[1];
			rollAngle = nunchuck1.values[2];
			pitchAngle = nunchuck1.values[3];
			accelX = nunchuck1.values[4];
			accelY = nunchuck1.values[5];
			accelZ = nunchuck1.values[6];
			buttonZ = nunchuck1.values[10];
			buttonY = nunchuck1.values[11];
			break;
		case 2:		// "WIICLASSIC"
			joyXLeft = nunchuck1.values[0];
			joyYLeft = nunchuck1.values[1];
			joyXRight = nunchuck1.values[2];
			joyYRight = nunchuck1.values[3];
			padLeftRight = nunchuck1.values[6];
			padUpDown = nunchuck1.values[7];
			buttonX = nunchuck1.values[8];
			buttonY = nunchuck1.values[9];
			buttonZL = nunchuck1.values[10];
			triggerLeft = nunchuck1.values[11];
			buttonA = nunchuck1.values[12];
			buttonB = nunchuck1.values[13];
			buttonPlusMinus = nunchuck1.values[14];
			buttonHome = nunchuck1.values[15];
			triggerRight = nunchuck1.values[17];
			buttonZR = nunchuck1.values[18];
			break;
		case 3:		// "GuitarHeroController" ## UNTESTED
			fretButtons[0] = nunchuck1.values[10];  // green
			fretButtons[1] = nunchuck1.values[11];  // red
			fretButtons[2] = nunchuck1.values[12];  // yellow
			fretButtons[3] = nunchuck1.values[13];  //blue
			fretButtons[4] = nunchuck1.values[14]; // orange
			minusButton = nunchuck1.values[6];
			plusButton = nunchuck1.values[16];
			strumDown = nunchuck1.values[7];
			strumUp = nunchuck1.values[7];
			whammyBar = nunchuck1.values[0];
			joyX = nunchuck1.values[20];
			joyY = nunchuck1.values[21];
			break;
		case 4:		// "GuitarHeroWorldTourDrums"
			break;
		case 5:		// "DrumController"
			break;
		case 6:		// "DrawsomeTablet"
			break;
		case 7:		// "Turntable"
			break;
		default:
			break;
	}
		

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

   // forward the message to USB MIDI virtual cable #0
    if (type != midi::SystemExclusive) {
      // Normal messages, simply give the data to the MIDI2.send()
      MIDI2.send(type, data1, data2, channel);
    } else {
      // SysEx messages are special.  The message length is given in data1 & data2
      unsigned int SysExLength = data1 + data2 * 256;
      //MIDI2.sendSysEx(SysExLength, MIDI.getSysExArray(), true, 0);
    }
  }

 
  if (MIDI2.read()) {
    // get the USB MIDI message, defined by these 5 numbers (except SysEX)
    midi::MidiType type = MIDI2.getType();
    byte channel = MIDI2.getChannel();
    byte data1 = MIDI2.getData1();
    byte data2 = MIDI2.getData2();
    //byte cable = MIDI2.getCable();
    
    if (type == midi::Clock) {
      // no led activity on clock
      activity = false;
      ledOnMillis = 0;
    } else {
      activity = true;
    }

   // forward this message to Serial MIDI OUT ports
    if (type != midi::SystemExclusive) {
      // Normal messages, first we must convert MIDI2's type (an ordinary
      // byte) to the MIDI library's special MidiType.
      midi::MidiType mtype = (midi::MidiType)type;

      // Then simply give the data to the MIDI library send()
      MIDI1.send(mtype, data1, data2, channel);
      
    } else {
      // SysEx messages are special.  The message length is given in data1 & data2
      unsigned int SysExLength = data1 + data2 * 256;
          //MIDI1.sendSysEx(SysExLength, MIDI2.getSysExArray(), true);
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
// **                                HELPERS                                **
// ***************************************************************************

// Pad a string of length 'len' with nulls
void pad_with_nulls(char* s, int len) {
  int l = strlen(s);
  for( int i=l;i<len; i++) {
    s[i] = '\0';
  }
}
