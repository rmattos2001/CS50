#include <cs50.h>
// index = 0.0588 * L - 0.296 * S - 15.8

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void) // int argc, string argv[]
{
    // get user to analyze
    string string = get_string("What is the string to analyze? \n");

    int letterCount = 0, wordCount = 0, sentenceCount = 0;

    // count letters, words, and sentences
    // assume a..z and A..Z is a letter
    for (int i = 0, stringLength = strlen(string); i < stringLength + 1; i++)
    {
        if ((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z'))
        {
            letterCount++;
        }
        // any chars seperated by space is a word, check end of string count last word
        if (string[i] == ' ' || string[i] == '\0')
        {
            wordCount++;

        }
        // when you see a . ! or ? count as sentence
        if (string[i] == '!' || string[i] == '.'|| string[i] == '?')
        {
            sentenceCount++;
        }

    }

    // calculate the grade reading index
    float averageWordsPer100 = (100 / (float) wordCount) * (float) letterCount;
    float averageSentencePer100 = (100 / (float) wordCount) * (float) sentenceCount;
    int readingIndex = round(0.0588 * averageWordsPer100 - 0.296 * averageSentencePer100 - 15.8);

    if (readingIndex < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (readingIndex > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", readingIndex);
    }

    return 0;
}