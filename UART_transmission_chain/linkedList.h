
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"
#pragma once

// Linked List
struct LinkedList {
    uint8_t value;
    struct LinkedList* next;
    struct LinkedList* prev;
};

void insertAtEnd(uint8_t val, struct LinkedList** data_start, struct LinkedList** data_end) {
    static uint8_t bitCounter = 0;
    static uint8_t newValue = 0b00000000;

    if (val == '1') {
        uint8_t tmp = 1 << (7 - bitCounter);
        newValue |= tmp;
    } else if (val != '0') {
        return;
    }

    if (bitCounter == 7) {
        struct LinkedList* newItem = malloc(sizeof(struct LinkedList));
        newItem->value = newValue;
        if (*data_start == NULL) {
            *data_start = newItem;
            newItem->prev = NULL;
        } else {
            newItem->prev = *data_end;
            (*data_end)->next = newItem;
        }
        *data_end = newItem;
        newItem->next = NULL;
        bitCounter = 0;
        newValue = 0b00000000;
        return;
    }

    bitCounter++;
}

void reTransferData(struct LinkedList* data_start, UART_HandleTypeDef *huart2) {
    struct LinkedList* currentItem = data_start;
    while (currentItem != NULL) {
        HAL_UART_Transmit(huart2, &currentItem->value, 1, HAL_MAX_DELAY);
        currentItem = currentItem->next;
    }
}