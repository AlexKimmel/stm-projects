#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "linkedList.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"
#pragma once 

// Function to get the checksum of a char array 
unsigned char checksum (unsigned char *ptr, size_t sz) {
    unsigned char chk = 0;
    while (sz-- != 0)
        chk -= *ptr++;
    return chk;
}

// Function to encode data using triplication
void QuadruplicationEncode(struct LinkedList *data) {
    struct LinkedList *tmpPointer = data;
    uint8_t value = data->value;

    uint8_t encodedData[4] = {0, 0, 0, 0};

    // Quadruplicate each bit
    for (int i = 0; i < 8; i++) {
        uint8_t bit = (value >> (7 - i)) & 0x01;
        encodedData[i / 2] |= (bit << (7 - 4 * (i % 2))) | (bit << (6 - 4 * (i % 2))) | (bit << (5 - 4 * (i % 2))) | (bit << (4 - 4 * (i % 2)));
    }

    // Assign the encoded bytes to the linked list
    data->value = encodedData[0];
    for (int i = 1; i < 4; i++) {
        insertInbetween(&tmpPointer, encodedData[i]);
        tmpPointer = tmpPointer->next;
    }
}

// uint8_t QuadruplicationDecode(struct LinkedList *data) {
//     uint8_t decodedValue = 0;
//     struct LinkedList *tmpPointer = data;

//     for (int i = 0; i < 8; i++) {
//         uint8_t bit1 = (tmpPointer->value >> (7 - 4 * (i % 2))) & 0x01;
//         uint8_t bit2 = (tmpPointer->value >> (6 - 4 * (i % 2))) & 0x01;
//         uint8_t bit3 = (tmpPointer->value >> (5 - 4 * (i % 2))) & 0x01;
//         uint8_t bit4 = (tmpPointer->value >> (4 - 4 * (i % 2))) & 0x01;

//         uint8_t majorityBit = (bit1 + bit2 + bit3 + bit4) >= 2 ? 1 : 0;

//         decodedValue |= (majorityBit << (7 - i));

//         if (i % 2 == 1) {
//             tmpPointer = tmpPointer->next;
//         }
//     }

//     return decodedValue;
// }

// Function to decode data using triplication
uint8_t QuadruplicationDecode(uint8_t *encodedData) {
    uint8_t decodedData = 0;

    // Use majority voting for each quadruplicated bit
    for (int i = 0; i < 8; i++) {
        uint8_t bit0 = (encodedData[i / 2] >> (7 - 4 * (i % 2))) & 0x01;
        uint8_t bit1 = (encodedData[i / 2] >> (6 - 4 * (i % 2))) & 0x01;
        uint8_t bit2 = (encodedData[i / 2] >> (5 - 4 * (i % 2))) & 0x01;
        uint8_t bit3 = (encodedData[i / 2] >> (4 - 4 * (i % 2))) & 0x01;

        uint8_t majorityBit = (bit0 + bit1 + bit2 + bit3) >= 2 ? 1 : 0;

        decodedData |= (majorityBit << (7 - i));
    }

    return decodedData;
}

