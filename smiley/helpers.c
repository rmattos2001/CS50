#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    // By checking every line column of the image (all pixels)
    for (int i = 0; i < height; i++)
    {
        for (int column = 0; column < width; column++)
        {
            // If the values for red, green and blue, in the pixel (image[line[column]])
            // are all '0', than the pixel is black and we should change its color
            if (image[i][column].rgbtBlue == 0
                && image[i][column].rgbtGreen == 0
                && image[i][column].rgbtRed == 0)
            {
                image[i][column].rgbtBlue = 66;
                image[i][column].rgbtGreen = 94;
                image[i][column].rgbtRed = 133;
            }
        }
    }
}
