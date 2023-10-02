#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int start_size;
    do
    {
        start_size = get_int("Start size: ");
    }
    while (start_size < 9);

    // Prompt for end size
    int end_size;
    do
    {
        end_size = get_int("End size: ");
    }
    while (end_size < start_size);

    // Calculate number of years until we reach threshold
    // Big O(1)
    int year = (int) floor(log( (double)end_size/ (double)start_size) / log(13.0/12.0));


    // Print number of years
    printf("Years: %i\n", year);
}
