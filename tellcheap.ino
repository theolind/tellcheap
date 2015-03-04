/*
* Heavily based on work by Erik Trædal (https://github.com/ErikLove)
* Original code: https://github.com/ErikLove/NexaGw
*
* Sends commands to Proove RF Switch Outlets
* And HomeEasy / NEXA (With modifications to pulse time)
* 
* Command structure: STTTTTTTTTTTTTTTTTTTTTTTTTTGOCCNNP
* S - Start/Sync
* T - Transmitter id / transmitter code
* G - Group (1 = no, 0 = yes)
* O - On/off (1 = off, 0 = on)
* C - Channel (01 for Proove)
* N - Device number (0: 00, 1: 01, 2: 10)
* P - Pause
*/

// Pulse timings, change these depending on your platform, this is for Proove
#define PULSE_SYNC 2500
#define PULSE_PAUSE 10000
#define PULSE_ONE 250
#define PULSE_ZERO 1250
#define PULSE_HIGH 250

// times to resend a command and the delay between resends
#define RESEND 4
#define RESEND_DELAY 10000

// Pinouts, use VCC_PIN and GND_PIN for the ability to connect transmitter directly to arduino board
// set a pin to -1 to disable it and use it for something else
#define TX_PIN 4
#define VCC_PIN 3
#define GND_PIN 2

#define MAX_MSG_SIZE 38

unsigned char msg[MAX_MSG_SIZE];
byte msgPtr = 0;
byte i, j;

void setup() {
  pinMode(TX_PIN, OUTPUT);
  if(VCC_PIN != -1) {
     pinMode(VCC_PIN, OUTPUT);
     digitalWrite(VCC_PIN, HIGH);
  }
  if(GND_PIN != -1) {
    pinMode(GND_PIN, OUTPUT);
    digitalWrite(GND_PIN, LOW);
  }
  
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    msg[msgPtr] = Serial.read();
    
    if(msg[0] != 'S' || msgPtr > MAX_MSG_SIZE) {
      msgPtr = 0;
    } else if(msg[msgPtr] == 'P') {
      for(j = 0; j < RESEND; j++) {
        for(i = 0; i <= msgPtr; i++) {
          switch(msg[i]) {
            case 'S':
              sendSync();
              break;
            case 'P':
              sendPause();
              break;
            case '0':
              sendZero();
              sendOne();
              break;
            case '1':
              sendOne();
              sendZero();
              break;
          }
        }
        delayMicroseconds(RESEND_DELAY);
      }
      msgPtr = 0;
    } else {
      msgPtr++;
    }
  }
}

void sendOne() {
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(PULSE_HIGH);
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(PULSE_ONE);
}

void sendZero() {
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(PULSE_HIGH);
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(PULSE_ZERO);
}

void sendSync() {
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(PULSE_HIGH);
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(PULSE_SYNC);
}

void sendPause() {
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(PULSE_HIGH);
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(PULSE_PAUSE);
}
