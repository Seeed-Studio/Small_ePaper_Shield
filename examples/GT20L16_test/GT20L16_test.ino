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

    cout << "get matrix over!" << endl;
    
    GT20L16.getMatrixUnicode('ΰ', matrix);
  
    for(int i=0; i<32; i++)
    {
        cout << matrix[i] << '\t';
        if((i+1)%16 == 0)cout << endl;
    }
    
    cout << endl << endl << endl;
      
    
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<16; j++)
        {

            if(matrix[j] & (0x01<<(7-i)))
            {
                cout << "  ";
                //myFile.print("  ");
            }
            else
            {
                cout << "**";
                //myFile.print("**");
            }
        }
        
        cout << endl;
        //myFile.println();
    }
    
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<16; j++)
        {

            if(matrix[j+16] & (0x01<<(7-i)))
            {
                cout << "  ";
                //myFile.print("  ");
            }
            else
            {
                cout << "**";
                //myFile.print("**");
            }
        }
        
        cout << endl;
        //myFile.println();
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