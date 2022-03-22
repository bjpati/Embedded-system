/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

#include "LCD.h"
#include "usart.h"
#include <string.h>
#include "Test.h"


static char HHMMSS[6];
uint8_t *ASCIIarray;
uint8_t Time[8];


/**
 * @brief settingTime, ask the user to set the RTC. The input is then placed in the HHMMSS buffer.
 * The function charConvertering converts the ASCII chars to the numerical values. Time is
 * then set with RTC_TimeTypeDef structure variables.
 * @param void
 * @return void
 */

void settingTime(void){
	RTC_TimeTypeDef setTime;

		//Message to print to user
		uint8_t setMessage[] = "Set time HH:MM:SS\n\r";

		//Print message via UART
		HAL_UART_Transmit(&huart5, (uint8_t *)setMessage, strlen(setMessage), 10000);

		//Get HH MM SS from user
		HAL_UART_Receive(&huart5, (uint8_t *)HHMMSS, 6, 10000);

		ASCIIarray = charConvertering(HHMMSS);

		setTime.Hours = ASCIIarray[0];
		setTime.Minutes = ASCIIarray[1];
		setTime.Seconds = ASCIIarray[2];

		HAL_RTC_SetTime(&hrtc, &setTime, RTC_FORMAT_BIN);
}


/**
 * @brief gettingTime, HAL_RTC_GetTime returns the current time value, stores it in a char buffer,
 * and prints it on the LCD.
 * @param void
 * @return void
 */
void gettingTime(void){

	RTC_TimeTypeDef getTime;
	RTC_DateTypeDef getDate;

		// Get the RTC current time and date
		HAL_RTC_GetTime(&hrtc, &getTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &getDate, RTC_FORMAT_BIN);

		// returns a formated string
		sprintf((char *)Time, "%02d:%02d:%02d", getTime.Hours, getTime.Minutes, getTime.Seconds);

		LCDSetPosition(LINE1);
		LCDWriteString(Time);
}

/**
 * @brief charConvertering, This function converts ASCII characters to their numeric values.
 * @param array[], This array contains the current input of time from the user.
 * @return newArray, An array containing converted ASCII characters to be printed.
 */
uint8_t * charConvertering(uint8_t array[]){
	static uint8_t newArray[3];
	for(int i = 0; i < 3; i++){
		newArray[i] = (((array[i*2] - 0x30) * 10) + (array[i*2+1] - 0x30));
	}
	return newArray;
}

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

	RTC_TimeTypeDef setTime = {0};
	RTC_DateTypeDef setDate = {0};

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_POS1;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initialize RTC and set the Time
  */
  setTime.Hours = 0;
  setTime.Minutes = 0;
  setTime.Seconds = 0;
  if (HAL_RTC_SetTime(&hrtc, &setTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable Calibration
  */
  if (HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_512HZ) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**RTC GPIO Configuration
    PB2     ------> RTC_OUT_CALIB
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_RTC_50Hz;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /**RTC GPIO Configuration
    PB2     ------> RTC_OUT_CALIB
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);

  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
