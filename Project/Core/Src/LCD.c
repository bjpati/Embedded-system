/*
 * Lcd.c
 *
 *  Created on: Dec 29, 2021
 *      Author: basharpati
 */


#include "LCD.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"
#include "adc.h"


uint8_t start = 0;
uint8_t first = 0;
uint8_t second = 0;
uint8_t buffer[3];

/**
 * @brief LCDSendInstruction, setup of display bitstream for data, with RS and RW variable set to 0.
 * SID Input bitstream to be set: 1111 10(RW)0(RS)0 D0D1D2D3 0000 D4D5D6D7 0000
 * @param inst, instruction to be sent to the display
 */
void LCDSendInstruction(uint8_t inst){
	uint8_t startByte = 0x1F;
	uint8_t firstByte = 0x00;
	uint8_t secondByte = 0x00;

	firstByte = firstByte | (inst & 0x0F);

	secondByte = (inst >> 4) & 0x0F;

	buffer[0] = startByte;
	buffer[1] = firstByte;
	buffer[2] = secondByte;

	HAL_GPIO_WritePin(GPIOB, Disp_PB12_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_SPI_Transmit(&hspi2, buffer, 3, 1000);
	HAL_GPIO_WritePin(GPIOB, Disp_PB12_Pin, GPIO_PIN_SET);
}
/**
 * @brief LCDSendData, setup of display bitstream for data, with RS variable set to 1.
 * SID Input bitstream to be set: 1111 10(RW)1(RS)0 D0D1D2D3 0000 D4D5D6D7 0000
 * @param data, data byte to be sent to the display
 * @return void
 */
void LCDSendData(uint8_t data){
	uint8_t startByte = 0x5F;
	uint8_t firstByte = 0x00;
	uint8_t secondByte = 0x00;

	firstByte = firstByte | (data & 0x0F);

	secondByte = (data >> 4) & 0x0F;

	buffer[0] = startByte;
	buffer[1] = firstByte;
	buffer[2] = secondByte;

	HAL_GPIO_WritePin(GPIOB, Disp_PB12_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, buffer, 3, 1000);
	HAL_GPIO_WritePin(GPIOB, Disp_PB12_Pin, GPIO_PIN_SET);
}

/**
 * @brief LCDInitialize, display initialization according to the example in EA DOGS104-A data sheet.
 * @param void
 * @return void
 */
void LCDInitialize(void){
	LCDSendInstruction(FUNCTION_SET_1);
	LCDSendInstruction(EXTENDED_FUNCTION_SET);
	LCDSendInstruction(ENTRY_MODE_SET);
	LCDSendInstruction(BIAS_SETTING);
	LCDSendInstruction(FUNCTION_SET_2);
	LCDSendInstruction(INTERNAL_OSC);
	LCDSendInstruction(FOLLOWER_CONTROL);
	LCDSendInstruction(POWER_CONTROL);
	LCDSendInstruction(CONTRAST_SET);
	LCDSendInstruction(FUNCTION_SET_3);
	LCDSendInstruction(DISPLAY_ON);
}

/**
 * @brief LCDSetPosition, sets the display position where the data should be printed.
 * @param position, what line on the display that should be utilized.
 * return void
 */
void LCDSetPosition(uint8_t position){
	LCDSendInstruction(LCD_HOME_L1 + position);
}

/**
 * @brief LCDClear, clear the display.
 * @param void
 * @return void
 */
void LCDClear(void){
	LCDSendInstruction(0x01);
}

/**
 * @brief LCDSetROM, sets the display ROM.
 * @param void
 * @return void
 */
void LCDSetROM(void){
	LCDSendInstruction(FUNCTION_SET_1);
	LCDSendInstruction(0x72);
	LCDSendData(ROMB);
	LCDSendInstruction(0x38);
}

/**
 * @brief LCDWriteString, send a buffer of characters to be printed by function LCDSendData.
 * @param string, a pointer to the string buffer.
 * @return void
 */
void LCDWriteString(uint8_t* string){
	do
	{
		LCDSendData(*string++);
	}
	while(*string);
}

/**
 * @brief WriteString, a function that send a string to LCDWriteString function
 * and the string should be printed on the display.
 * @param void
 * @return void
 */
void WriteString(void){
	LCDSetPosition(LINE1);
	LCDWriteString("Welcome To");
	HAL_Delay(2000);
	LCDSetPosition(LINE2);
	LCDWriteString("EmbeddedSystems");
	HAL_Delay(2000);
	LCDSetPosition(LINE3);
	LCDWriteString("Project");
	HAL_Delay(2000);
	LCDSetPosition(LINE4);
	LCDWriteString("Bashar,J,P. ");
	HAL_Delay(3000);
}
/**
 * @brief backlightSetBrightnes, gets a value from function getADCValue and sets the backlight brightness with that value.
 * uses PWM/TIM3 to be able to set the display brightness.
 * @param void
 * @return void
 */
void backlightSetBrightnes(void){
	uint32_t ADCval = getADCValue();
	htim3.Instance->CCR2 = ADCval;
}

/**
 * @brief backlightSetWhite, sets the color of the LCD backlight to white
 * @param void
 */
void backlightSetWhite(void){
	HAL_GPIO_WritePin(GPIOC, Disp_White_Pin, GPIO_PIN_SET);
}

/**
 * @brief backlightSetGreen, sets the color of the LCD backlight to green
 * @param void
 */
void backlightSetGreen(void){
	HAL_GPIO_WritePin(GPIOC, Disp_Green_Pin, GPIO_PIN_SET);
}

/**
 * @brief backlightSetRed, sets the color of the lcd backlight to red
 * @param void
 */
void backlightSetRed(void){
	HAL_GPIO_WritePin(GPIOC, Disp_Red_Pin, GPIO_PIN_SET);
}


