#include <cs50.h>
#include <stdio.h>

void build(int n);

int main(void)
{
    int height;

    // Get height
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Call build function to build the wall
    build(height);
}

void build(int n)
{
    // For each row
    for (int i = 1; i < (n + 1); i++)
    {
        // For each brick
        for (int j = 1; j < (n + 1); j++)
        {
            if (j <= (n - i))
            {
                //Print blank spaces
                printf(" ");
            }
            else
            {
                // Print the brick
                printf("#");
            }
        }

        // Go to next line
        printf("\n");
    }
}