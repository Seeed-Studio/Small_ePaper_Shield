// demo of epd

#include <inttypes.h>
#include <ctype.h>
#include <SPI.h>
#include <EPD.h>
#include <Streaming.h>

#include "ePaper.h"

// supported sizes: 144 20 27
#define SCREEN_SIZE 20 // 144 20 27

#if (SCREEN_SIZE == 144)
#define EPD_SIZE EPD_1_44
#define TEXT_IMAGE "text_image_1_44.xbm"
#define TEXT_BITS text_image_1_44_bits
#define PICTURE "cat_1_44.xbm"
#define PICTURE_BITS cat_1_44_bits

#elif (SCREEN_SIZE == 20)
#define EPD_SIZE EPD_2_0
#define TEXT_BITS text_image_2_0_bits
#define TEXT_IMAGE "text_image_2_0.xbm"
#define PICTURE "cat_2_0.xbm"
#define PICTURE_BITS cat_2_0_bits

#elif (SCREEN_SIZE == 27)
#define EPD_SIZE EPD_2_7
#define TEXT_IMAGE "text_hello_2_7.xbm"
#define TEXT_BITS text_hello_2_7_bits
#define PICTURE "cat_2_7.xbm"
#define PICTURE_BITS cat_2_7_bits

#else
#error Unknown EPB size
#endif

// Error message for MSP430
#if (SCREEN_SIZE == 27) && defined(__MSP430_CPU__)
#error MSP430: not enough memory
#endif

// no futher changed below this point

// current version number
#define DEMO_VERSION "1"


// Add Images library to compiler path
#include <Images.h>  // this is just an empty file

// images
PROGMEM const
#define unsigned
#define char uint8_t
#include TEXT_IMAGE
#undef char
#undef unsigned

PROGMEM const
#define unsigned
#define char uint8_t
#include PICTURE
#undef char
#undef unsigned

// pre-processor convert to string
#define MAKE_STRING1(X) #X
#define MAKE_STRING(X) MAKE_STRING1(X)

// define the E-Ink display
EPD_Class EPD(EPD_SIZE, Pin_PANEL_ON, Pin_BORDER, Pin_DISCHARGE, Pin_PWM, Pin_RESET, Pin_BUSY, Pin_EPD_CS, SPI);

void init_epd()
{
    Serial.println("Demo version: " DEMO_VERSION);
    Serial.println("Display: " MAKE_STRING(EPD_SIZE));
    Serial.println();
    
    int temp = EPAPER.getTemperature();
    cout << "temperature: " << temp << endl;
    delay(100);
    EPD.begin();                 // power up the EPD panel
    EPD.setFactor(temp);         // adjust for current temperature
}

void setup()
{

    Serial.begin(115200);
    EPAPER.init_io();
    init_epd();
    cout << "init over!!" << endl;
}

void loop() 
{

    cout << "wait for clear" << endl << endl;
    EPD.clear();

    delay(5000);
    cout << "clear over!" << endl;
    
    while(1);


}
