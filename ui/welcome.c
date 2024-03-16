/* Copyright 2023 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Modified work Copyright 2023-2024 egzumer
 * https://github.com/egzumer
 *
 * Modified work Copyright 2024 nikant
 * https://github.com/nikant
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#include <string.h>

#include "driver/eeprom.h"
#include "driver/st7565.h"
#include "external/printf/printf.h"
#include "helper/battery.h"
#include "settings.h"
#include "misc.h"
#include "ui/helper.h"
#include "ui/welcome.h"
#include "ui/status.h"
#include "version.h"

void UI_DisplayReleaseKeys(void)
{
	memset(gStatusLine,  0, sizeof(gStatusLine));
	UI_DisplayClear();

	UI_PrintString("RELEASE", 0, 127, 1, 10);
	UI_PrintString("ALL KEYS", 0, 127, 3, 10);

	ST7565_BlitStatusLine();  // blank status line
	ST7565_BlitFullScreen();
}

void UI_DisplayWelcome(void)
{
	char WelcomeString0[16];
	char WelcomeString1[16];

	memset(gStatusLine,  0, sizeof(gStatusLine));
	UI_DisplayClear();

	if (gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_NONE)
	{
		ST7565_BlitFullScreen();
	}
	else
	if (gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_FULL_SCREEN)
	{
		char battstatus[10];
		sprintf(battstatus, "%u.%02uV %u%%",gBatteryVoltageAverage / 100,gBatteryVoltageAverage % 100,BATTERY_VoltsToPercent(gBatteryVoltageAverage));
		#ifdef ENABLE_SY1EBE						
			UI_PrintString("NIKANT", 0, LCD_WIDTH, 0, 8);
			UI_PrintString("SY1EBE", 0, LCD_WIDTH, 2, 8);		
			UI_PrintString(battstatus, 0, LCD_WIDTH, 4, 10);			
		#else
			UI_PrintString("Quansheng", 0, LCD_WIDTH, 0, 8);	
			UI_PrintString(battstatus, 0, LCD_WIDTH, 3, 10);
		#endif	
		UI_PrintStringSmall(Version, 0, 128, 6);
		ST7565_BlitStatusLine();  // blank status line
		ST7565_BlitFullScreen();	

		//ST7565_FillScreen(0xFF);
	} else {
		memset(WelcomeString0, 0, sizeof(WelcomeString0));
		memset(WelcomeString1, 0, sizeof(WelcomeString1));

		if (gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_VOLTAGE)
		{
			strcpy(WelcomeString0, "VOLTAGE");
			sprintf(WelcomeString1, "%u.%02uV %u%%",
				gBatteryVoltageAverage / 100,
				gBatteryVoltageAverage % 100,
				BATTERY_VoltsToPercent(gBatteryVoltageAverage));
		}
		else
		{
			EEPROM_ReadBuffer(0x0EB0, WelcomeString0, 16);
			EEPROM_ReadBuffer(0x0EC0, WelcomeString1, 16);
		}

		UI_PrintString(WelcomeString0, 0, 127, 0, 10);
		UI_PrintString(WelcomeString1, 0, 127, 2, 10);
		UI_PrintStringSmallNormal(Version, 0, 128, 6);

		ST7565_BlitStatusLine();  // blank status line
		ST7565_BlitFullScreen();
	}
}
