/*
    sd_paper.cpp
    2013 Copyright (c) Seeed Technology Inc.  All right reserved.

    Modified by Loovee
    www.seeedstudio.com
    2013-7-7

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
#include <SD.h>
#include <SPI.h>

#include "sd_epaper.h"
#include "ePaperDfs.h"


static void spi_on() {
    SPI.begin();
    //SPI.setClockDivider(SPI_CLOCK_DIV2);
    //SPI_put(0x00);
    //SPI_put(0x00);
}
/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
void sd_epaper::begin(EPD_size sz) {

    openFileTime = 1;
    closeFileTime = 1;
    //spi_on();
    switch (sz) {
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
            println_sd("wrong size");
            while (1);                  // die here
    }

    DISP_LEN    = SIZE_LEN;
    DISP_WIDTH  = SIZE_WIDTH;

    LINE_BYTE = SIZE_LEN / 8;
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    while (!SD.begin(Pin_SD_CS)) {
        println_sd("sd card initialization failed!");
        delay(100);
    }

    println_sd("initialization done.");

    openFile();
    #endif
    clear();

}


void sd_epaper::setDirection(EPD_DIR dir) {
    direction = dir;

    if ((direction == DIRLEFT) || (direction == DIRRIGHT)) {
        DISP_LEN    = SIZE_WIDTH;
        DISP_WIDTH  = SIZE_LEN;
    }
}

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
unsigned char sd_epaper::openFile() {
    //spi_on();
    new_image = SD.open(NEWIMAGENAME, FILE_WRITE);
    if (new_image) {
        println_sd("new_image open ok");
    } else {
        println_sd("new_image_open err: ");
    }
}

unsigned char sd_epaper::closeFile() {
    new_image.close();
}
#endif
/*********************************************************************************************************
** Function name:           putLine
** Descriptions:            putLine
*********************************************************************************************************/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
unsigned char sd_epaper::putLine(int line, unsigned char* dta) {
    //spi_on();
    new_image.seek(line * LINE_BYTE);
    new_image.write(dta, LINE_BYTE);
}
#endif
/*********************************************************************************************************
** Function name:           getLine
** Descriptions:            getLine
*********************************************************************************************************/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
unsigned char sd_epaper::getLine(int line, unsigned char* dta) {
    //spi_on();
    new_image.seek(line * LINE_BYTE);
    new_image.read(dta, LINE_BYTE);
}
#endif
/*********************************************************************************************************
** Function name:           putPixel
** Descriptions:            putPixel
*********************************************************************************************************/
void sd_epaper::putPixel(int x, int y, unsigned char pixel) {
    int x1 = x;
    int y1 = y;

    switch (direction) {
        case DIRLEFT:

            x = y1;
            y = SIZE_WIDTH - x1;

            break;

        case DIRRIGHT:

            x = SIZE_LEN - y1;
            y = x1;
            break;

        case DIRDOWN:

            x = SIZE_LEN - x1;
            y = SIZE_WIDTH - y1;
            break;

        default:

            ;
    }

    if (x > DISP_LEN || y > DISP_WIDTH) {
        return;
    }

    int bit = x & 0x07;
    int byte = (x >> 3) + y * LINE_BYTE;

    int mask = 0x01 << bit;

    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    new_image.seek(byte);
    unsigned char tmp = new_image.read();

    if ((tmp & mask) == (pixel << bit)) {
        return ;
    }

    if (BLACK == pixel) {
        tmp |= mask;
    } else {
        tmp &= ~mask;
    }

    new_image.seek(byte);
    new_image.write(tmp);
    #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    if (BLACK == pixel) {
        sram_image[byte] |= mask;
    } else {
        sram_image[byte] &= ~mask;
    }

    #endif

}

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
void sd_epaper::getPixel(int x, int y) {

    int bit = x & 0x07;
    int byte = x / 8 + y * (SIZE_LEN / 8);
    int mask = 0x01 << bit;

    new_image.seek(byte);

    unsigned char tmp = new_image.read();

    if (tmp & (0x01 << bit)) {
        tmp = BLACK;
    } else {
        tmp = WHITE;
    }
}
#endif

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
unsigned char sd_epaper::clear() {
    //spi_on();
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)

    new_image.seek(0);

    memset(lineDta, 0x00, LINE_BYTE);


    for (int i = 0; i < SIZE_WIDTH; i++) {
        new_image.write(lineDta, LINE_BYTE);
    }

    #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    memset(sram_image, 0x00, 5808);
    #endif

}

sd_epaper eSD;

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
