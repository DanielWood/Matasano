#ifndef __MATASANO_H__
#define __MATASANO_H__

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include <err.h>

// Base-64 encode a string
char *base64_encode(char *dest, const char *src)
{
    assert(src != dest);

    static const char charset[64 + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
                                        "ghijklmnopqrstuvwxyz0123456789+/";

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
// Possible to let src == dest?
char *base64_decode(char *dest, const char *src)
{
    assert(src != dest);

    static const char charset[64 + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
                                        "ghijklmnopqrstuvwxyz0123456789+/";

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
            if (index == NULL)
                err(1, "base64_decode: Invalid character '%c' (%d)", c, c);
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

// Convert a hex string to binary    
char *hex_to_bin(char *dest, const char *src)
{
    //assert(src != dest);
 
    // Strip prefix
    while ((*src == 'x' && *(src + 1)) || *(src + 1) == 'x')
        src++;

    size_t len = strlen(src);
    if (len % 2 != 0)
        err(1, "hex_to_bin: Source length (%d) must be a multiple of two", len);

    int j = 0;              // Dest increment
    char buf[3] = "\0\0";   // Nybble buffer
    
    // Decode hex
    for (int i = 0, len = strlen(src); i < len;)
    {
        char c = src[i];

        if (!isxdigit(c))
            err(1, "Invalid hex character '%c' (%d)", c, c);

        buf[i % 2] = c;     // Pack nybbles
        
        // Convert buf to single char every 2nd nybble
        if (++i % 2 == 0)
            dest[j++] = strtol(buf, NULL, 16);
    }
    
    dest[j] = '\0';

    return dest;
}

// Convert a binary string to hex
char *bin_to_hex(char *dest, const char *src, size_t size)
{
    assert(src != dest);

    // Convert to bytes nybble string
    for (int i = 0, j = 0; i < size; i++, j += 2)
        snprintf(&dest[j], 2 + 1, "%02x", src[i]);

    return dest;
}

// XOR's the first <size> bytes of a and b, storing the result in dest
char *str_xor(char *dest, const char *a, const char *b, size_t size)
{
    for (int i = 0; i < size; i++) 
       dest[i] = a[i] ^ b[i];

    return dest;
}

#endif
