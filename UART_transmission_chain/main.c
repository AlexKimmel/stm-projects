#include <stdint.h>
#include <stdlib.h>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"

void SystemClock_Config(void);
void GPIO_Init(void);
void USART2_UART_Init(void);
void USART3_UART_Init(void);

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

#define BUF_SZ 100
uint8_t rcv2[BUF_SZ];
uint8_t rcv3[BUF_SZ];
const unsigned char NEWLINE = '\n';

// Linked List
struct LinkedList {
    uint8_t value;
    struct LinkedList* next;
    struct LinkedList* prev;
};

struct LinkedList* data_start = NULL;
struct LinkedList* data_end = NULL;
unsigned char transfer_finished = 'n';

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
        newItem->value = newItem->value | tmp;
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

void reTransferData() {
    struct LinkedList *currentItem = data_start;
    while (currentItem != NULL) {
        HAL_UART_Transmit(&huart2, &currentItem->value, 1, HAL_MAX_DELAY);
        currentItem = currentItem->next;
    }
    transfer_finished = 'n';
}
// Linked List End

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        HAL_UART_Transmit(&huart3, rcv2, 1, HAL_MAX_DELAY);

        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14); // Toggle LED to signal character reception
        HAL_UART_Receive_IT(&huart2, rcv2, 1); // Restart the interrupt
    } else if (huart->Instance == USART3) {
        // HAL_UART_Transmit(&huart2, rcv3, 1, HAL_MAX_DELAY);

        if (rcv3[0] == '\r') {
            transfer_finished = 'y';
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); // Toggle LED to signal character reception
            HAL_UART_Receive_IT(&huart3, rcv3, 1); // Restart the interrupt
            return;
        } else {
            insertAtEnd(rcv3[0]);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); // Toggle LED to signal character reception
            HAL_UART_Receive_IT(&huart3, rcv3, 1); // Restart the interrupt
        }
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    USART2_UART_Init();
    USART3_UART_Init();

    BSP_LED_Init(LED2);

    data_start = NULL;
    data_end = NULL;

    HAL_UART_Receive_IT(&huart2, rcv2, 1); // Start UART receive interrupt
    HAL_UART_Receive_IT(&huart3, rcv3, 1); // Start UART receive interrupt

    while (1) {
        if (transfer_finished == 'y') {
            reTransferData();
        } else {
            HAL_Delay(100); //Keep the CPU active
            BSP_LED_Toggle(LED2);
        }
    }
}

// Interrupt Handlers
void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}

void USART3_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart3);
}

void SysTick_Handler(void) {
    HAL_IncTick();
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void) {
    BSP_LED_Init(LED2);
    BSP_LED_On(LED2);
    while (1) {
        // Stay here
    }
}

void USART2_UART_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void USART3_UART_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart3) != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
}

void SystemClock_Config(void) {
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;

    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                   RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    __HAL_RCC_CSI_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_EnableCompensationCell();
}