/*
  sd_paper.cpp
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Modified by Loovee
  www.seeedstudio.com
  2013-7-7

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
#include <SD.h>
#include <SPI.h>
#include <Streaming.h>

#include "sd_epaper.h"
#include "ePaperDfs.h"

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
unsigned char sd_epaper::begin(unsigned char pinCs)
{

    openFileTime = 1;
    closeFileTime = 1;
    
    while(!SD.begin(4))
    {
        Serial.println("initialization failed!");
        delay(100);
    }
    
    Serial.println("initialization done.");
    
    openFile();
    
    clear();

}

unsigned char sd_epaper::openFile()
{
    new_image = SD.open(NEWIMAGENAME, FILE_WRITE);
    if(new_image)cout << "new_image open ok: " << openFileTime++ << endl;
    else cout << "new_image_open err: " << openFileTime++ << endl;

}

unsigned char sd_epaper::closeFile()
{
    new_image.close();
    cout << "new_image close : " << closeFileTime++ << endl;
}

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
unsigned char sd_epaper::putLine(int line, unsigned char *dta)
{

    //Serial.print("Writing to NEWIMAGE.ep");
    new_image.seek(line*25);
    new_image.write(dta, 25);


}

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
unsigned char sd_epaper::getLine(int line, unsigned char *dta)
{

    new_image.seek(line*25);
    new_image.read(dta, 25);

}

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
unsigned char sd_epaper::putPixel(int x, int y, unsigned char pixel)
{

    int bit = x & 0x07;
    int byte = x / 8 + y * (pixel_width / 8);
    int mask = 0x01 << bit;

    new_image.seek(byte);
    unsigned char tmp = new_image.read();

    if (BLACK == pixel)
    {
        tmp |= mask;
    }
    else
    {
        tmp &= ~mask;
    }
    new_image.seek(byte);
    new_image.write(tmp);


}

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
unsigned char sd_epaper::getPixel(int x, int y)
{

    int bit = x & 0x07;
    int byte = x / 8 + y * (pixel_width / 8);
    int mask = 0x01 << bit;

    new_image.seek(byte);

    unsigned char tmp = new_image.read();

    if(tmp & (0x01 << bit))tmp = BLACK;
    else tmp = WHITE;

    return tmp;

}


/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
unsigned char sd_epaper::clear()
{

    //openFile();
    new_image.seek(0);
    memset(lineDta, 0x00, 25);

    for(int i=0; i<96; i++)
    {
        new_image.write(lineDta, 25);
    }
    //closeFile();
}


sd_epaper eSD;


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/