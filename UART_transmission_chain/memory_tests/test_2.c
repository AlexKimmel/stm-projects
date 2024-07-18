#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void printBits(uint8_t* num) {
    for (int i = 0; i <= 7; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            putchar((num[i] & (1 << bit)) ? '1' : '0');
        }
        putchar(' ');
    }
    putchar('\n');
}

uint8_t sendBufferA[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t sendBufferB[8] = {0, 0, 0, 0, 0, 0, 0, 0};

uint8_t receiveBufferA[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t receiveBufferB[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void clearSendBuffer(char buffer) {
    if (buffer == 'A') {
        memset(sendBufferA, 0, sizeof(sendBufferA));
    } else {
        memset(sendBufferB, 0, sizeof(sendBufferB));
    }
}

void clearReceiveBuffer(char buffer) {
    if (buffer == 'A') {
        memset(receiveBufferA, 0, sizeof(receiveBufferA));
    } else {
        memset(receiveBufferB, 0, sizeof(receiveBufferB));
    }
}

void singularizeAndSend(uint8_t data) {
    uint8_t counterOne = 0;
    uint8_t counterZero = 0;
    static uint8_t binaryCounter = 0;

    for (int bit = 7; bit >= 0; bit--) {
        if (data & (1 << bit)) {
            ++counterOne;
        } else {
            ++counterZero;
        }

        if ((counterOne + counterZero) >= 4) {
            uint8_t tmp = (counterOne > counterZero) ? '1' : '0';
            putchar(tmp);
            counterOne = 0;
            counterZero = 0;
            ++binaryCounter;
        }
    }

    if (binaryCounter >= 8) {
        binaryCounter = 0;
        putchar(' ');
    }
}

void quadrupleAndTransmit(uint8_t val) {
    static uint8_t counterA = 0;
    static uint8_t counterB = 0;
    static char activeBuffer = 'A';

    if (val == '1') {
        if (activeBuffer == 'A') {
            uint8_t tmp = (0b00001111 << (4 - ((counterA % 2) * 4)));
            sendBufferA[counterA / 2] |= tmp;
            
        } else {
            uint8_t tmp = (0b00001111 << (4 - ((counterB % 2) * 4)));
            sendBufferB[counterB / 2] |= tmp;
        }
    } else if (val != '0') {
        return;
    }

    if (activeBuffer == 'A') {
        ++counterA;
        if (counterA > 15) {
            counterA = 0;
            counterB = 0;
            activeBuffer = 'B';
            for (int i = 0; i < 8; ++i) {
                singularizeAndSend(sendBufferA[i]);
            }
            clearSendBuffer('B');
        }
    } else {
        ++counterB;
        if (counterB > 15) {
            counterA = 0;
            counterB = 0;
            activeBuffer = 'A';
            for (int i = 0; i < 8; ++i) {
                singularizeAndSend(sendBufferB[i]);
            }
            clearSendBuffer('A');
        }
    }
}

int main() {
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('1');
    quadrupleAndTransmit('1');
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('1');

    quadrupleAndTransmit('0');
    quadrupleAndTransmit('1');
    quadrupleAndTransmit('1');
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('0');
    quadrupleAndTransmit('1');

    return 0;
}
