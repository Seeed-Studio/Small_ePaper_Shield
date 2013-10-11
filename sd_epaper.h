/*
  sd_epaper.h
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Modified by Loovee
  www.seeedstudio.com
  2013-7-2

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __SD_EPAPER_H__
#define __SD_EPAPER_H__

#include "ePaperDfs.h"
#include "EPD.h"

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)

#define OLDIMAGENAME "oldimage"
#define NEWIMAGENAME "newimage"

#endif

#define BLACK   1
#define WHITE   0

#define SD_DEBUG            0

#if SD_DEBUG
#define print_sd(X)         Serial.print(X)
#define println_sd(X)       Serial.println(X)
#else
#define print_sd(X)
#define println_sd(X)
#endif

class sd_epaper
{
private:

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    File old_image;
    File new_image;
#endif 
    int SIZE_LEN;
    int SIZE_WIDTH;
    int LINE_BYTE;
    
    int DISP_LEN;
    int DISP_WIDTH;
    
    int openFileTime;
    int closeFileTime;
    
    EPD_DIR direction;

public:
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    unsigned char sram_image[5808];    // 264*176/8
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    unsigned char lineDta[33];
#endif
    
public:

    void begin(EPD_size sz);

    void setDirection(EPD_DIR dir);
    
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    unsigned char openFile();
    unsigned char closeFile();

    unsigned char putLine(int line, unsigned char *dta);
    unsigned char getLine(int line, unsigned char *dta);
    void getPixel(int x, int y);
#endif 
    void putPixel(int x, int y, unsigned char pixel);

    
    unsigned char clear();

};

extern sd_epaper eSD;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
