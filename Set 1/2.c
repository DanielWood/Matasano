#include <stdio.h>
#include "../matasano.h"

int main(void)
{
    char A[] = "1c0111001f010100061a024b53535009181c";
    char B[] = "686974207468652062756c6c277320657965";
    size_t size = strlen(A) / 2;
    
    char buf[128], xor[128]; 

    hex_to_bin(A, A);
    hex_to_bin(B, B);

    printf("A ^ B = \"%s\"\n", bin_to_hex(buf, str_xor(xor, A, B, size), size));
    return 0;
}
