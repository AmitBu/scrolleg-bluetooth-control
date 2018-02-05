#include <avr/pgmspace.h>


// WT12 Settings
//------------------------------------------------------------------------------

const char wt12_init_0[] PROGMEM = "SET BT NAME Amit";
const char wt12_init_1[] PROGMEM = "SET BT PAIR *";

#define WT12_RESET  7
#define NUM_INIT_MSG  2

// http://forum.arduino.cc/index.php?topic=272313.0
// Initialize flash commands
PGM_P const string_table[]  PROGMEM {
    wt12_init_0,
    wt12_init_1
};

char buffer[60]; // used for printing these init messages


// Set buttons pin numbers
const int UP_BUTTON = 2;
const int DOWN_BUTTON = 4;

// The Mouse HID command prefix, as hex array
const byte packetPrefix[] = { 0x9F, 0x05, 0xA1, 0x01 }; 

// Check if it works
uint8_t bluetoothPacket[7] = {0x9F, 0x05, 0xA1, 0x01};
//
//bluetoothPacket[0] = 0x9F;
//bluetoothPacket[1] = 0x05;
//bluetoothPacket[2] = 0xA1;
//bluetoothPacket[3] = 0x01;


// Up/Down commands translated to hex array
byte MOUSE_UP[]   = { 0x10, 0x00, 0x00 };
byte MOUSE_DOWN[] = { 0xF0, 0x00, 0x00 };

/**
 * Communicates with BT chip in Serial
 * Sends the entire packet, each byte in `Serial.write`
 */
void sendMousePacket(byte * packetExtra) {  
  // TODO: Check why loop doesn't work here
  
  Serial.write((byte)packetPrefix[0]);
  Serial.write((byte)packetPrefix[1]);
  Serial.write((byte)packetPrefix[2]);
  Serial.write((byte)packetPrefix[3]);

  Serial.write((byte)packetExtra[0]);
  Serial.write((byte)packetExtra[1]);
  Serial.write((byte)packetExtra[2]);
  
  // Send the command prefix
//  for (byte i = 0; i < sizeof(packetPrefix); i++) {
//    Serial.write((byte)packetPrefix[i]);
//  }
  
//  // Send the actual wanted command packet
//  for (byte i = 0; i < sizeof(packetExtra); i++) {
//    Serial.write((byte)packetExtra[i]);
//  }
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

void initBluetooth()
{
    // Reset the WT12 module
    pinMode(WT12_RESET, OUTPUT);
    digitalWrite(WT12_RESET, HIGH);
    delay(10);
    digitalWrite(WT12_RESET, LOW);

    Serial.println("\n\n");
    Serial.println("AT");
    delay(1000);

    // do the WT12 init
    for (byte i = 0; i < NUM_INIT_MSG; i++)
    {
        strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));
        Serial.println(buffer);
        delay(100);
    }
    delay(1000);
    Serial.flush(); // it echoes back everything we say?
    Serial.println("RESET");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);

  initBluetooth();

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonActivity();

  delay(200);
}
