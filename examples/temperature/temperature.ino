/*  -------------------------------------------------------------------------------------------
    A simple demo to display the temperature, a counter, and to show refreshing the display
    ever few seconds

    harrisonhjones (harrison@hhj.me)
    Nov(11) 30, 2014

    note: if you use an Arduin UNO, Seeeduino 3.0 (any board that use Atmega 328P or 32U4)
        you should insert an SD card if you want to use this demo
        if you use a Arduino Mega(any board that use Atmega1280 or Atmega2560), you neen't
        insert an SD card for this demo

    note 2: I have no idea if the following demo will work with an SD card inserted (as per
        note #1). It's possible that clearDisplay() which is an alias for clear_sd won't
        clear the display with an SD card inserted. Feel free to submit an issue on the
        github repo if that turns out to be the case

    note 3: GB2312 table: http://ash.jp/code/cn/gb2312tbl.htm
    -------------------------------------------------------------------------------------------*/

#include <ePaper.h>
#include <SPI.h>
#include <SD.h>
#include "GT20L16_drive.h"

#define SCREEN_SIZE 270                 // choose screen size: 144, 200, 270

#if (SCREEN_SIZE == 144)
    #define EPD_SIZE    EPD_1_44

#elif (SCREEN_SIZE == 200)
    #define EPD_SIZE    EPD_2_0

#elif (SCREEN_SIZE == 270)
    #define EPD_SIZE    EPD_2_7

#else
    #error "Unknown EPB size: Change the #define SCREEN_SIZE to a supported value"
#endif

int x;
char tmpTemp[15];

void setup() {
    EPAPER.begin(EPD_SIZE);                                             // setup epaper, size
    EPAPER.setDirection(DIRNORMAL);                                     // set display direction
    eSD.begin(EPD_SIZE);
    GT20L16.begin();
}

void loop() {
    // Draw the "title"
    EPAPER.drawStringBig("SeeedStudio", 10, 10);
    EPAPER.drawString("Small e-Paper Display", 10, 40);

    // Draw the temperature
    sprintf(tmpTemp, "Temp: %d C", EPAPER.getTemperature());
    EPAPER.drawStringBig(tmpTemp, 10, 70);

    // Draw the count. This isn't the best way to do it (the number will
    // eventually flow over the count text) but it's just a simple example

    EPAPER.drawString("Count: ", 10, 110);
    EPAPER.drawNumber(x, 80, 110);
    x++;

    EPAPER.display();

    delay(5000);

    EPAPER.clearDisplay();
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/