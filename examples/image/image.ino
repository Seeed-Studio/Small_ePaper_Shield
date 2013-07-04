// demo of epd

#include <inttypes.h>
#include <ctype.h>
#include <SPI.h>
#include <EPD.h>
#include <Streaming.h>

#include "picture.h"
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
    
    EPAPER.init_io();
    EPD.begin();                 // power up the EPD panel
    EPD.setFactor(temp);         // adjust for current temperature
}

unsigned char dtaPixel[25];

void dtaPixel_init()
{

    for(int i=0; i<25; i++)
    {
        // dtaPixel[i] = random(0, 255);
        // dtaPixel[i] = i<12 ? 0 : 0xff;
        dtaPixel[i] = 0x55;
    }

}

void allPixel()
{
    init_epd();
    
    EPD.clear();
    
    EPD.end();
    delay(100);
    init_epd();
    for(int i=0; i<96; i++)
    {
        dtaPixel_init();
       // for(int j=0; j<5; j++)
       // EPD.lineLvc(i, dtaPixel, 0, 0, EPD_normal);
    }
    
    EPD.end();
    
    while(1);

}
void setup()
{

    Serial.begin(115200);
    init_epd();
    EPD.clear();
    EPD.end();
    delay(1000);
    cout << "init over!!" << endl;
}

static int state = 0;

void loop() 
{

    cout << "wait for clear" << endl << endl;
   // EPD.clear();
   // EPD.end();
    //allPixel();
  
   // delay(5000);

    init_epd();
    
    EPD.image(TEXT_BITS);
    
    delay(100);
    //EPD.end();
    delay(100);
    while(1);
    
#if 0
  //  EPD.image(image_array_200_1);
  
  	switch(state) {
	default:
	case 0:         // clear the screen
		EPD.clear();
		state = 1;
		break;

	case 1:         // clear -> text
		EPD.image(TEXT_BITS);
		++state;
		break;

	case 2:         // text -> picture
		EPD.image(PICTURE_BITS);
		++state;
		break;

	case 3:        // picture -> text
		EPD.image(TEXT_BITS);
		state = 1;  // backe to picture nex time
		break;
	}
	EPD.end();   // power down the EPD panel

    delay(100);
   // EPD.end();

    cout << "clear over!" << endl;
    
   // while(1);
#endif

}