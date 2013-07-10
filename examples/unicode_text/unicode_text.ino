/*-------------------------------------------------------------------------------------------
  demo of display unicode, such as Chinese
  
  loovee
  2013-7-10
  
  note: if you use an Arduin UNO, Seeeduino 3.0 (any board that use Atmega 328P or 32U4) 
        you should insert an SD card if you want to use this demo
        if you use a Arduino Mega(any board that use Atmega1280 or Atmega2560), you neen't
        insert an SD card for this demo
        
        GB2312 table: http://ash.jp/code/cn/gb2312tbl.htm
-------------------------------------------------------------------------------------------*/ 

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

    unsigned int uniCode[5] = {0xc4e3, 0xbac3, '!', '!', '!'};          // this means "how are you" in chinese
    
    EPAPER.begin(EPD_SIZE);                                             // setup epaper, size 
    EPAPER.setDirection(DIRNORMAL);                                     // set display direction
    
    eSD.begin(EPD_SIZE);                                   
    GT20L16.begin();

    EPAPER.drawString("SeeedStudio", 10, 10);
    EPAPER.deawUnicodeString(uniCode, 5, 30, 40);

    EPAPER.display();                                   // use only once

}

void loop()
{
    // add code here
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/