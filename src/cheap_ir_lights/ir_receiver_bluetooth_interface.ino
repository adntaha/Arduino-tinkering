#include <IRremote.h>

const int RECV_PIN = 9;
const int SNDV_PIN = 4;
const int FLED_PIN = 7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  IrReceiver.begin(RECV_PIN, true, FLED_PIN);
  IrSender.begin(SNDV_PIN);
  Serial.println("Ready!");
}

int commandMap[10][2] = {
  {0, 0x16},
  {1, 0xC},
  {2, 0x18},
  {3, 0x5E},
  {4, 0x8},
  {5, 0x1C},
  {6, 0x5A},
  {7, 0x42},
  {8, 0x52},
  {9, 0x4A}
};

/*
protocol: 8 (NEC)
address: 0xEF00

row
ON: 0x3
OFF: 0x2
BRIGHTNESS LOW: 0x1
BRIGHTNESS HIGH: 0x0

columns
REDDEST: 0x4
RED: 0x8
RED-ORANGE: 0xC
ORANGE: 0x10
YELLOW: 0x14

GREENEST: 0x5
BRIGHT GREEN: 0x9
LIGHT BLUE: 0xD
BLUE: 0x11
DARKEST BLUE: 0x15

DARK BLUE: 0x6
DARKER PURPLE: 0xA
BLACK: 0xE
DARK PURPLE: 0x12
PINK: 0x16

WHITE: 0x7
FLASH: 0xB
STROBE: 0xF
FADE: 0x13
SMOOTH: 0x17
*/

int findNumber(int command) {
    int i = 0;
    while (commandMap[i][1] != command && i < 10) {
      i++;
    }

    if (i == 10) return command;

    return commandMap[i][0];
}

void loop() {
    if (false && IrReceiver.decode()) {
        // Serial.println(IrReceiver.decodedIRData.protocol);
        if (IrReceiver.decodedIRData.protocol != NEC) {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print extended info
            Serial.println(findNumber(IrReceiver.decodedIRData.command));
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            IrReceiver.resume();
        } else {
            IrReceiver.resume(); // Early enable receiving of the next IR frame
            IrReceiver.printIRResultMinimal(&Serial);
            Serial.println();
            Serial.println(findNumber(IrReceiver.decodedIRData.command));
        }
    }

    // IrSender.sendNEC(0xEF00, 0x11, 2)
}
