// demo of ...

#include <ePaper.h>
#include <SPI.h>
#include <SD.h>

#include "GT20L16_drive.h"

#define PINCSGT         9

unsigned char matrix[32];

void dispMatrix(int len, unsigned char *Matrix)
{

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
}

void dispMatrixs(int len, unsigned int *unicode)
{
    for(int i=0; i<len; i++)
    {
        int dtaLen = GT20L16.getMatrixUnicode(unicode[i], matrix);
        dispMatrix(dtaLen, matrix);
    }
}

void setup()
{
    Serial.begin(38400);
    
    EPAPER.init_io();
    GT20L16.begin(PINCSGT);

    unsigned int utest[3] = {'2', 'b', 'k'};
    dispMatrixs(3, utest);


}

void loop()
{
    // add code here
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/