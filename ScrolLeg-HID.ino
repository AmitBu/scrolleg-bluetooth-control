/**
 * ScrolLeg - Arduino controlled bluetooth device, that lets you scroll with your legs
 * 
 * Device contains 2 buttons and WT12 bluetooth chip
 * Each buttons sends a different command to the blueooth chip,
 * One for scroll up, one for down.
 * 
 * 
 */

#include <avr/pgmspace.h>

/**
 * Up/Down button pin numbers
 */
#define UP_BUTTON     2
#define DOWN_BUTTON   4
/**
 * WT12 Reset pin connection
 */
#define WT12_RESET    7


/**
 * WT12 Setting
 * Will run on setup()
 */
const char wt12_init_0[] PROGMEM = "SET BT NAME ScrolLeg";
const char wt12_init_1[] PROGMEM = "SET BT CLASS 00540";
const char wt12_init_2[] PROGMEM = "SET BT SSP 3 0";
const char wt12_init_3[] PROGMEM = "SET CONTROL AUTOCALL 0011 500 HID";

#define NUM_INIT_MSG  4


// http://forum.arduino.cc/index.php?topic=272313.0
// Initialize flash commands
PGM_P const string_table[]  PROGMEM {
    wt12_init_0,
    wt12_init_1,
    wt12_init_2,
    wt12_init_3
};

char buffer[60]; // used for printing these init messages

// The Mouse HID command prefix, as hex array
const byte packetPrefix[] = { 0x9F, 0x05, 0xA1, 0x01 }; 


// Up/Down commands translated to hex array
byte MOUSE_UP[]   = { 0x10, 0x00, 0x00 };
byte MOUSE_DOWN[] = { 0xF0, 0x00, 0x00 };

/**
 * Communicates with BT chip in Serial
 * Sends the entire packet, each byte in `Serial.write`
 */
void sendMousePacket(byte * packetExtra) {  
  // TODO: Check why loop doesn't work here

  // Send the mouse HID packet prefix
  Serial.write((byte)packetPrefix[0]);
  Serial.write((byte)packetPrefix[1]);
  Serial.write((byte)packetPrefix[2]);
  Serial.write((byte)packetPrefix[3]);

  // Sends the actual mouse action
  Serial.write((byte)packetExtra[0]);
  Serial.write((byte)packetExtra[1]);
  Serial.write((byte)packetExtra[2]);
 }

/**
 * Scroll up command wrapper
 * Send BT scroll up packet
 */
void mouseScrollUp() {
  sendMousePacket(MOUSE_UP);
}

/**
 * Scroll down command wrapper
 * Send BT scroll up packet
 */
void mouseScrollDown() {
  sendMousePacket(MOUSE_DOWN);
}

void log(String message) {
  //Serial.println(message);
}

/**
 * Checks button press
 * Sends wanted commands to BT accrordingly
 */
void buttonActivity() {
    // Up button pressed
    if (digitalRead(UP_BUTTON) == HIGH) {
      log("Up pressed");
      mouseScrollUp();
    }
    // Down button pressed
    else if (digitalRead(DOWN_BUTTON) == HIGH) {
      log("Down pressed");
      mouseScrollDown();
    }
}

/**
 * Starting bluetooth chip actions
 */
void initBluetooth() {
    // Reset the WT12 module on startup, in case something gets stuck
    pinMode(WT12_RESET, OUTPUT);
    digitalWrite(WT12_RESET, HIGH);
    delay(10);
    digitalWrite(WT12_RESET, LOW);

    log("\n\n");
    delay(1000);

    // Runs the startup commands that we defined above, on the bluetooth chip
    for (byte i = 0; i < NUM_INIT_MSG; i++) {
        strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));
        Serial.println(buffer);
        delay(100);
    }
    
    delay(1000);
    Serial.flush();
    log("RESET");
}

void setup() {
  Serial.begin(115200);
    
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);

  initBluetooth();

}

void loop() {
  buttonActivity();

  delay(200);
}
