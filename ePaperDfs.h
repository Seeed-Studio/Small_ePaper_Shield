/*
  ePaperDfs.h
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

#ifndef __EPAPERDFS_H__
#define __EPAPERDFS_H__

// pin define
#define Pin_TEMPERATURE   A0
#define Pin_PANEL_ON      2
#define Pin_BORDER        3
#define Pin_DISCHARGE     8
#define Pin_PWM           5
#define Pin_RESET         6
#define Pin_BUSY          7
#define Pin_EPD_CS        10

#define Pin_SD_CS         4
#define Pin_Font_CS       9

#define Pin_OE123         A1
#define Pin_STV_IN        A3

// spi cs

#define EPD_SELECT()        digitalWrite(Pin_EPD_CS, LOW)
#define EPD_UNSELECT()      digitalWrite(Pin_EPD_CS, HIGH)
#define SD_SELECT()         digitalWrite(Pin_SD_CS, LOW) 
#define SD_UNSELECT()       digitalWrite(Pin_SD_CS, HIGH)
#define FONT_SELECT()       digitalWrite(Pin_Font_CS, LOW) 
#define FONT_UNSELECT()     digitalWrite(Pin_Font_CS, HIGH)


#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/