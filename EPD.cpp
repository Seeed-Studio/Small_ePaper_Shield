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


#include <Arduino.h>
#include <limits.h>
#include <SPI.h>
#include <SD.h>

#include "sd_epaper.h"

#include "EPD.h"

// delays - more consistent naming
#define Delay_ms(ms) delay(ms)
#define Delay_us(us) delayMicroseconds(us)

// inline arrays
#define ARRAY(type, ...) ((type[]){__VA_ARGS__})
#define CU8(...) (ARRAY(const uint8_t, __VA_ARGS__))


static void PWM_start(int pin);
static void PWM_stop(int pin);

static void SPI_put(uint8_t c);
static void SPI_put_wait(uint8_t c, int busy_pin);
static void SPI_send(uint8_t cs_pin, const uint8_t* buffer, uint16_t length);
static void SPI_on();
static void SPI_off();
static void SPI_off_final();

void EPD_Class::begin(EPD_size sz) {

    EPD_Pin_EPD_CS      = Pin_EPD_CS;
    EPD_Pin_PANEL_ON    = Pin_PANEL_ON;
    EPD_Pin_BORDER      = Pin_BORDER;
    EPD_Pin_DISCHARGE   = Pin_DISCHARGE;
    EPD_Pin_PWM         = Pin_PWM;
    EPD_Pin_RESET       = Pin_RESET;
    EPD_Pin_BUSY        = Pin_BUSY;

    this->size = sz;
    this->stage_time = 480; // milliseconds
    this->lines_per_display = 96;
    this->dots_per_line = 128;
    this->bytes_per_line = 128 / 8;
    this->bytes_per_scan = 96 / 4;
    this->filler = false;


    // display size dependant items
    {
        static uint8_t cs[] = {0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00};
        static uint8_t gs[] = {0x72, 0x03};
        this->channel_select = cs;
        this->channel_select_length = sizeof(cs);
        this->gate_source = gs;
        this->gate_source_length = sizeof(gs);
    }

    // set up size structure
    switch (size) {
        default:
        case EPD_1_44:  // default so no change
            break;

        case EPD_2_0: {
                this->lines_per_display = 96;
                this->dots_per_line = 200;
                this->bytes_per_line = 200 / 8;
                this->bytes_per_scan = 96 / 4;
                this->filler = true;
                static uint8_t cs[] = {0x72, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x00};
                static uint8_t gs[] = {0x72, 0x03};
                this->channel_select = cs;
                this->channel_select_length = sizeof(cs);
                this->gate_source = gs;
                this->gate_source_length = sizeof(gs);
                break;
            }

        case EPD_2_7: {
                this->stage_time = 630; // milliseconds
                this->lines_per_display = 176;
                this->dots_per_line = 264;
                this->bytes_per_line = 264 / 8;
                this->bytes_per_scan = 176 / 4;
                this->filler = true;
                static uint8_t cs[] = {0x72, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00};
                static uint8_t gs[] = {0x72, 0x00};
                this->channel_select = cs;
                this->channel_select_length = sizeof(cs);
                this->gate_source = gs;
                this->gate_source_length = sizeof(gs);
                break;
            }
    }

    this->factored_stage_time = this->stage_time;
}


void EPD_Class::start() {

    // power up sequence
    //SPI_put(0x00);

    digitalWrite(this->EPD_Pin_RESET, LOW);
    digitalWrite(this->EPD_Pin_PANEL_ON, LOW);
    digitalWrite(this->EPD_Pin_DISCHARGE, LOW);
    digitalWrite(this->EPD_Pin_BORDER, LOW);
    digitalWrite(this->EPD_Pin_EPD_CS, LOW);

    SPI_on();
    PWM_start(this->EPD_Pin_PWM);
    Delay_ms(5);
    digitalWrite(this->EPD_Pin_PANEL_ON, HIGH);
    Delay_ms(10);

    digitalWrite(this->EPD_Pin_RESET, HIGH);
    digitalWrite(this->EPD_Pin_BORDER, HIGH);
    digitalWrite(this->EPD_Pin_EPD_CS, HIGH);
    Delay_ms(5);

    digitalWrite(this->EPD_Pin_RESET, LOW);
    Delay_ms(5);

    digitalWrite(this->EPD_Pin_RESET, HIGH);
    Delay_ms(5);

    // wait for COG to become ready

    while (HIGH == digitalRead(this->EPD_Pin_BUSY)) {
    }

    // channel select
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x01), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, this->channel_select, this->channel_select_length);

    // DC/DC frequency
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x06), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0xff), 2);

    // high power mode osc
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x07), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x9d), 2);


    // disable ADC
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x08), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x00), 2);

    // Vcom level
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x09), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0xd0, 0x00), 3);

    // gate and source voltage levels
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x04), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, this->gate_source, this->gate_source_length);

    Delay_ms(5);  //???

    // driver latch on
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x03), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x01), 2);

    // driver latch off
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x03), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x00), 2);

    Delay_ms(5);

    // charge pump positive voltage on
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x05), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x01), 2);

    // final delay before PWM off
    Delay_ms(30);
    PWM_stop(this->EPD_Pin_PWM);

    // charge pump negative voltage on
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x05), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x03), 2);

    Delay_ms(30);

    // Vcom driver on
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x05), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x0f), 2);

    Delay_ms(30);

    // output enable to disable
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x02), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x24), 2);

    SPI_off();
}


void EPD_Class::end() {
    // dummy frame
    //this->frame_fixed(0x55, EPD_normal);
    // dummy line and border
    if (EPD_1_44 == this->size) {
        // only for 1.44" EPD
        this->line(0x7fffu, 0, 0xaa, false, EPD_normal);

        Delay_ms(250);

    } else {
        // all other display sizes
        this->line(0x7fffu, 0, 0x55, false, EPD_normal);

        Delay_ms(25);

        digitalWrite(this->EPD_Pin_BORDER, LOW);
        Delay_ms(250);
        digitalWrite(this->EPD_Pin_BORDER, HIGH);
    }
    SPI_on();
    // latch reset turn on
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x03), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x01), 2);

    // output enable off
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x02), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x05), 2);

    // Vcom power off
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x05), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x0e), 2);

    // power off negative charge pump
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x05), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x02), 2);

    // discharge
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x04), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x0c), 2);

    Delay_ms(120);

    // all charge pumps off
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x05), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x00), 2);

    // turn of osc
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x07), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x0d), 2);

    // discharge internal - 1
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x04), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x50), 2);

    Delay_ms(40);

    // discharge internal - 2
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x04), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0xA0), 2);

    Delay_ms(40);

    // discharge internal - 3
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x04), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x00), 2);
    // turn of power and all signals
    //

    delay(10);
    digitalWrite(this->EPD_Pin_RESET, LOW);
    digitalWrite(this->EPD_Pin_PANEL_ON, LOW);
    digitalWrite(this->EPD_Pin_BORDER, LOW);

    SPI_off_final();
    //	digitalWrite(this->EPD_Pin_EPD_CS, LOW);

    // discharge pulse
    digitalWrite(this->EPD_Pin_DISCHARGE, HIGH);
    Delay_ms(250);
    digitalWrite(this->EPD_Pin_DISCHARGE, LOW);

    digitalWrite(EPD_Pin_EPD_CS, HIGH);

}


// convert a temperature in Celcius to
// the scale factor for frame_*_repeat methods
int EPD_Class::temperature_to_factor_10x(int temperature) {
    if (temperature <= -10) {
        return 170;
    } else if (temperature <= -5) {
        return 120;
    } else if (temperature <= 5) {
        return 80;
    } else if (temperature <= 10) {
        return 40;
    } else if (temperature <= 15) {
        return 30;
    } else if (temperature <= 20) {
        return 20;
    } else if (temperature <= 40) {
        return 10;
    }
    return 7;
}


// One frame of data is the number of lines * rows. For example:
// The 1.44” frame of data is 96 lines * 128 dots.
// The 2” frame of data is 96 lines * 200 dots.
// The 2.7” frame of data is 176 lines * 264 dots.

// the image is arranged by line which matches the display size
// so smallest would have 96 * 32 bytes

void EPD_Class::frame_fixed(uint8_t fixed_value, EPD_stage stage) {
    for (uint8_t line = 0; line < this->lines_per_display ; ++line) {
        this->line(line, 0, fixed_value, false, stage);
    }
}

void EPD_Class::frame_data(PROGMEM const uint8_t* image, EPD_stage stage) {
    for (uint8_t line = 0; line < this->lines_per_display ; ++line) {
        this->line(line, &image[line * this->bytes_per_line], 0, true, stage);
    }
}

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
void EPD_Class::frame_data_sd(EPD_stage stage) {
    for (uint8_t line = 0; line < this->lines_per_display ; ++line) {
        SPI_on();
        eSD.getLine(line, lineDta);
        this->line(line, lineDta, 0, 0, stage);
    }
}
#endif

#if defined(EPD_ENABLE_EXTRA_SRAM)
void EPD_Class::frame_sram(const uint8_t* image, EPD_stage stage) {
    for (uint8_t line = 0; line < this->lines_per_display ; ++line) {
        this->line(line, &image[line * this->bytes_per_line], 0, false, stage);
    }
}
#endif


void EPD_Class::frame_cb(uint32_t address, EPD_reader* reader, EPD_stage stage) {
    static uint8_t buffer[264 / 8];
    for (uint8_t line = 0; line < this->lines_per_display; ++line) {
        reader(buffer, address + line * this->bytes_per_line, this->bytes_per_line);
        this->line(line, buffer, 0, false, stage);
    }
}


void EPD_Class::frame_fixed_repeat(uint8_t fixed_value, EPD_stage stage) {
    long stage_time = this->factored_stage_time;

    do {

        unsigned long t_start = millis();
        this->frame_fixed(fixed_value, stage);
        unsigned long t_end = millis();
        if (t_end > t_start) {
            stage_time -= t_end - t_start;
        } else {
            stage_time -= t_start - t_end + 1 + ULONG_MAX;
        }
    } while (stage_time > 0);
}


void EPD_Class::frame_data_repeat(PROGMEM const uint8_t* image, EPD_stage stage) {

    long stage_time = this->factored_stage_time;

    #if 1

    do {
        unsigned long t_start = millis();
        this->frame_data(image, stage);
        unsigned long t_end = millis();
        if (t_end > t_start) {
            stage_time -= t_end - t_start;
        } else {
            stage_time -= t_start - t_end + 1 + ULONG_MAX;
        }
    } while (stage_time > 0);
    #else

    for (int i = 0; i < 7; i++) {
        this->frame_data(image, stage);
    }
    #endif
}

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
void EPD_Class::frame_data_repeat_sd(EPD_stage stage) {

    long stage_time = this->factored_stage_time;


    for (int i = 0; i < 7; i++) {
        unsigned long t_start = millis();
        this->frame_data_sd(stage);
        unsigned long t_end = millis();
        if (t_end > t_start) {
            stage_time -= t_end - t_start;
        } else {
            stage_time -= t_start - t_end + 1 + ULONG_MAX;
        }
    }
}
#endif

#if defined(EPD_ENABLE_EXTRA_SRAM)
void EPD_Class::frame_sram_repeat(const uint8_t* image, EPD_stage stage) {

    #if 1
    long stage_time = this->factored_stage_time;
    do {
        unsigned long t_start = millis();
        this->frame_sram(image, stage);
        unsigned long t_end = millis();
        if (t_end > t_start) {
            stage_time -= t_end - t_start;
        } else {
            stage_time -= t_start - t_end + 1 + ULONG_MAX;
        }
    } while (stage_time > 0);
    #else
    for (int i = 0; i < 7; i++) {
        this->frame_sram(image, stage);
    }
    #endif
}
#endif


void EPD_Class::frame_cb_repeat(uint32_t address, EPD_reader* reader, EPD_stage stage) {
    long stage_time = this->factored_stage_time;
    do {
        unsigned long t_start = millis();
        this->frame_cb(address, reader, stage);
        unsigned long t_end = millis();
        if (t_end > t_start) {
            stage_time -= t_end - t_start;
        } else {
            stage_time -= t_start - t_end + 1 + ULONG_MAX;
        }
    } while (stage_time > 0);
}


void EPD_Class::line(uint16_t line, const uint8_t* data, uint8_t fixed_value, bool read_progmem, EPD_stage stage) {
    // charge pump voltage levels
    SPI_on();
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x04), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, this->gate_source, this->gate_source_length);

    // send data
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x0a), 2);
    Delay_us(10);

    // CS low
    digitalWrite(this->EPD_Pin_EPD_CS, LOW);
    SPI_put_wait(0x72, this->EPD_Pin_BUSY);

    // even pixels
    for (uint16_t b = this->bytes_per_line; b > 0; --b) {
        if (0 != data) {

            // AVR has multiple memory spaces
            uint8_t pixels;
            if (read_progmem) {
                pixels = pgm_read_byte_near(data + b - 1) & 0xaa;
            } else {
                pixels = data[b - 1] & 0xaa;
            }

            switch (stage) {
                case EPD_compensate:  // B -> W, W -> B (Current Image)
                    pixels = 0xaa | ((pixels ^ 0xaa) >> 1);
                    break;

                case EPD_white:       // B -> N, W -> W (Current Image)
                    pixels = 0x55 + ((pixels ^ 0xaa) >> 1);
                    break;

                case EPD_inverse:     // B -> N, W -> B (New Image)
                    pixels = 0x55 | (pixels ^ 0xaa);
                    break;

                case EPD_normal:       // B -> B, W -> W (New Image)
                    pixels = 0xaa | (pixels >> 1);
                    break;
            }

            SPI_put_wait(pixels, this->EPD_Pin_BUSY);
            //delay(10);
        } else {
            SPI_put_wait(fixed_value, this->EPD_Pin_BUSY);
        }
    }

    // scan line
    for (uint16_t b = 0; b < this->bytes_per_scan; ++b) {
        if (line / 4 == b) {
            SPI_put_wait(0xc0 >> (2 * (line & 0x03)), this->EPD_Pin_BUSY);
        } else {
            SPI_put_wait(0x00, this->EPD_Pin_BUSY);
        }
        //delay(10);
    }

    // odd pixels
    for (uint16_t b = 0; b < this->bytes_per_line; ++b) {
        if (0 != data) {
            // AVR has multiple memory spaces
            uint8_t pixels;
            if (read_progmem) {
                pixels = pgm_read_byte_near(data + b) & 0x55;
            } else {
                pixels = data[b] & 0x55;
            }

            switch (stage) {
                case EPD_compensate:  // B -> W, W -> B (Current Image)
                    pixels = 0xaa | (pixels ^ 0x55);
                    break;
                case EPD_white:       // B -> N, W -> W (Current Image)
                    pixels = 0x55 + (pixels ^ 0x55);
                    break;
                case EPD_inverse:     // B -> N, W -> B (New Image)
                    pixels = 0x55 | ((pixels ^ 0x55) << 1);
                    break;
                case EPD_normal:       // B -> B, W -> W (New Image)
                    pixels = 0xaa | pixels;
                    break;
            }
            uint8_t p1 = (pixels >> 6) & 0x03;
            uint8_t p2 = (pixels >> 4) & 0x03;
            uint8_t p3 = (pixels >> 2) & 0x03;
            uint8_t p4 = (pixels >> 0) & 0x03;
            pixels = (p1 << 0) | (p2 << 2) | (p3 << 4) | (p4 << 6);
            SPI_put_wait(pixels, this->EPD_Pin_BUSY);
        } else {
            SPI_put_wait(fixed_value, this->EPD_Pin_BUSY);
        }
        //delay(10);
    }

    if (this->filler) {
        SPI_put_wait(0x00, this->EPD_Pin_BUSY);
    }

    // CS high
    digitalWrite(this->EPD_Pin_EPD_CS, HIGH);

    // output data to panel
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x02), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x2f), 2);
    SPI_off();
}

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega328P__)
void EPD_Class::line_sd(uint16_t line, const uint8_t* data, uint8_t fixed_value, bool read_progmem, EPD_stage stage) {
    // charge pump voltage levels
    SPI_on();
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x04), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, this->gate_source, this->gate_source_length);

    // send data
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x0a), 2);
    Delay_us(10);

    // CS low
    digitalWrite(this->EPD_Pin_EPD_CS, LOW);
    SPI_put_wait(0x72, this->EPD_Pin_BUSY);

    // even pixels
    for (uint16_t b = this->bytes_per_line; b > 0; --b)
        //for (int b=10; b > 0; --b)
    {
        if (0 != data) {

            // AVR has multiple memory spaces
            uint8_t pixels;
            if (read_progmem) {
                pixels = pgm_read_byte_near(data + b - 1) & 0xaa;
            } else {
                pixels = data[b - 1] & 0xaa;
            }

            switch (stage) {
                case EPD_compensate:  // B -> W, W -> B (Current Image)
                    pixels = 0xaa | ((pixels ^ 0xaa) >> 1);
                    break;

                case EPD_white:       // B -> N, W -> W (Current Image)
                    pixels = 0x55 + ((pixels ^ 0xaa) >> 1);
                    break;

                case EPD_inverse:     // B -> N, W -> B (New Image)
                    pixels = 0x55 | (pixels ^ 0xaa);
                    break;

                case EPD_normal:       // B -> B, W -> W (New Image)
                    pixels = 0xaa | (pixels >> 1);
                    break;
            }

            SPI_put_wait(pixels, this->EPD_Pin_BUSY);
            //delay(10);
        } else {
            SPI_put_wait(fixed_value, this->EPD_Pin_BUSY);
        }
    }

    // scan line
    for (uint16_t b = 0; b < this->bytes_per_scan; ++b) {
        if (line / 4 == b) {
            SPI_put_wait(0xc0 >> (2 * (line & 0x03)), this->EPD_Pin_BUSY);
        } else {
            SPI_put_wait(0x00, this->EPD_Pin_BUSY);
        }
        //delay(10);
    }

    // odd pixels
    for (uint16_t b = this->bytes_per_line; b > 0; --b)
        //for (int b=10; b > 0; --b)
    {
        if (0 != data) {
            // AVR has multiple memory spaces
            uint8_t pixels;
            if (read_progmem) {
                pixels = pgm_read_byte_near(data + b) & 0x55;
            } else {
                pixels = data[b] & 0x55;
            }

            switch (stage) {
                case EPD_compensate:  // B -> W, W -> B (Current Image)
                    pixels = 0xaa | (pixels ^ 0x55);
                    break;
                case EPD_white:       // B -> N, W -> W (Current Image)
                    pixels = 0x55 + (pixels ^ 0x55);
                    break;
                case EPD_inverse:     // B -> N, W -> B (New Image)
                    pixels = 0x55 | ((pixels ^ 0x55) << 1);
                    break;
                case EPD_normal:       // B -> B, W -> W (New Image)
                    pixels = 0xaa | pixels;
                    break;
            }
            uint8_t p1 = (pixels >> 6) & 0x03;
            uint8_t p2 = (pixels >> 4) & 0x03;
            uint8_t p3 = (pixels >> 2) & 0x03;
            uint8_t p4 = (pixels >> 0) & 0x03;
            pixels = (p1 << 0) | (p2 << 2) | (p3 << 4) | (p4 << 6);
            SPI_put_wait(pixels, this->EPD_Pin_BUSY);
        } else {
            SPI_put_wait(fixed_value, this->EPD_Pin_BUSY);
        }
        //delay(10);
    }

    if (this->filler) {
        SPI_put_wait(0x00, this->EPD_Pin_BUSY);
    }

    // CS high
    digitalWrite(this->EPD_Pin_EPD_CS, HIGH);

    // output data to panel
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x70, 0x02), 2);
    Delay_us(10);
    SPI_send(this->EPD_Pin_EPD_CS, CU8(0x72, 0x2f), 2);
    SPI_off();
}

#endif

static void SPI_on() {
    SPI.end();
    SPI.begin();
    //SPI.setBitOrder(MSBFIRST);
    //SPI.setDataMode(SPI_MODE2);
    //SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI_put(0x00);
    SPI_put(0x00);
    Delay_us(10);
}

static void SPI_off() {
    // SPI.begin();
    // SPI.setBitOrder(MSBFIRST);
    // SPI.setDataMode(SPI_MODE0);
    // SPI.setClockDivider(SPI_CLOCK_DIV2);
    #if 0
    SPI_put(0x00);
    SPI_put(0x00);
    Delay_us(10);
    SPI.end();
    #else
    SPI_off_final();
    #endif

}

static void SPI_off_final() {

    SPI_put(0x00);
    SPI_put(0x00);
    Delay_us(10);
    SPI.end();

    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, OUTPUT);
    digitalWrite(SCK, LOW);
    digitalWrite(MOSI, LOW);
    digitalWrite(MISO, LOW);
}

static void SPI_put(uint8_t c) {
    SPI.transfer(c);
}


static void SPI_put_wait(uint8_t c, int busy_pin) {

    SPI_put(c);

    // wait for COG ready
    while (HIGH == digitalRead(busy_pin)) {
    }

}


static void SPI_send(uint8_t cs_pin, const uint8_t* buffer, uint16_t length) {
    // CS low
    digitalWrite(cs_pin, LOW);

    // send all data
    for (uint16_t i = 0; i < length; ++i) {
        SPI_put(*buffer++);
    }

    // CS high
    digitalWrite(cs_pin, HIGH);
}


static void PWM_start(int pin) {
    analogWrite(pin, 128);  // 50% duty cycle
}


static void PWM_stop(int pin) {
    analogWrite(pin, 0);
}


EPD_Class EPD;

