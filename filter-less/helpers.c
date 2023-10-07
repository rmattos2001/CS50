#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Sums the pixel's RGB values, averages this sum, and rounds it
            int sum = image[y][x].rgbtBlue + image[y][x].rgbtGreen + image[y][x].rgbtRed;
            int average = round(sum / 3.0);

            // Change RGB pixel values to be equal to average
            image[y][x].rgbtBlue = average;
            image[y][x].rgbtGreen = average;
            image[y][x].rgbtRed = average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Declare rgb variables for the pixel
            int originalBlue = image[y][x].rgbtBlue;
            int originalGreen = image[y][x].rgbtGreen;
            int originalRed = image[y][x].rgbtRed;

            // Calculates sepia pixel versions.
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);

            // Limita a versão sépia a um máximo de 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // Muda os valores rgb do pixel para suas versões sépia
            image[y][x].rgbtBlue = sepiaBlue;
            image[y][x].rgbtGreen = sepiaGreen;
            image[y][x].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
