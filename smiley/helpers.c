#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    // By checking every line column of the image (all pixels)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // i=line , j=column
            // If the values for red, green and blue, in the pixel (image[line[column]])
            if (image[i][j].rgbtBlue == 0
                && image[i][j].rgbtGreen == 0
                && image[i][j].rgbtRed == 0)
            {
                image[i][j].rgbtBlue = 66;
                image[i][j].rgbtGreen = 94;
                image[i][j].rgbtRed = 133;
            }
        }
    }
}
