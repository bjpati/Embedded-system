/*
 * RealTImeClock.c
 *
 *  Created on: Jan 2, 2022
 *      Author: basharpati
 */

#include "Test.h"
#include "LCD.h"

/**
 * @brief RealTimeClock, calls the functions LCDInitialize, LCDClear, WriteString, backlightSetRed, settingTime,
 * backlightSetBrightnes and gettingTime, which launches and reads the welcome message and to set the time.
 * @param void
 * @return void
 */

void RealTimeClock(void)
{
	LCDInitialize();
	LCDClear();
	HAL_GPIO_WritePin(GPIOC, Disp_White_Pin, GPIO_PIN_SET);
	WriteString();
	LCDClear();
	HAL_GPIO_WritePin(GPIOC, Disp_White_Pin, GPIO_PIN_RESET);
	backlightSetRed();
	settingTime();
	while(1)
	{
		backlightSetBrightnes();
		gettingTime();
	}
}
