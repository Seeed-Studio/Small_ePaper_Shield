// demo of ...

#include <Streaming.h>
#include <ePaper.h>
#include <SPI.h>
#include <SD.h>

#include "GT20L16_drive.h"
#include "picture.h"

#define PINCSGT         9

// Change this for different display size
// supported sizes: 144 200 270
#define SCREEN_SIZE 200


// set up images from screen size2
#if (SCREEN_SIZE == 144)
#define EPD_SIZE    EPD_1_44
#define IMAGEFILE   image_144

#elif (SCREEN_SIZE == 200)
#define EPD_SIZE    EPD_2_0
#define IMAGEFILE   image_200

#elif (SCREEN_SIZE == 270)
#define EPD_SIZE    EPD_2_7
#define IMAGEFILE   image_270

#else
#error "Unknown EPB size: Change the #define SCREEN_SIZE to a supported value"
#endif


void setup()
{

    Serial.begin(38400);
    
    EPAPER.begin(EPD_SIZE);                          // setup epaper, size
    EPAPER.start();
    EPD.image(IMAGEFILE);
    EPAPER.end();

}

void loop()
{
    // add code here
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/