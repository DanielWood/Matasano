#include <stdio.h>
#include "matasano.h"

int main(void)
{
    char encode[128];
    char decode[128];
    char *src = "Cooking MC's like a pound of Phizo (~:";

    printf("base64_encode(\"%s\") = \"%s\"\n", src, base64_encode(encode, src));
    printf("base64_decode(\"%s\") = \"%s\"\n\n", encode, base64_decode(decode, encode));
    
    return 0;
}
