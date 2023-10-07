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

            // Limits the sepia version to a maximum of 255
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
    for (int y = 0; y < height; y++)
    {
        // Instead of checking all columns of pixels, we will only check half, like
        // Swap half of the pixels, the other half will also be swapped
        for (int x = 0, temp; x < width / 2; x++)
        {
            // Swap the x column pixel with the corresponding reverse column pixel
            temp = image[y][x].rgbtBlue;
            image[y][x].rgbtBlue = image[y][width - 1 - x].rgbtBlue;
            image[y][width - 1 - x].rgbtBlue = temp;

            temp = image[y][x].rgbtGreen;
            image[y][x].rgbtGreen = image[y][width - 1 - x].rgbtGreen;
            image[y][width - 1 - x].rgbtGreen = temp;

            temp = image[y][x].rgbtRed;
            image[y][x].rgbtRed = image[y][width - 1 - x].rgbtRed;
            image[y][width - 1 - x].rgbtRed = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the image because otherwise blurring one pixel will affect the effect on the next.
    RGBTRIPLE copy[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            copy[y][x].rgbtBlue = image[y][x].rgbtBlue;
            copy[y][x].rgbtGreen = image[y][x].rgbtGreen;
            copy[y][x].rgbtRed = image[y][x].rgbtRed;
        }
    }

    // Repeat the process for all pixels.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Establish variables for the sum of each RGB value, counter and blur radio
            int blueSum = 0, greenSum = 0, redSum = 0, c = 0;
            int r = 1;

            // Checks each pixel and adds their RGB values based on radius
            // Ex: Radius 1 the sum will be done for all pixels up to 1 distance from the main pixel.
            for (int row = y - r; row <= y + r; row++)
            {
                for (int column = x - r; column <= x + r; column++)
                {
                    if ((row >= 0 && column >= 0) && (row <= height - 1 && column <= width - 1))
                    {
                        blueSum += copy[row][column].rgbtBlue;
                        greenSum += copy[row][column].rgbtGreen;
                        redSum += copy[row][column].rgbtRed;
                        c++;
                    }
                }
            }

            // Take the average of the sum, based on how many pixels were added and rounding
            int averageBlue = round(blueSum * 1.0 / c);
            int averageGreen = round(greenSum * 1.0 / c);
            int averageRed = round(redSum * 1.0 / c);

            // Muda os valores rgb do pixel para a média rgb dos pixels à sua volta
            image[y][x].rgbtBlue = averageBlue;
            image[y][x].rgbtGreen = averageGreen;
            image[y][x].rgbtRed = averageRed;
        }
    }
    return;
}
