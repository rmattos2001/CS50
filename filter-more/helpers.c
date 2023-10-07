#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Soma os valores rgb do pixel, calcula a média dessa soma e a arredonda
            int sum = image[y][x].rgbtBlue + image[y][x].rgbtGreen + image[y][x].rgbtRed;
            int average = round(sum / 3.0);

            // Muda os valores rgb do pixel para serem iguais a média
            image[y][x].rgbtBlue = average;
            image[y][x].rgbtGreen = average;
            image[y][x].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int y = 0; y < height; y++)
    {
        // Instead of checking all pixel columns, we will only check the middle, like
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
    // Copy the image because otherwise, by diffusing a pixel, this will affect the effect afterwards.
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
            // Define variáveis para a soma de cada valor rgb, contador e raio do blur
            int blueSum = 0, greenSum = 0, redSum = 0, c = 0;
            int r = 1;

            // Verifica cada pixel e soma seus valores rgb de acordo com o raio
            // Ex: raio 1 a soma será feita para todas os pixeis até 1 de distância do pixel principal
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

            // Tira a média da soma, de acordo com quantos pixels foram somados e arredondando
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
