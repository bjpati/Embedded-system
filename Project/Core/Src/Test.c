/*
 * Test.c
 *
 *  Created on: Jan 2, 2022
 *      Author: basharpati
 */

#include "Test.h"
#include "LCD.h"
#include "gpio.h"
#include "adc.h"
#include "rtc.h"

char ADCString[20];

void Test_program(void)
{
	LCDClearTest();
	LCDInitializeTest();
	LCDSetROMTest();
	LCDClear();
	backlightSetWhiteTest();
	WriteCharTest();
	HAL_Delay(1000);
	LCDClear();
	WriteStringTest();
	HAL_Delay(1000);
	LCDClear();
	LCDSetPositionTest();
	LCDClear();
	SetBacklightTest();
	getADCValueTest();
	LCDClear();
	settingTimeTest();
	gettingTimeTest();

}

/**
 * @brief LCDClearTest, a test function to run LCDClear.
 * Successful tests result in the display being cleared with information.
 * The message "LCDClearTest succeed" being printed through serial communication.
 * @param void
 * @return void
 */
void LCDClearTest(void){
	LCDClear();
	uint8_t* LCDClearTest = "LCDClearTest succeed\n\r";
	Transmission(LCDClearTest);
}


/**
 * @brief LCDInitializeTest, a test to initialize the display.
 * A successful test will print "LCDInitializeTest succeed" via serial communication.
 * @param void
 * @return void
 */
void LCDInitializeTest(void){
	LCDInitialize();
	uint8_t* LCDInitializeTest = "LCDInitializeTest succeed\n\r";
	Transmission(LCDInitializeTest);
}

/**
 * @brief LCDSetROMTest, test to set the LCD ROM with function call to LCDSetROM.
 * A successful test is marked by "LCDRomTest succeed" being printed via serial communication.
 * @param void
 * @return void
 */
void LCDSetROMTest(void){
	LCDSetROM();
	uint8_t* LCDRomTest = "LCDRomTest succeed\n\r";
	Transmission(LCDRomTest);
}

/**
 * @brief backlightSetWhiteTest, a test function to turn on the white backlight on the display. Calls
 * function backlightSetWhite. If the display has a white backlight, the test is successful.
 * @param void
 * @return void
 */
void backlightSetWhiteTest(void){
	backlightSetWhite();
}

/**
 * @brief WriteCharTest, Using serial communications, the test is considered successful if characters
 * are visible on the display and "WriteCharTest succeed" is printed.
 * @param void
 *  @return void
*/
void WriteCharTest(void){
	LCDSendInstruction(0x80);
	LCDSendData(0x77);
	LCDSendData(0x61);
	LCDSendData(0x64);
	LCDSendData(0x64);
	LCDSendData(0x61);
	LCDSendData(0x70);
	LCDSendData(0x3F);
	uint8_t* WriteCharTest = "WriteCharTest succeed\n\r";
	Transmission(WriteCharTest);
}

/**
 * @brief WriteStringTest, a test function that send a string to LCDWriteString function
 * and the string should be printed on the display.
 * If the string appears on display and "WriteStringTest succeed" is displayed via serial communication,
 * the test is considered successful.
 * @param void
 * @return void
 */
void WriteStringTest(void){
	LCDSetPosition(LINE1);
	LCDWriteString("HELLO");
	HAL_Delay(2000);
	LCDSetPosition(LINE2);
	LCDWriteString("I CAN");
	HAL_Delay(2000);
	LCDSetPosition(LINE3);
	LCDWriteString("WRITE");
	HAL_Delay(2000);
	LCDSetPosition(LINE4);
	LCDWriteString("ON DESPLAY");
	HAL_Delay(3000);

	uint8_t* WriteStringTest = "WriteStringTest succeed\n\r";
	Transmission(WriteStringTest);
}

/**
 * @brief LCDSetPositionTest, prints string "x.Line", where x represents the line number that will be printed.
 * A successful test is seen when 4 lines of text appear on the display and "LCDSetPositionTest succeed"
 * is printed via serial communication.
 * @param void
 * @return void
 */
void LCDSetPositionTest(void){
	LCDSetPosition(LINE1);
	LCDWriteString("First.Line");
	HAL_Delay(1000);
	LCDSetPosition(LINE2);
	LCDWriteString("Second.Line");
	HAL_Delay(1000);
	LCDSetPosition(LINE3);
	LCDWriteString("Third.Line");
	HAL_Delay(1000);
	LCDSetPosition(LINE4);
	LCDWriteString("Fourth.Line");
	HAL_Delay(3000);
	uint8_t* LCDSetPositionTest = "LCDSetPositionTest succeed\n\r";
	Transmission(LCDSetPositionTest);
}


/**
 * @brief SetBacklightTest a test function that turns white and green display color off, and turns on the red
 * color to be able to set the backlight strength via PWM. Backlighting function to test the PWM on the microcontroller,
 * SetBrightnes() is run in a while loop.Backlight SetBrightnes allows you to change the brightness of the backlight using a timing PWM.
 * @param void
 * @return void
 */
void SetBacklightTest(void){
	int i = 0;
	HAL_GPIO_WritePin(GPIOC, Disp_White_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Disp_Green_Pin, GPIO_PIN_RESET);

	LCDWriteString("PWM");
	LCDSetPosition(LINE2);
	LCDWriteString("is succeed");

	while(i < 1000000){
		backlightSetBrightnes();
		i++;
	}
}


/**
 * @brief getADCValueTest, calls the function getADCValue, which launches adc and reads the analog signal
 * before returning the value as an integer. The value is then displayed on the screen.
 * If the value and "getADCValueTest successfully" are printed over serial communication, the test is considered successful.
 * @param void
 * @return void
 */
void getADCValueTest(void){
	LCDInitialize();
	LCDClear();
	LCDSetPosition(LINE1);
	uint32_t value = getADCValue();
	sprintf((char *)ADCString, "%d" , value);
	LCDWriteString("ADC_Value");
	LCDSetPosition(LINE2);
	LCDWriteString(ADCString);
	HAL_Delay(1000);
	uint8_t* getADCValueTest = "getADCValueTest succeed\n\r";
	Transmission(getADCValueTest);
}

/**
 * @brief settingTimeTest calls function settingTime() that asks the user for a input to set the RTC.
 * @param void
 * @return void
 */
void settingTimeTest(void){
	settingTime();
}


/**
 * @brief gettingTimeTest calls function gettingTime() that displays the time on the display for 10 seconds.
 * @param void
 * @return void
 */
void gettingTimeTest(void){
	int i = 0;
	while(i < 2000){
		gettingTime();
		i++;
	}
}
