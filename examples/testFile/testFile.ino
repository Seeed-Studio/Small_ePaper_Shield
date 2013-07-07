// demo of epd

#include <inttypes.h>
#include <ctype.h>
#include <SPI.h>
#include <Streaming.h>
#include <SD.h>

#include "ePaper.h"
#include "sd_epaper.h"


File myFile;

unsigned char lineDta[25];


void dispLine()
{
    for(int i=0; i<25; i++)
    {
        cout << lineDta[i] << '\t';
        if((i+1)%5 == 0)cout << endl;
    }
    cout << endl;
}
void setup()
{

    Serial.begin(38400);
    EPAPER.init_io();
    eSD.begin(4);
    
    eSD.putPixel(100, 50, 1);
    
    eSD.getLine(50, lineDta);
    
    dispLine();
    
}

void loop()
{
    // nothing happens after setup
}
