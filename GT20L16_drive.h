/*
  GT20L16_drive.h
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

#ifndef __GT20L16_DRIVE_H__
#define __GT20L16_DRIVE_H__

#define PIN_GT20L16_CS      9

class GT20L16_drive{

private:

    int pinCS;

private:

    void GT_Select();
    void GT_UnSelect();
    unsigned long getAddrFromUnicode(unsigned int uniCode);
    unsigned long GTRead(unsigned long Address);
public:

    void begin(int pinSelect);
    int getMatrixUnicode(unsigned int uniCode, unsigned char *matrix);


};

extern GT20L16_drive GT20L16;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/