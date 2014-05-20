/*
  ePaper.cpp
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

#include <Arduino.h>
#include <SPI.h>
#include "GT20L16_drive.h"
#include "ePaper.h"


static void spi_on()
{
    SPI.begin();
    //SPI.setClockDivider(SPI_CLOCK_DIV2);
    //SPI_put(0x00);
    //SPI_put(0x00);
}

/*********************************************************************************************************
* \brief According to EPD size and temperature to get stage_time
* \note Refer to COG document Section 5.3 for more details
*
* \param EPD_type_index The defined EPD size
**********************************************************************************************************/
void ePaper::begin(EPD_size sz)
{
    size = sz;
    direction = DIRNORMAL;
    
    switch(size)
    {
        case EPD_1_44:              // 128*96
        SIZE_LEN    = 128;
        SIZE_WIDTH  = 96;
        break;
        
        case EPD_2_0:               // 200*96
        SIZE_LEN    = 200;
        SIZE_WIDTH  = 96;
        break;
        
        case EPD_2_7:               // 264*176
        SIZE_LEN    = 264;
        SIZE_WIDTH  = 176;
        break;
        
        default:
        println_ep("wrong size");
        while(1);                   // die here
    }
    
    DISP_LEN    = SIZE_LEN;
    DISP_WIDTH  = SIZE_WIDTH;
    
    EPD.begin(size);
    init_io();
}

void ePaper::spi_detach()
{
}
/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
void ePaper::setDirection(EPD_DIR dir)
{
    direction = dir;
    
    if((direction == DIRLEFT) || (direction == DIRRIGHT))
    {
        DISP_LEN    = SIZE_WIDTH;
        DISP_WIDTH  = SIZE_LEN;
    }
    
    eSD.setDirection(direction);
}
/*********************************************************************************************************
** Function name:           start
** Descriptions:            start
*********************************************************************************************************/
void ePaper::start()
{
    EPD.start();                 // power up the EPD panel
    
    int tmp = getTemperature();
    Serial.print("temperature: ");
    EPD.setFactor(getTemperature());         // adjust for current temperature
}

/*********************************************************************************************************
** Function name:           end
** Descriptions:            end
*********************************************************************************************************/
void ePaper::end()
{
    EPD.end();
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
    
    pinMode(9, OUTPUT);
    digitalWrite(9, HIGH);

    digitalWrite(Pin_RESET, LOW);
    digitalWrite(Pin_PANEL_ON, LOW);
    digitalWrite(Pin_DISCHARGE, LOW);
    digitalWrite(Pin_BORDER, LOW);
    digitalWrite(Pin_EPD_CS, HIGH);
    digitalWrite(Pin_SD_CS, HIGH);
    
    // init SPI
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    //SPI.setClockDivider(SPI_CLOCK_DIV2);
}

/*********************************************************************************************************
** Function name:           getTemperature
** Descriptions:            get temperature of sensor
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

/*********************************************************************************************************
** Function name:           drawUnicode
** Descriptions:            draw a unicode
*********************************************************************************************************/
int ePaper::drawUnicode(unsigned int uniCode, int x, int y)
{
    
   // if(((x+16)>= DISP_LEN) || ((y+16) >= DISP_WIDTH) || x<0 || y<0) return 0;
   

    int dtaLen = GT20L16.getMatrixUnicode(uniCode, tMatrix);


    int pX      = 0;
    int pY      = 0;
    int color   = 0;
    //spi_on();
    for(int k = 0; k<2; k++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<(dtaLen/2); j++)
            {

                if(tMatrix[j+(dtaLen/2)*k] & (0x01<<(7-i)))
                {  
                    color = 1;
                }
                else
                {
                    color = 0;
                }
                
                pX = x + j;
                pY = y + k*8+i;
                
                
                drawPixel(pX, pY, color);
            }
        }
    }

    return dtaLen/2;        // x +
}

int ePaper::drawUnicode(unsigned char *matrix, int x, int y)
{
    
   // if(((x+16)>= DISP_LEN) || ((y+16) >= DISP_WIDTH) || x<0 || y<0) return 0;
   

    int dtaLen  = 32;
    int pX      = 0;
    int pY      = 0;
    int color   = 0;
    //spi_on();
    for(int k = 0; k<2; k++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<(dtaLen/2); j++)
            {

                if(matrix[j+(dtaLen/2)*k] & (0x01<<(7-i)))
                {  
                    color = 1;
                }
                else
                {
                    color = 0;
                }
                
                pX = x + j;
                pY = y + k*8+i;
                
                drawPixel(pX, pY, color);
            }
        }
    }

    return dtaLen/2;        // x +
}

/*********************************************************************************************************
** Function name:           drawUnicodeString
** Descriptions:            draw string
*********************************************************************************************************/
int ePaper::drawUnicodeString(unsigned int *uniCode, int len, int x, int y)
{
    int xPlus = 0;
    int xSum  = 0;
    
    for(int i=0; i<len; i++)
    {
        xPlus = drawUnicode(uniCode[i], x, y);
        x += xPlus;
        xSum += xPlus;
    }
    return xSum;
}

/*********************************************************************************************************
** Function name:           drawChar
** Descriptions:            draw char
*********************************************************************************************************/
int ePaper::drawChar(char c, int x, int y)
{
    return drawUnicode(c, x, y);
}

/*********************************************************************************************************
** Function name:           drawString
** Descriptions:            draw string
*********************************************************************************************************/
int ePaper::drawString(char *string, int poX, int poY)
{
    int sumX = 0;

    while(*string)
    {
        
        int xPlus = drawChar(*string, poX, poY);
        sumX += xPlus;
        *string++;
        
        if(poX < 200)
        {
            poX += xPlus;                                     /* Move cursor right            */
        }
    }
    
    return sumX;
}

/*********************************************************************************************************
** Function name:           drawNumber
** Descriptions:            drawNumber
*********************************************************************************************************/
int ePaper::drawNumber(long long_num,int poX, int poY)
{
    char tmp[10];
    sprintf(tmp, "%d", long_num);
    return drawString(tmp, poX, poY);

}

/*********************************************************************************************************
** Function name:           drawFloat
** Descriptions:            drawFloat
*********************************************************************************************************/
int ePaper::drawFloat(float floatNumber, int decimal, int poX, int poY)
{
    unsigned long temp=0;
    float decy=0.0;
    float rounding = 0.5;
    unsigned char f=0;
    
    float eep = 0.000001;
    
    int sumX    = 0;
    int xPlus   = 0;
    
    if(floatNumber-0.0 < eep)       // floatNumber < 0
    {
        xPlus = drawChar('-',poX, poY);
        floatNumber = -floatNumber;

        poX  += xPlus; 
        sumX += xPlus;
    }
    
    for (unsigned char i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    
    floatNumber += rounding;

    temp = (long)floatNumber;
    
    
    xPlus = drawNumber(temp,poX, poY);

    poX  += xPlus; 
    sumX += xPlus;

    if(decimal>0)
    {
        xPlus = drawChar('.',poX, poY);
        poX += xPlus;                                       /* Move cursor right            */
        sumX += xPlus;
    }
    else
    {
        return sumX;
    }
    
    decy = floatNumber - temp;
    for(unsigned char i=0; i<decimal; i++)                                      
    {
        decy *= 10;                                                      /* for the next decimal         */
        temp = decy;                                                    /* get the decimal              */
        xPlus = drawNumber(temp,poX, poY);
        
        poX += xPlus;                                       /* Move cursor right            */
        sumX += xPlus;
        decy -= temp;
    }
    return sumX;
}

/*********************************************************************************************************
** Function name:           drawLine
** Descriptions:            drawLine
*********************************************************************************************************/
void ePaper::drawLine(int x0, int y0, int x1, int y1)
{

    init_io();
    
    int x = x1-x0;
    int y = y1-y0;
    int dx = abs(x), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2;                                              
    for (;;)
    {                                                          
        drawPixel(x0,y0,1);
        e2 = 2*err;
        if (e2 >= dy) 
        {                                                
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) 
        {                                                
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }
}


/*********************************************************************************************************
** Function name:           clear_sd
** Descriptions:            clear sd card
*********************************************************************************************************/
void ePaper::clear_sd()
{
    
    init_io();
    
    for(int i=0; i<DISP_WIDTH; i++)
    {
        for(int j=0; j<DISP_LEN; j++)
        {
            drawPixel(j, i, 0);
            
        }
    }
}

/*********************************************************************************************************
** Function name:           drawCircle
** Descriptions:            drawCircle
*********************************************************************************************************/
void ePaper::drawCircle(int poX, int poY, int r)
{

    init_io();
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        drawPixel(poX-x, poY+y, 1);
        drawPixel(poX+x, poY+y, 1);
        drawPixel(poX+x, poY-y, 1);
        drawPixel(poX-x, poY-y, 1);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}

/*********************************************************************************************************
** Function name:           drawHorizontalLine
** Descriptions:            drawHorizontalLine
*********************************************************************************************************/
void ePaper::drawHorizontalLine( int poX, int poY, int len)
{
    drawLine(poX, poY, poX+len, poY);
}

/*********************************************************************************************************
** Function name:           drawVerticalLine
** Descriptions:            drawVerticalLine
*********************************************************************************************************/
void ePaper::drawVerticalLine( int poX, int poY, int len)
{
    drawLine(poX, poY, poX, poY+len);
}

/*********************************************************************************************************
** Function name:           drawRectangle
** Descriptions:            drawRectangle
*********************************************************************************************************/
void ePaper::drawRectangle(int poX, int poY, int len, int width)
{
    drawHorizontalLine(poX, poY, len);
    drawHorizontalLine(poX, poY+width, len);
    drawVerticalLine(poX, poY, width);
    drawVerticalLine(poX + len, poY, width);
}

/*********************************************************************************************************
** Function name:           fillCircle
** Descriptions:            fillCircle
*********************************************************************************************************/
void ePaper::fillCircle(int poX, int poY, int r)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    
    do {

        drawVerticalLine(poX-x, poY-y, 2*y);
        drawVerticalLine(poX+x, poY-y, 2*y);

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);

}

/*********************************************************************************************************
** Function name:           drawTraingle
** Descriptions:            drawTraingle
*********************************************************************************************************/
void ePaper::drawTraingle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3)
{
    drawLine(poX1, poY1, poX2, poY2);
    drawLine(poX1, poY1, poX3, poY3);
    drawLine(poX2, poY2, poX3, poY3);
}

/*********************************************************************************************************
** Function name:           drawTraingle
** Descriptions:            drawTraingle
*********************************************************************************************************/
void ePaper::fillRectangle(int poX, int poY, int len, int width)
{
    for(int i=0; i<width; i++)
    {
        drawHorizontalLine(poX, poY+i, len);
    }
}

/*********************************************************************************************************
** Function name:           display
** Descriptions:            you should use this function once after finish drawing
*********************************************************************************************************/
unsigned char ePaper::display()
{
    start();
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    EPD.image_sd();
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    EPD.image_sram(eSD.sram_image);
#endif
    end();
}

ePaper EPAPER;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
