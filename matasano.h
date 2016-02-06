#ifndef __MATASANO_H__
#define __MATASANO_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//#include <err.h>
#include <assert.h>

// Base-64 encode a string
char *base64_encode(char *dest, const char *src)
{
    static const char charset[64 + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
                                        "ghijklmnopqrstuvwxyz0123456789+/";

    assert(dest != src);

    size_t len = strlen(src);
    size_t padlen = len % 3;
    len += padlen ? (3 - padlen) : 0;
    
    // Temporary padding buffer
    char *temp = malloc(len);
    strncpy(temp, src, len);    // <-- Auto pads with nullbytes up to len

    int i;      // Source increment
    int j;      // Dest increment
   
    // Encode string
    int32_t word;
    for (i = 0, j = 0; i < len; i += 3, j += 4)
    {
        word = 0;

        // Pack three bytes (24 bits) into a dword
        for (int k = 0; k < 3; k++)
        {
            int shift = (2 - k) * 8;
            word |= (int32_t)(temp[i + k]) << shift;
        }
  
        // Unpack four 6-bit ints from dword (4 * 6 = 24)
        for (int k = 0; k < 4; k++)
        {
            int shift = (3 - k) * 6;
            int index = (word >> shift) & 0x3f;
            dest[j + k] = charset[index];
        }
    }
   
    // Pad and terminate dest
    int k = 0;
    if (padlen)
        for (; k < 3 - padlen; k++)
            dest[j + k] = '=';
    dest[j + k] = '\0';

    free(temp);
    return dest;
}

// Base64-decode a string
char *base64_decode(char *dest, const char *src)
{
    static const char charset[64 + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
                                        "ghijklmnopqrstuvwxyz0123456789+/";

    assert(dest != src);

    size_t len = strlen(src);

    int i;      // Source increment
    int j;      // Dest increment

    // Decode string
    int32_t word;
    for (i = 0, j = 0; i < len; i += 4, j += 3)
    {
        word = 0;

        // Pack four 6-bit ints into dword
        for (int k = 0; k < 4; k++)
        {
            char c = src[i + k];
            if (c == '\0' || c == '=')
                break;

            // Find charset index
            char *index = strchr(charset, c);
            assert(index != NULL);
            index -= (int)charset;

            int shift = (3 - k) * 6;
            word |= ((int32_t)(index) & 0x3f) << shift;
        }

        // Unpack three bytes from dword
        for (int k = 0; k < 3; k++)
        {
            int shift = (2 - k) * 8;
            dest[j + k] = (word >> shift) & 0xff;
        }
    }

    // Terminate dest
    dest[j] = '\0';

    return dest;
}

#endif
