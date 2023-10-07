// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t tempHeader;

    // TODO: Read samples from input file and write updated data to output file
    // Will copy the header, based on its size given by HEADER_SIZE
    for (int i = 0; i < HEADER_SIZE; i++)
    {
        fread(&tempHeader, sizeof(uint8_t), 1, input);
        fwrite(&tempHeader, sizeof(uint8_t), 1, output);
    }

    // TODO: Read samples from input file and write updated data to output file
    // Declare a temporary variable to store two bytes of the sample.
    int16_t tempSample;

    // It will read from the end of the header to the end of the file, copying the samples and multiplying them by the factor
    // entered by the user.
    while (fread(&tempSample, sizeof(int16_t), 1, input))
    {
        tempSample *= factor;
        fwrite(&tempSample, sizeof(uint16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
