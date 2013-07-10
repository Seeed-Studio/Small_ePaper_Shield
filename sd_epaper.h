/*
  sd_epaper.h
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

#ifndef __SD_EPAPER_H__
#define __SD_EPAPER_H__

#include "ePaperDfs.h"
#include "EPD.h"

#define OLDIMAGENAME "oldimage"
#define NEWIMAGENAME "newimage"

#define BLACK   1
#define WHITE   0

#define SD_DEBUG            1

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

    unsigned char lineDta[33];          // line data
    
    File old_image;
    File new_image;
    
    int SIZE_LEN;
    int SIZE_WIDTH;
    int LINE_BYTE;
    
    int DISP_LEN;
    int DISP_WIDTH;
    
    int openFileTime;
    int closeFileTime;
    
    EPD_DIR direction;
    
public:

    
    unsigned char begin(unsigned char pinCs, EPD_size sz);
    void begin(EPD_size sz){begin(Pin_SD_CS, sz);}
    void setDirection(EPD_DIR dir);
    
    unsigned char openFile();
    unsigned char closeFile();
    
    unsigned char putLine(int line, unsigned char *dta);
    unsigned char getLine(int line, unsigned char *dta);
    
    void putPixel(int x, int y, unsigned char pixel);

    void getPixel(int x, int y);
    unsigned char clear();

};

extern sd_epaper eSD;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/