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

int MIDI_OUT_CH = 9;  // pick your midi output channel here

Adafruit_NeoPixel onePixel = Adafruit_NeoPixel(1, 11, NEO_GRB + NEO_KHZ800);

// A variable to know how long the LED has been turned on
elapsedMillis ledOnMillis;
elapsedMillis button_msec = 0;
unsigned long buttonInterval = 120 ;

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

bool padLeftOn = false;
bool padRightOn = false;
bool padUpOn = false;
bool padDownOn = false;
bool buttonXOn = false;
bool buttonYOn = false;
bool buttonAOn = false;
bool buttonBOn = false;

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

int lastJoyX = 32;  // resting value of joyX
int joyXCCNum = 71; // VCF or whatever you assign in synth software
int joyXCCVal = 0;
int lastJoyY = 32;  // resting value of joyY
int joyYCCNum = 72; // VCA
int joyYCCVal = 0;

int octave = 12;  // note offset value, used to change octaves
const char* controller[] = {"unknown ", "NUNCHUCK", "WIICLASSIC", "GuitarHeroController","GuitarHeroWorldTourDrums","DrumController","DrawsomeTablet","Turntable"};

void setup() {
	Serial.begin(115200);

	pad_with_nulls(mfgstr, 32);
	pad_with_nulls(prodstr, 32);
	USBDevice.setManufacturerDescriptor(mfgstr);
	USBDevice.setProductDescriptor(prodstr);
  	

	MIDI1.begin(MIDI_CHANNEL_OMNI);
	MIDI2.begin(MIDI_CHANNEL_OMNI);
	MIDI1.turnThruOff();
	MIDI2.turnThruOff();


	nunchuck1.begin();

	// wait until device mounted
//	while ( !USBDevice.mounted() ) delay(1);

	delay(1500);
	Serial.println(controller[nunchuck1.type]);

	
	onePixel.begin();             // Start the NeoPixel object
	onePixel.clear();             // Set NeoPixel color to black (0,0,0)
	onePixel.setBrightness(50);   // Affects all subsequent settings

	onePixel.setPixelColor(0, 100, 0, 0); // blink an led
	onePixel.show();
	delay(200);
	onePixel.clear();
	onePixel.show();              // Update the pixel state
}


void loop() {
  activity = false;
  int r=0, g=0, b=100;
 

  	nunchuck1.readData();    // Read inputs and update maps
//	nunchuck1.printInputs(); // Print all inputs
//	Serial.println(nunchuck1.type);
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
			
			// M8 mapping
			if(joyX!=lastJoyX){
				joyXCCVal=map(joyX, 190, 255, 0, 127); // remap to bigger range
				MIDI2.sendControlChange(joyXCCNum, joyXCCVal, MIDI_OUT_CH);
				lastJoyX=joyX;
			}
			if(joyY!=lastJoyY){
				joyYCCVal=map(joyY, 190, 255, 0, 127); // remap to bigger range
				MIDI2.sendControlChange(joyYCCNum, joyYCCVal, MIDI_OUT_CH);
				lastJoyY=joyY;
			}
			
			
			
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

			// M8 mapping
			if(joyXLeft!=lastJoyX){
//				Serial.println(joyXLeft);
				joyXCCVal=map(joyX, 0, 128, 0, 127); // remap to bigger range
				MIDI1.sendControlChange(joyXCCNum, joyXLeft, MIDI_OUT_CH);
				MIDI2.sendControlChange(joyXCCNum, joyXLeft, MIDI_OUT_CH);
				lastJoyX=joyXLeft;
			}
			if(joyYLeft!=lastJoyY){
//				Serial.println(joyYLeft);
				joyYCCVal=map(joyY, 0, 128, 0, 127); // remap to bigger range
				MIDI1.sendControlChange(joyYCCNum, joyYLeft, MIDI_OUT_CH);
				MIDI2.sendControlChange(joyYCCNum, joyYLeft, MIDI_OUT_CH);
				lastJoyY=joyYLeft;
			}

		if (button_msec >= buttonInterval){

			// M8 KEY MAPPING
			if(padLeftRight==0){ // left
				MIDI1.sendNoteOn(4, 127, MIDI_OUT_CH);
				MIDI2.sendNoteOn(4, 127, MIDI_OUT_CH);
				padRightOn = true;
			} else if (padLeftRight==255){ // right
				MIDI1.sendNoteOn(5, 127, MIDI_OUT_CH);
				MIDI2.sendNoteOn(5, 127, MIDI_OUT_CH);
				padLeftOn = true;
			}
			if (padRightOn && padLeftRight==128) {
				MIDI1.sendNoteOff(4, 0, MIDI_OUT_CH);
				MIDI2.sendNoteOff(4, 0, MIDI_OUT_CH);
				padRightOn = false;
			}
			if (padLeftOn && padLeftRight==128) {
				MIDI1.sendNoteOff(5, 0, MIDI_OUT_CH);
				MIDI2.sendNoteOff(5, 0, MIDI_OUT_CH);
				padLeftOn = false;
			}
			
			if(padUpDown==0){ // down
				MIDI1.sendNoteOn(7, 127, MIDI_OUT_CH);
				MIDI2.sendNoteOn(7, 127, MIDI_OUT_CH);
				padUpOn = true;
			} else if (padUpDown==255){ // up
				MIDI1.sendNoteOn(6, 127, MIDI_OUT_CH);
				MIDI2.sendNoteOn(6, 127, MIDI_OUT_CH);
				padDownOn = true;
			}
			if (padUpOn && padUpDown==128) {
				MIDI1.sendNoteOff(7, 0, MIDI_OUT_CH);
				MIDI2.sendNoteOff(7, 0, MIDI_OUT_CH);
				padUpOn = false;
			}
			if (padDownOn && padUpDown==128) {
				MIDI1.sendNoteOff(6, 0, MIDI_OUT_CH);
				MIDI2.sendNoteOff(6, 0, MIDI_OUT_CH);
				padDownOn = false;
			}
			
			if(buttonY==255 && !buttonYOn){ 			// Y  Option
				MIDI1.sendNoteOn(3, 127, MIDI_OUT_CH);
				MIDI2.sendNoteOn(3, 127, MIDI_OUT_CH);
				buttonYOn = true;
			}
			if (buttonYOn && buttonY==0) {
				MIDI1.sendNoteOff(3, 0, MIDI_OUT_CH);
				MIDI2.sendNoteOff(3, 0, MIDI_OUT_CH);
				buttonYOn = false;
			}
			
			if(buttonX==255 && !buttonXOn){ 			// X Edit
				MIDI1.sendNoteOn(2, 127, MIDI_OUT_CH);
				MIDI2.sendNoteOn(2, 127, MIDI_OUT_CH);
				buttonXOn = true;
			}
			if (buttonXOn && buttonX==0) {
				MIDI1.sendNoteOff(2, 0, MIDI_OUT_CH);
				MIDI2.sendNoteOff(2, 0, MIDI_OUT_CH);
				buttonXOn = false;
			}
			
			if(buttonB==255 && !buttonBOn){ 			// B - Shift
				MIDI1.sendNoteOn(1, 127, MIDI_OUT_CH);
				MIDI2.sendNoteOn(1, 127, MIDI_OUT_CH);
				buttonBOn = true;
			}
			if (buttonBOn && buttonB==0) {
				MIDI1.sendNoteOff(1, 0, MIDI_OUT_CH);
				MIDI2.sendNoteOff(1, 0, MIDI_OUT_CH);
				buttonBOn = false;
			}
			
			if(buttonA==255 && !buttonAOn){ 			// A - Play
				MIDI1.sendNoteOn(0, 127, MIDI_OUT_CH);
				MIDI2.sendNoteOn(0, 127, MIDI_OUT_CH);
				buttonAOn = true;
			}
			if (buttonAOn && buttonA==0) {
				MIDI1.sendNoteOff(0, 0, MIDI_OUT_CH);
				MIDI2.sendNoteOff(0, 0, MIDI_OUT_CH);
				buttonAOn = false;
			}
		button_msec = 0;
	}



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
