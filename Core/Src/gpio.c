/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
leds_t leds[number] = {0};
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  LL_GPIO_ResetOutputPin(GPIOE, LED1_Pin|LED2_Pin|LED3_Pin);

  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
//-----------------------------------------
void leds_init (void) {
    leds[green].port = LED1_GPIO_Port;
    leds[green].pin = LED1_Pin;   
    leds[orange].port = LED2_GPIO_Port;
    leds[orange].pin = LED2_Pin;
    leds[red].port = LED3_GPIO_Port;
    leds[red].pin = LED3_Pin;
}

//-----------------------------------------
void control_LED (uint8_t number_leds, uint8_t comm)  {
  led_ctl(comm, leds[number_leds].port, leds[number_leds].pin);
}

//-----------------------------------------
void gpio_RS485_DirCtl_init (void) {
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

//-----------------------------------------
void RS485_DirCtl (uint8_t RS485mode)
{
  if (RS485mode == TX) {    LL_GPIO_SetOutputPin(RS485_DIR_Ctl_GPIO_Port, RS485_DIR_Ctl_Pin);  }
  else {    LL_GPIO_ResetOutputPin(RS485_DIR_Ctl_GPIO_Port, RS485_DIR_Ctl_Pin);  }
}
/* USER CODE END 2 */
