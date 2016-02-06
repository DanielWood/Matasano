#include <stdio.h>
#include "../matasano.h"

const char *str = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

int main(void)
{
    char buf[128], buf64[128];

    printf("base64_encode(hex_to_bin(str)) = \"%s\"\n", base64_encode(buf64, hex_to_bin(buf, str)));  
    return 0;
}
