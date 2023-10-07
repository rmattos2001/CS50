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
        // Caso o bloco seja vazio e já tenhamos lido algum arquivo, finaliza o loop e fecha o arquivo.
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

        // Se o bloco começar com os bytes característicos de um jpeg: fecha o arquivo anterior - caso exista;
        // Marca que provavelmente é um novo jpeg; cria um novo arquivo e aumenta o contador.
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0)
        {
            if (outputNumber > 0)
            {
                fclose(output);
            }

            probablyJPEG = true;

            char outputName[8]; // Array de caracteres na qual será armazenado o nome do arquivo ex: 000.txt.
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

        // Escreve no arquivo criado um bloco do provável jpeg;
        // Como esse processo é um loop, vai escrevendo os blocos no output até achar outro provável jpeg ou achar um bloco vazio.
        if (probablyJPEG)
        {
            fwrite(block, 1, BLOCK_SIZE, output);
        }
    }

    // Se o arquivo output ainda estiver aberto, o fecha.
    if (outputIsOpen)
    {
        fclose(output);
    }

    // Fecha o arquivo input.
    fclose(input);
    return 0;
}