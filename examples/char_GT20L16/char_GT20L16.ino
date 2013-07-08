// demo of ...

#include <Streaming.h>
#include <ePaper.h>
#include <SPI.h>
#include <SD.h>

#include "GT20L16_drive.h"

#define PINCSGT         9

unsigned char matrix[32];

void dispMatrix(int len, unsigned char *Matrix)
{

    //eSD.openFile();
    for(int k = 0; k<2; k++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<(len/2); j++)
            {

                if(Matrix[j+(len/2)*k] & (0x01<<(7-i)))
                {
                    //Serial.print("**");
                    Serial.write(random('A', 'Z'));         // dandom...fuck
                    Serial.write(random('A', 'Z'));         // ...    
                }
                else
                {
                    Serial.print("  ");
                }
            }
            Serial.println();
        }
    }
    //eSD.closeFile();
}

void dispMatrixs(int len, unsigned int *unicode)
{
    for(int i=0; i<len; i++)
    {
        int dtaLen = GT20L16.getMatrixUnicode(unicode[i], matrix);
        dispMatrix(dtaLen, matrix);
    }
}

unsigned char dispUnicodeGT(int x, int y, unsigned int uniCode)
{
    unsigned char tmpMatrix[32];
    
    int dtaLen = GT20L16.getMatrixUnicode(uniCode, tmpMatrix);
    
    cout << "dtaLen = " << dtaLen << endl;

    int pX      = 0;
    int pY      = 0;
    int color   = 0;
    
    //eSD.openFile();
    for(int k = 0; k<2; k++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<(dtaLen/2); j++)
            {

                if(tmpMatrix[j+(dtaLen/2)*k] & (0x01<<(7-i)))
                {
                    //Serial.print("**");
                    Serial.write(random('A', 'Z'));         // dandom...fuck
                    Serial.write(random('A', 'Z'));         // ...    
                    color = 1;
                }
                else
                {
                    Serial.print("  ");
                    color = 0;
                }
                
                pX = x + j;
                pY = y + k*8+i;
                
                EPAPER.drawPixel(pX, pY, color);
            }
            Serial.println();
        }
    }
    //eSD.closeFile();
    
    cout << "out dispUnicodeGT" << endl;
    
    
    
    return dtaLen;
}

void setup()
{
    Serial.begin(38400);
    
    EPAPER.begin(EPD_2_0);
    eSD.begin(4);
    GT20L16.begin(PINCSGT);

    int x = 10;
    int y = 15;
    
    EPAPER.drawString("hello world!!", 10, 10);
    EPAPER.drawNumber(-129, 30, 50);
    
    EPAPER.drawFloat(1.25, 2, 70, 50);
    
    EPAPER.drawLine(50, 50, 0, 0);
    
    EPAPER.drawCircle(40, 50, 20);
    EPAPER.fillRectangle(10, 10, 60, 30);
    cout << endl;
    
    cout << "the end" << endl;
    
    EPAPER.display();

}

void loop()
{
    // add code here
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/