// demo of ...

#include <Streaming.h>
#include <ePaper.h>
#include <SPI.h>

#include "GT20L16_drive.h"


#define PINCSGT         9

unsigned char matrix[32];

void setup()
{
    Serial.begin(38400);
    
    EPAPER.init_io();
    GT20L16.begin(PINCSGT);

    int dtaLen = GT20L16.getMatrixUnicode('S', matrix);
    
    cout << "dtaLen = " << dtaLen << endl;

    for(int k = 0; k<2; k++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<(dtaLen/2); j++)
            {

                if(matrix[j+(dtaLen/2)*k] & (0x01<<(7-i)))
                {
                    cout << "**";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << endl;
        }
    }

    
    cout << endl;

}

void loop()
{
    // add code here
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/