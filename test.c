#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "matasano.h"

int main(void)
{
    char *b64plain = "base64 test";
    char *b64encoded = "YmFzZTY0IHRlc3QA=";

    char *hexplain = "hex test";
    char *hexencoded = "6865782074657374";

    char *xorplain = "xor test";
    char *xorkey = "ABCDEFGH";

    char *hanning_a = "this is a test";
    char *hanning_b = "wokka wokka!!!";

    char buf[128];
    
    // Base64 unit test
    assert(strcmp(b64encoded, base64_encode(buf, b64plain)) == 0);
    assert(strcmp(b64plain, base64_decode(buf, b64encoded)) == 0);

    // Hex unit test
    assert(strcmp(hexencoded, bin_to_hex(buf, hexplain, strlen(hexplain))) == 0);
    assert(strcmp(hexplain, hex_to_bin(buf, hexencoded)) == 0);

    // XOR unit test
    size_t xorlen = strlen(xorplain);
    assert(strcmp(xorplain, str_xor(buf, str_xor(buf, xorplain, xorkey, xorlen), xorkey, xorlen)) == 0);

    // Hanning distance test
    assert(edit_dist(hanning_a, hanning_b) == 37);

    printf("All tests completed successfully!\n"); 
    return 0;
}
