/*
  GT20L16_drive.h
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
    
    void begin(){begin(PIN_GT20L16_CS);}

    int getMatrixUnicode(unsigned int uniCode, unsigned char *matrix);


};

extern GT20L16_drive GT20L16;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/