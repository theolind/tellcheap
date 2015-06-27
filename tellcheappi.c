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
 *
 * Requires wiringPi
 * gcc -o tellcheappi tellcheappi.c -lwiringPi
 *
 */

#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

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
#define TX_PIN 0

#define MAX_MSG_SIZE 38

void sendOne() {
    digitalWrite(TX_PIN, HIGH);
    usleep(PULSE_HIGH);
    digitalWrite(TX_PIN, LOW);
    usleep(PULSE_ONE);
}

void sendZero() {
    digitalWrite(TX_PIN, HIGH);
    usleep(PULSE_HIGH);
    digitalWrite(TX_PIN, LOW);
    usleep(PULSE_ZERO);
}

void sendSync() {
    digitalWrite(TX_PIN, HIGH);
    usleep(PULSE_HIGH);
    digitalWrite(TX_PIN, LOW);
    usleep(PULSE_SYNC);
}

void sendPause() {
    digitalWrite(TX_PIN, HIGH);
    usleep(PULSE_HIGH);
    digitalWrite(TX_PIN, LOW);
    usleep(PULSE_PAUSE);
}

int i, j;

int main(int argc, char **argv) {
    wiringPiSetup();
    pinMode(TX_PIN, OUTPUT);
    for(i = 1; i <= RESEND; i++) {
        sendSync();
        for(j = 0; j < strlen(argv[1]); j++) {
            switch(argv[1][j]) {
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
        sendPause();
        usleep(RESEND_DELAY);
    }

    return 0;
}
