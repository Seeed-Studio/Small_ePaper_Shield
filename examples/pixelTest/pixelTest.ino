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
// *
// * clear screen
// * update display (temperature)
// * delay 60 seconds (flash LED)
// * back to update display

#include <inttypes.h>
#include <ctype.h>

#include <SPI.h>
#include <EPD.h>
#include <SD.h>
#include <Streaming.h>

#include <sd_epaper.h>
#include "ePaper.h"

// Change this for different display size
// supported sizes: 1_44 2_0
#define EPD_SIZE EPD_2_0

void setup() 
{
	Serial.begin(38400);
    EPAPER.begin(EPD_SIZE);
    eSD.begin(4);
    cout << "inti over" << endl;
}

// main loop
void loop() 
{

    int temperature  = 30;

    eSD.openFile();

    for(int i=10; i<190; i++)
    for(int j=10; j<15; j++)
    EPAPER.drawPixel(i, j, 1);
    
    EPAPER.start();

    EPAPER.display();               // refresh image
    eSD.closeFile();
    cout << "put pixel over" << endl;
    while(1);
}
