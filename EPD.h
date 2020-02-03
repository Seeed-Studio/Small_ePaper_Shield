// Copyright 2013 Pervasive Displays, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied.  See the License for the specific language
// governing permissions and limitations under the License.

#if !defined(EPD_H)
#define EPD_H 1

#include <Arduino.h>
#include <SPI.h>

#if defined(__MSP430_CPU__)
    #define PROGMEM
#else
    #include <avr/pgmspace.h>
#endif

// if more SRAM available (8 kBytes)
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    #define EPD_ENABLE_EXTRA_SRAM 1
#endif

typedef enum {
    EPD_1_44,        // 128 x 96
    EPD_2_0,         // 200 x 96
    EPD_2_7          // 264 x 176
} EPD_size;

typedef enum {           // Image pixel -> Display pixel
    EPD_compensate,  // B -> W, W -> B (Current Image)
    EPD_white,       // B -> N, W -> W (Current Image)
    EPD_inverse,     // B -> N, W -> B (New Image)
    EPD_normal       // B -> B, W -> W (New Image)
} EPD_stage;

typedef void EPD_reader(void* buffer, uint32_t address, uint16_t length);

class EPD_Class {
  private:

    int EPD_Pin_EPD_CS;
    int EPD_Pin_PANEL_ON;
    int EPD_Pin_BORDER;
    int EPD_Pin_DISCHARGE;
    int EPD_Pin_PWM;
    int EPD_Pin_RESET;
    int EPD_Pin_BUSY;

    EPD_size size;
    uint16_t stage_time;

    uint16_t factored_stage_time;
    uint16_t lines_per_display;
    uint16_t dots_per_line;
    uint16_t bytes_per_line;
    uint16_t bytes_per_scan;
    PROGMEM const uint8_t* gate_source;
    uint16_t gate_source_length;
    PROGMEM const uint8_t* channel_select;
    uint16_t channel_select_length;

    bool filler;

  public:
    unsigned char lineDta[33];

  public:
    // power up and power down the EPD panel
    void begin(EPD_size sz);
    void start();
    void end();

    void setFactor(int temperature = 25) {
        this->factored_stage_time = this->stage_time * this->temperature_to_factor_10x(temperature) / 10;
    }

    // clear display (anything -> white)
    void clear() {
        this->frame_fixed_repeat(0xff, EPD_compensate);
        this->frame_fixed_repeat(0xff, EPD_white);
        this->frame_fixed_repeat(0xaa, EPD_inverse);
        this->frame_fixed_repeat(0xaa, EPD_normal);
    }

    // assuming a clear (white) screen output an image (PROGMEM data)
    void image(PROGMEM const uint8_t* image) {
        this->frame_fixed_repeat(0xaa, EPD_compensate);
        this->frame_fixed_repeat(0xaa, EPD_white);
        this->frame_data_repeat(image, EPD_inverse);
        this->frame_data_repeat(image, EPD_normal);
    }

    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    void image_sd() {
        this->frame_fixed_repeat(0xaa, EPD_compensate);
        this->frame_fixed_repeat(0xaa, EPD_white);
        this->frame_data_repeat_sd(EPD_inverse);
        this->frame_data_repeat_sd(EPD_normal);
    }
    #endif
    // change from old image to new image (PROGMEM data)
    void image(PROGMEM const uint8_t* old_image, PROGMEM const uint8_t* new_image) {
        this->frame_data_repeat(old_image, EPD_compensate);
        this->frame_data_repeat(old_image, EPD_white);
        this->frame_data_repeat(new_image, EPD_inverse);
        this->frame_data_repeat(new_image, EPD_normal);
    }

    #if defined(EPD_ENABLE_EXTRA_SRAM)
    // change from old image to new image (SRAM version)
    void image_sram(unsigned char* new_image) {
        this->frame_fixed_repeat(0xaa, EPD_compensate);
        this->frame_fixed_repeat(0xaa, EPD_white);
        this->frame_sram_repeat(new_image, EPD_inverse);
        this->frame_sram_repeat(new_image, EPD_normal);
    }
    #endif

    // Low level API calls
    // ===================

    // single frame refresh
    void frame_fixed(uint8_t fixed_value, EPD_stage stage);
    void frame_data(PROGMEM const uint8_t* new_image, EPD_stage stage);
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    void frame_data_sd(EPD_stage stage);
    #endif

    #if defined(EPD_ENABLE_EXTRA_SRAM)
    void frame_sram(const uint8_t* new_image, EPD_stage stage);
    #endif
    void frame_cb(uint32_t address, EPD_reader* reader, EPD_stage stage);

    // stage_time frame refresh
    void frame_fixed_repeat(uint8_t fixed_value, EPD_stage stage);
    void frame_data_repeat(PROGMEM const uint8_t* new_image, EPD_stage stage);
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    void frame_data_repeat_sd(EPD_stage stage);
    #endif

    #if defined(EPD_ENABLE_EXTRA_SRAM)
    void frame_sram_repeat(const uint8_t* new_image, EPD_stage stage);
    #endif
    void frame_cb_repeat(uint32_t address, EPD_reader* reader, EPD_stage stage);

    // convert temperature to compensation factor
    int temperature_to_factor_10x(int temperature);

    // single line display - very low-level
    // also has to handle AVR progmem
    void line(uint16_t line, const uint8_t* data, uint8_t fixed_value, bool read_progmem, EPD_stage stage);
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
    void line_sd(uint16_t line, const uint8_t* data, uint8_t fixed_value, bool read_progmem, EPD_stage stage);
    #endif
};

extern EPD_Class EPD;

#endif


