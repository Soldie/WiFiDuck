/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "ducktest.h"

#include "config.h"
#include "debug.h"

#include "i2c.h"

#define RESPONSE_OK 0x00
#define RESPONSE_PROCESSING 0x01
#define RESPONSE_REPEAT 0x02
#define RESPONSE_I2C_ERROR 0xFF

namespace ducktest {
    // ===== PRIVATE ===== //
    char * prevMessage    { NULL };
    size_t prevMessageLen { 0 };

    bool ran { false };

    void wait() {
        if (!i2c::connected()) return;

        i2c::sendRequest();

        while (((i2c::getResponse()) & (0x01)) == (RESPONSE_PROCESSING)) {
            delay(i2c::getResponse());
            debug('.');
            i2c::sendRequest();
        }
    }

    void transmit(const char* str, size_t len) {
        if (!i2c::connected()) return;

        size_t   buf_i { 0 };
        uint8_t* buf = (uint8_t*)str;

        while (len > 0) {
            if (len > BUFFER_SIZE) {
                i2c::transmit(&buf[buf_i], BUFFER_SIZE);
                buf_i += BUFFER_SIZE;
                len   -= BUFFER_SIZE;
            } else {
                i2c::transmit(&buf[buf_i], len);
                buf_i += len;
                len    = 0;
            }
            wait();
        }
    }

    void sendMessage(const char* msg) {
        debug("Sending message: ");
        // debug(msg);

        size_t msg_len { strlen(msg) };

        transmit(msg, msg_len);

        // If transmission was succesful, save message
        if (i2c::getStatus() == i2c::status::OK) {
            if (prevMessage) free(prevMessage);
            prevMessageLen = msg_len;
            prevMessage    = (char*)malloc(prevMessageLen + 1);
            strcpy(prevMessage, msg);
        }

        // Repeat last message as long as requested
        while (i2c::getStatus() == i2c::status::REPEAT) {
            transmit(prevMessage, prevMessageLen);
        }

        debugln("Done");
    }

    // ===== PUBLIC ===== //
    void run() {
        if (ran) return;

#ifdef ENABLE_DEFAULT_DELAY
        sendMessage("DEFAULTDELAY 200\n");
#endif // ifdef ENABLE_DEFAULT_DELAY

#ifdef LED_TEST
        sendMessage("LED 0 100 0\n");
        sendMessage("LED 255 0 0\n");
        sendMessage("DELAY 1000\n");
        sendMessage("LED 0 255 0\n");
        sendMessage("DELAY 1000\n");
        sendMessage("LED 0 0 255\n");
#endif // ifdef ENABLE_DEFAULT_DELAY

#ifdef OPEN_NOTEPAD
        sendMessage("GUI r\n");
        sendMessage("STRING notepad\n");
        sendMessage("ENTER\n");
#endif // ifdef OPEN_NOTEPAD

#ifdef HELLO_WORLD
        sendMessage("STRING Hello World! \n");
        sendMessage("ENTER\n");
#endif // ifdef HELLO_WORLD

#ifdef DELAY_TEST
        sendMessage("DELAY 1000\n");
        sendMessage(".\n");
        sendMessage("DELAY 3000\n");
        sendMessage(".\n");
        sendMessage("DELAY 5000\n");
        sendMessage(".\n");
        sendMessage("ENTER\n");
#endif // ifdef HELLO_WORLD

#ifdef REPEAT_TEST
        sendMessage("STRING Hello World! \n");
        sendMessage("REPEAT 2\n");
        sendMessage("ENTER\n");
#endif // ifdef REPEAT_TEST

#ifdef US_CHAR_TEST
        sendMessage("LOCALE US\n");
        sendMessage("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~`\n");
        sendMessage("ENTER\n");
#endif // ifdef ALL_CHAR_TEST

#ifdef DE_CHAR_TEST
        sendMessage("LOCALE DE\n");
        sendMessage("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~²³äöüÄÖÜß€°§` \n");
        sendMessage("ENTER\n");
#endif // ifdef ALL_CHAR_TEST

#ifdef GB_CHAR_TEST
        sendMessage("LOCALE GB\n");
        sendMessage("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~£¦¬éíúóÉÍÚÓ€\n");
        sendMessage("ENTER\n");
#endif // ifdef ALL_CHAR_TEST

#ifdef OVERFLOW_TEST
        sendMessage("STRING 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 \n");
        sendMessage("ENTER\n");
#endif // ifdef OVERFLOW_TEST

#ifdef KEYCODE_TEST
        sendMessage("A\n");
        sendMessage("KEYCODE 0x02 0x04\n");
        sendMessage("KEYCODE 2 4\n");
#endif // ifdef ALL_CHAR_TEST

#ifdef CLOSE_NOTEPAD
        sendMessage("DELAY 5000\n");
        sendMessage("ALT F4\n");
        sendMessage("RIGHT\n");
        sendMessage("ENTER\n");
#endif // ifdef CLOSE_NOTEPAD

        ran = true;
    }
}