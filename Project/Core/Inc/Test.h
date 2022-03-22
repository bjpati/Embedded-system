
/*
 * Test.h
 *
 *  Created on: Jan 2, 2022
 *      Author: basharpati
 */

#ifndef INC_TEST_H_
#define INC_TEST_H_

#include "adc.h"
#include "LCD.h"
#include "usart.h"
#include "rtc.h"
#include <stdlib.h>
#include <stdio.h>

void Test_program(void);
void LCDClearTest(void);
void LCDInitializeTest(void);
void LCDSetROMTest(void);
void backlightSetWhiteTest(void);
void WriteCharTest(void);
void WriteStringTest(void);
void LCDSetPositionTest(void);
void SetBacklightTest(void);
void getADCValueTest(void);
void settingTimeTest(void);
void gettingTimeTest(void);


#endif /* INC_TEST_H_ */
