#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc > 2 || argc < 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int k = atoi(argv[1]) % 26;

    if (k <= 0)
    {
        printf("Usage ./caesar key\n");
        return 1;
    }

    string plaintext = get_string("plaintext:  ");

    int lenText = strlen(plaintext);

    char ciphertext[lenText];

    for (int i = 0; i < lenText; i++)
    {
        k = atoi(argv[1]) % 26;

        if (isalpha(plaintext[i]))
        {
            if (islower(plaintext[i]))
            {
                if (plaintext[i] + k > 122)
                {
                    k = k - 26;
                }
            }

            else
            {
                if (plaintext[i] + k > 90)
                {
                    k = k - 26;
                }
            }

            ciphertext[i] = plaintext[i] + k;
        }

        else
        {
            ciphertext[i] = plaintext[i];
        }
    }

    printf("ciphertext: %s\n", ciphertext);
}