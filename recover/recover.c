#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Show correct use of the program if necessary.
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Declare the file name and open it, displaying an error if it cannot be opened.
    char *inputName = argv[1];
    FILE *input = fopen(inputName, "r");
    if (input == NULL)
    {
        printf("Could not open the file %s.\n", inputName);
        return 1;
    }

    // Declare an array to store the block of bytes; a bool to check if it is a JPEG and a counter for files created.
    BYTE block[BLOCK_SIZE];
    bool probablyJPEG = false;
    int outputNumber = 0; // O count.

    // Declare the output variable so you can close the file after finding another jpeg.
    FILE *output = NULL;

    // Declare a variable to verify whether the output file is still open or not.
    bool outputIsOpen = false;

    while (fread(block, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        // If the block is empty and we have already read a file, it ends the loop and closes the file.
        bool emptyBlock = true;
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            if (block[i] != 0)
            {
                emptyBlock = false;
                break;
            }
        }
        if (emptyBlock && outputNumber > 0)
        {
            fclose(output);
            outputIsOpen = false;
            break;
        }

        // If the block starts with the characteristic bytes of a jpeg: closes the previous file - if it exists;
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0)
        {
            if (outputNumber > 0)
            {
                fclose(output);
            }

            probablyJPEG = true;

            char outputName[8]; // Character array in which the file name will be stored ex: 000.txt.
            sprintf(outputName, "%03i.jpg", outputNumber);

            output = fopen(outputName, "w");
            if (output == NULL)
            {
                printf("Could not create the file %s.\n", outputName);
                return 1;
            }
            outputIsOpen = true;
            outputNumber++;
        }

        // As this process is a loop, it writes blocks to the output until it finds another likely jpeg or finds an empty block.
        if (probablyJPEG)
        {
            fwrite(block, 1, BLOCK_SIZE, output);
        }
    }

    // If the output file is still open, close it.
    if (outputIsOpen)
    {
        fclose(output);
    }

    // Fecha o arquivo input.
    fclose(input);
    return 0;
}