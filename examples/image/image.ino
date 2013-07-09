// demo of ...

#include <Streaming.h>
#include <ePaper.h>
#include <SPI.h>
#include <SD.h>

#include "GT20L16_drive.h"
#include "picture.h"

#define PINCSGT         9


void setup()
{

    Serial.begin(38400);
    
    EPAPER.begin(EPD_2_0);                          // setup epaper, size
    // eSD.begin(4, EPD_2_0);                                   
    // GT20L16.begin(PINCSGT);

    EPAPER.start();
    EPD.image(image_array_200_1);
    EPAPER.end();
    
    

}

void loop()
{
    // add code here
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/