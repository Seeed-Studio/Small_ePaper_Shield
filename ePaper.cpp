/*
  ePaper.cpp
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

#include <Arduino.h>
#include <SPI.h>

#include "ePaper.h"

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
void ePaper::begin()
{
    init_io();
}

/*********************************************************************************************************
** Function name:           init_io
** Descriptions:            init IO
*********************************************************************************************************/
void ePaper::init_io()
{
    // init IO
    pinMode(Pin_BUSY, INPUT);
    pinMode(Pin_RESET, OUTPUT);
    pinMode(Pin_PANEL_ON, OUTPUT);
    pinMode(Pin_DISCHARGE, OUTPUT);
    pinMode(Pin_BORDER, OUTPUT);
    pinMode(Pin_EPD_CS, OUTPUT);
    pinMode(Pin_SD_CS, OUTPUT);
    pinMode(Pin_Font_CS, OUTPUT);

    digitalWrite(Pin_RESET, LOW);
    digitalWrite(Pin_PANEL_ON, LOW);
    digitalWrite(Pin_DISCHARGE, LOW);
    digitalWrite(Pin_BORDER, LOW);
    digitalWrite(Pin_EPD_CS, LOW);
    digitalWrite(Pin_SD_CS, HIGH);
    digitalWrite(Pin_Font_CS, HIGH);
    
    // init SPI
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
}

/*********************************************************************************************************
** Function name:           init_io
** Descriptions:            init IO
*********************************************************************************************************/
int ePaper::getTemperature()
{
    int sum = 0;
    for(int i=0; i<32; i++)
    {
        sum += analogRead(Pin_TEMPERATURE);
    }
    sum  = sum >> 5;

    float temp = 209.56-121.7*(float(sum)/1023.0*5.0);

    return (int)temp;
}

ePaper EPAPER;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/