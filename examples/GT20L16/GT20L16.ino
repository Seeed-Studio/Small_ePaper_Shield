/*  -------------------------------------------------------------------------------------------
    demo of GT20L16, a font chip, you can read some matrix from GT20L16,
    and display it in serialmonitor

    loovee
    2013-7-10
    -------------------------------------------------------------------------------------------*/
#include <ePaper.h>
#include <SPI.h>
#include <SD.h>

#include "GT20L16_drive.h"

unsigned char matrix[32];

void dispMatrix(int len, unsigned char* Matrix) {

    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < (len / 2); j++) {
                if (Matrix[j + (len / 2)*k] & (0x01 << (7 - i))) {
                    Serial.write(random('A', 'Z'));         // random...
                    Serial.write(random('A', 'Z'));         // ...
                } else {
                    Serial.print("  ");
                }
            }
            Serial.println();
        }
    }
}

void dispMatrixs(int len, unsigned int* unicode) {
    for (int i = 0; i < len; i++) {
        int dtaLen = GT20L16.getMatrixUnicode(unicode[i], matrix);
        dispMatrix(dtaLen, matrix);
        Serial.println();
        delay(20);
    }
}

void setup() {
    Serial.begin(38400);
    EPAPER.init_io();
    GT20L16.begin();

    unsigned int utest[2] = {0xc4e3, 0xbac3};           // this means "how are you" in chinese
    dispMatrixs(2, utest);
}

void loop() {
    // add code here
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/