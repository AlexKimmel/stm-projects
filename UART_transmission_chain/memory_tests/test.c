#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct LinkedList* data_start = NULL;
struct LinkedList* data_end = NULL;

struct LinkedList {
    uint8_t value;
    struct LinkedList* next;
    struct LinkedList* prev;
};

void printBits(uint8_t num) {
    for (int bit = 7; bit >= 0; bit--) {
        putchar((num & (1 << bit)) ? '1' : '0');
        if (bit % 8 == 0) { // Optional: Add a space every 8 bits for readability
            putchar(' ');
        }
    }
    putchar('\n');
}

void insertAtEnd(uint8_t val) {
    static uint8_t bitCounter = 0;
    static struct LinkedList *newItem = NULL;

    if (bitCounter == 0) {
        newItem = (struct LinkedList*) malloc(sizeof(struct LinkedList));
        newItem->value = 0b00000000;
        newItem->prev = data_end;
        newItem->next = NULL;
    }

    if (val == '1') {
        uint8_t tmp = 1 << (7 - bitCounter);
        newItem->value |= tmp;
    }

    if (bitCounter == 7) {
        if (data_end != NULL) {
            data_end->next = newItem;
        } else {
            data_start = newItem;
        }
        data_end = newItem; 
        bitCounter = 0;
        return;
    }

    ++bitCounter;
}

int intToAscii(int number) {
   return '0' + number;
}

int main() {
    insertAtEnd('0');
    insertAtEnd('1');
    insertAtEnd('1');
    insertAtEnd('0');
    insertAtEnd('0');
    insertAtEnd('0');
    insertAtEnd('0');
    insertAtEnd('1');

    printf("%c", data_start->value);
    char crs[4] = {0,0,0,0};
    sprintf(crs, "%d", 3255);
    for (int i = 0; i < 4; i++) {
        printf("digit %d \n", crs[i]);
    }
}