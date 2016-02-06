/* Matasano Crypto Challenge (set #1, challenge #3) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Prototypes. */
int score(char *str);
void hex2bin(char *str, char *dst);
void str_xor(char *str1, char *str2, char *dst);

int main (int argc, char *argv[])
{
	// Ciphertext
	char *cipher = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	size_t cipherlen = strlen(cipher);
	size_t ciphersize = cipherlen / 2;

	// High score buffers
	int highscore = 0;
	char keywin = 0;
	char *winner = malloc(ciphersize + 1);

	// XOR buffers
	char *cipherbin = malloc(ciphersize + 1);
	char *key = malloc(ciphersize + 1);
	char *plain = malloc(ciphersize + 1);

	// Null terminate key buffer
	key[ciphersize] = '\0';

	// Convert ciphertext to binary
	hex2bin(cipher, cipherbin);

	// For all possible bytes (excluding 0)
	for (int n = 1; n < 256; n++)
	{
		// Fill key buffer with byte
		for (int j = 0; j < ciphersize; j++)
			key[j] = (char)n;

		// XOR ciphertext with the key
		str_xor(cipherbin, key, plain);

		// Score this potential plaintext
		int keyscore = score(plain);

		// Set new records
		if (keyscore > highscore)
		{
			highscore = keyscore;
			keywin = (char)n;
			strcpy(winner, plain);
		}
	}

	// Output highest scoring plaintext/key combo
	printf ("Key: %c\n", keywin);
	printf ("Score: %d (%g%%)\n", highscore, ((double)highscore / (double)ciphersize) * 100.0);
	printf ("Plaintext: %s\n", winner);

	// Free memory
	free(winner);
	free(key);
	free(plain);

	return 0;
}

/* score(string) - Returns the 'score' of a string based on English letter frequency. */
int score(char *str)
{
	int i, j;								// Iterators
	int score = 0;							// Score counter
	static char top[] = " etaoinshrdlu";	// Most frequent characters in English

	// Iterator limits
	size_t toplen = strlen(top);
	size_t len = strlen(str);

	// Increment score for each matching character
	for (i = 0; i < len; i++)
		for (j = 0; j < toplen; j++)
			score += (top[j] == str[i]) & 1;

	return score;
	//return (float)score / (float)len * 100.f;     // Normalized version
}

/* hex2bin(str, dst) - Converts a hex string to binary and stores the result in dst. */
void hex2bin(char *str, char *dst)
{
	int i, j;
	int bytecount = 0;
	int len = strlen (str);
	char nybs[3] = "00";

	// Die if length is not a multiple of 2
	if (len % 2 != 0) {
		fprintf (stderr, "Error - hex2bin() in %s at line %d: string length must be a multiple of two.\n",
																					  __FILE__, __LINE__);
		exit(1);
	}

	// Convert hex to binary
	for (i = 0; i < len; i += 2)
	{
		// Extract nybble 2-tuple
		for (j = 0; j < 2; j++)
			nybs[j] = str[i+j];

		// Convert nybbles to byte
		dst[bytecount++] = strtol (nybs, NULL, 16);
	}

	// Ensure dst is null-terminated
	dst[len/2] = '\0';
}

/* str_xor(str1, str2, dst) - XORs two strings and stores the result in dst */
void str_xor(char *str1, char *str2, char *dst)
{
	int i, j;
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);

	// Die if string lengths dont match
	if (len1 != len2)
	{
		fprintf(stderr, "Error - str_xor() in %s at line %d: strings are not of equal length.\n",
																			__FILE__, __LINE__);
		exit(1);
	}

	// Xor each byte
	for (i = 0; i < len1; i++)
		dst[i] = str1[i] ^ str2[i];

	// Ensure dst is null terminated
	dst[len1] = '\0';
}
