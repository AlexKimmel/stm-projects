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
    uint32_t encodedData = 0;
    uint8_t value = data->value;
    data->value = 0;

    // Triplicate each bit
    for (int i = 0; i < 8; i++) {
        uint8_t bit = (value >> i) & 0x01;
        encodedData |= (bit << (4 * i)) | (bit << (4 * i + 1)) | (bit << (4 * i + 2)) | (bit << (4 * i + 3));
    }
    for (int i = 0; i < 4; i++) {
        uint8_t tmp = (value >> (i * 8)) & 0xFF;
        if (i == 0) {data->value;}
        else{insertInbetween(tmpPointer, tmp);}
        tmpPointer = tmpPointer->next;
    }
}

// Function to decode data using triplication
char QuadruplicationDecode(uint32_t encodedData) {
    char decodedData = 0;

    // Use majority voting for each triplicated bit
    for (int i = 0; i < 8; i++) {
        uint8_t bit0 = (encodedData >> (4 * i)) & 0x01;
        uint8_t bit1 = (encodedData >> (4 * i + 1)) & 0x01;
        uint8_t bit2 = (encodedData >> (4 * i + 2)) & 0x01;
        uint8_t bit3 = (encodedData >> (4 * i + 3)) & 0x01;
        uint8_t majorityBit = 0;
        if ((bit0 + bit1 + bit2 + bit3) > 2){
            majorityBit = 1;} 
        else if ((bit0 + bit1 + bit2 + bit3) <= 2) {
            majorityBit = 0;}

        decodedData |= (majorityBit << i);
    }

    return decodedData;
}

void printBits(uint32_t num) {
    for (int bit = 31; bit >= 0; bit--) {
        putchar((num & (1 << bit)) ? '1' : '0');
        if (bit % 8 == 0) { // Optional: Add a space every 8 bits for readability
            putchar(' ');
        }
    }
    putchar('\n');
}

