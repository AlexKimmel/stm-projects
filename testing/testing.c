#include <stdio.h>
#include <string.h>
#include <stdint.h>


// Function to encode data using triplication
uint32_t QuadruplicationEncode(char data) {
    uint32_t encodedData = 0;

    // Triplicate each bit
    for (int i = 0; i < 8; i++) {
        uint8_t bit = (data >> i) & 0x01;
        encodedData |= (bit << (4 * i)) | (bit << (4 * i + 1)) | (bit << (4 * i + 2)) | (bit << (4 * i + 3));
    }

    return encodedData;
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
            majorityBit = 1;} else if ((bit0 + bit1 + bit2 + bit3) < 2){
            majorityBit = 0;} else {}; //to-do

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



int main() {
     char data = 'a';



    uint32_t encoded = QuadruplicationEncode(data);
    char decode = QuadruplicationDecode(encoded);
    printBits(encoded);
    printBits(decode);

    return 0;
}