// -*- mode: c++ -*-
// Copyright 2013 Pervasive Displays, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied.  See the License for the specific language
// governing permissions and limitations under the License.


// This program is to illustrate the display operation as described in
// the datasheets.  The code is in a simple linear fashion and all the
// delays are set to maximum, but the SPI clock is set lower than its
// limit.  Therfore the display sequence will be much slower than
// normal and all of the individual display stages be clearly visible.

// Operation from reset:
// * display version
// * display compiled-in display setting
// * display FLASH detected or not
// * display temperature (displayed before every image is changed)
// * clear screen
// * update display (temperature)
// * delay 60 seconds (flash LED)
// * back to update display


#include <inttypes.h>
#include <ctype.h>

#include <SPI.h>
#include <FLASH.h>
#include <EPD.h>
#include <Adafruit_GFX.h>
#include <EPD_GFX.h>
#include <SPI.h>
#include <SD.h>
#include <Streaming.h>

#include <sd_epaper.h>


// Change this for different display size
// supported sizes: 1_44 2_0
#define EPD_SIZE EPD_2_0

// define the E-Ink display
EPD_Class EPD(EPD_SIZE, Pin_PANEL_ON, Pin_BORDER, Pin_DISCHARGE, Pin_PWM, Pin_RESET, Pin_BUSY, Pin_EPD_CS, SPI);

EPD_GFX G_EPD;

void init_epd()
{

    int temp = EPAPER.getTemperature();
    cout << "temperature: " << temp << endl;
    delay(100);

    EPAPER.init_io();
    EPD.begin();                 // power up the EPD panel
    EPD.setFactor(temp);         // adjust for current temperature
}



void setup() 
{
	Serial.begin(38400);
    EPAPER.init_io();
    eSD.begin(4);
    
    cout << "inti over" << endl;
}


// main loop
void loop() 
{

    int temperature  = 30;
    
    G_EPD.drawPixel(0, 0, 1);
    
    eSD.openFile();


#if 1
	char temp[sizeof("-999 C")];
	snprintf(temp, sizeof(temp), "%4d C", temperature);

    cout << temp << endl;
    
	int x = 20;
	int y = 30;
	for (int i = 0; i < sizeof(temp) - 1; ++i, x += 14) {
		G_EPD.drawChar(x, y, temp[i], BLACK, WHITE, 2);
	}

// 100 difference just to simplify things
// so 1 pixel = 1 degree
#define T_MIN (-10)
#define T_MAX 80

	// clip
	if (temperature < T_MIN) {
		temperature= T_MIN;
	} else if (temperature > T_MAX) {
		temperature = T_MAX;
	}

	// temperature bar
	int bar_w = temperature - T_MIN;  // zero based
	int bar_h = 4;
	int bar_x0 = 24;
	int bar_y0 = 60;

	G_EPD.fillRect(bar_x0, bar_y0, T_MAX - T_MIN, bar_h, WHITE);
	G_EPD.fillRect(bar_x0, bar_y0, bar_w, bar_h, BLACK);

	// scale
	for (int t0 = T_MIN; t0 < T_MAX; t0 += 5) {
		int t = t0 - T_MIN;
		int tick = 8;
		if (0 == t0) {
			tick = 12;
			G_EPD.drawCircle(bar_x0 + t, bar_y0 + 16, 3, BLACK);
		} else if (0 == t0 % 10) {
			tick = 10;
		}
		G_EPD.drawLine(bar_x0 + t, bar_y0 + tick, bar_x0 + t, bar_y0 + 6, BLACK);
		G_EPD.drawLine(bar_x0 + t, bar_y0 + 6, bar_x0 + t + 5, bar_y0 + 6, BLACK);
		G_EPD.drawLine(bar_x0 + t + 5, bar_y0 + 6, bar_x0 + t + 5, bar_y0 + 8, BLACK);
	}

	// update the display
	
#endif
    
    init_epd();

    G_EPD.display_lvc();
    eSD.closeFile();
    cout << "put pixel over" << endl;
    while(1);
}
