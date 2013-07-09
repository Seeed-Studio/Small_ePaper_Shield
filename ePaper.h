/*
  ePaper.h
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Modified by Loovee
  www.seeedstudio.com
  2013-7-2

  Copyright 2013 Pervasive Displays, Inc.
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at:

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing,
  software distributed under the License is distributed on an
  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
  express or implied.  See the License for the specific language
  governing permissions and limitations under the License.

  This program is to illustrate the display operation as described in
  the datasheets.  The code is in a simple linear fashion and all the
  delays are set to maximum, but the SPI clock is set lower than its
  limit.  Therfore the display sequence will be much slower than
  normal and all of the individual display stages be clearly visible.
*/

#ifndef __EPAPER_H__
#define __EPAPER_H__

#include <SD.h>
#include <SPI.h>
#include <EPD.h>

#include "sd_epaper.h"
#include "ePaperDfs.h"

#define EP_DEBUG            1

#if EP_DEBUG
#define print_ep(X)         Serial.print(X)
#define println_ep(X)       Serial.println(X)
#else
#define print_ep(X)
#define println_ep(X)
#endif

class ePaper
{

private:

    int getTemperature();                   // get temperature
    unsigned char tMatrix[32];
    
    int SIZE_LEN;
    int SIZE_WIDTH;
    
    EPD_DIR direction;
    
public:

    EPD_size size;

    void begin(EPD_size sz);
    
    void setDirection(EPD_DIR dir);
    
    void start();
    
    void end();
    
    void init_io();
    
    unsigned char display();                // refresh 
    
    inline void drawPixel(unsigned char x, unsigned char y, unsigned char color)
    {
		eSD.putPixel(x, y, color);
	}
    
    unsigned char drawChar(char c, unsigned char x, unsigned char y);
    unsigned char drawString(char *string, int poX, int poY);
    unsigned char drawUnicode(unsigned int uniCode, unsigned char x, unsigned char y);
    unsigned char drawNumber(long long_num,int poX, int poY);
    unsigned char drawFloat(float floatNumber,unsigned char decimal,unsigned int poX, unsigned int poY);
    
    unsigned char drawLine(int x0, int y0, int x1, int y1);
    unsigned char drawCircle(int poX, int poY, int r);
    unsigned char drawHorizontalLine( int poX, int poY, int len);
    unsigned char drawVerticalLine( int poX, int poY, int len);
    unsigned char drawRectangle(int poX, int poY, int len, int width);
    unsigned char fillRectangle(int poX, int poY, int len, int width);
    unsigned char fillCircle(int poX, int poY, int r);
    unsigned char drawTraingle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3);
};

extern ePaper EPAPER;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/