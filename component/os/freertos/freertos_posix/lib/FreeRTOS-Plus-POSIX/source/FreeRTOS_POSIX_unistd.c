/*
 * FreeRTOS POSIX V1.2.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file FreeRTOS_POSIX_unistd.c
 * @brief Implementation of functions in unistd.h
 */

/* FreeRTOS+POSIX includes. */
#include "FreeRTOS_POSIX.h"
#include "FreeRTOS_POSIX/unistd.h"

/*-----------------------------------------------------------*/

unsigned sleep( unsigned seconds )
{
    vTaskDelay( pdMS_TO_TICKS( seconds * 1000 ) );

    return 0;
}

/*-----------------------------------------------------------*/

int usleep( useconds_t usec )
{
    /* To avoid delaying for less than usec, always round up. */
    vTaskDelay( pdMS_TO_TICKS( usec / 1000 + ( usec % 1000 != 0 ) ) );

    return 0;
}

typedef struct {
    int code;
    const char *msg;
} custom_err_msg_t;

static const custom_err_msg_t custom_err_msg_table[] = {
      {-1, "custom err -1"},
      {101, "custom err 101"},
      {1001, "custom err 1001"}
};

char *strerror_r(int errnum, char *buffer, size_t n)
{
    int i;

    for (i = 0; i < sizeof(custom_err_msg_table)/sizeof(custom_err_msg_table[0]); ++i) {
        if (custom_err_msg_table[i].code == errnum) {
            strlcpy(buffer, custom_err_msg_table[i].msg, n);
            return buffer;
        }
    }
    
    return buffer;
}

void bzero(char *s, int n)
{
    memset(s, 0, n);
}

int prctl (int __option, ...)
{
   return 0;
}

/*-----------------------------------------------------------*/
