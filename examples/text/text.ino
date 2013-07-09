// demo of text
// note: you should insert an SD card if you want to use this demo

#include <ePaper.h>
#include <SPI.h>
#include <SD.h>
#include "GT20L16_drive.h"


#define SCREEN_SIZE 200                 // choose screen size: 144, 200, 270

#if (SCREEN_SIZE == 144)
#define EPD_SIZE    EPD_1_44

#elif (SCREEN_SIZE == 200)
#define EPD_SIZE    EPD_2_0

#elif (SCREEN_SIZE == 270)
#define EPD_SIZE    EPD_2_7

#else
#error "Unknown EPB size: Change the #define SCREEN_SIZE to a supported value"
#endif

void setup()
{

    EPAPER.begin(EPD_SIZE);                             // setup epaper, size 
    EPAPER.setDirection(DIRNORMAL);                     // set display direction
    
    eSD.begin(EPD_SIZE);                                   
    GT20L16.begin();

    int timer1 = millis();
    EPAPER.drawString("SeeedStudio", 10, 10);
    EPAPER.drawNumber(12345, 60, 40);
    EPAPER.drawFloat(-1.25, 2, 90, 65);
    EPAPER.display();                                   // use only once

}

void loop()
{
    // add code here
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/