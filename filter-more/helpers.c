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
            // Set variables for the sum of each RGB value, counter and blur radius
            int blueSum = 0, greenSum = 0, redSum = 0, c = 0;
            int r = 1;

            // Checks each pixel and adds their RGB values based on radius
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

            // Take the middle of the shape, according to how many pixels are shaped and rounded.
            int averageBlue = round(blueSum * 1.0 / c);
            int averageGreen = round(greenSum * 1.0 / c);
            int averageRed = round(redSum * 1.0 / c);

            // Changes the RGB values of the pixel to the average RGB of the pixels surrounding it
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
    // Copia a imagem pois se não, ao desfocar um pixel, isso vai afetar o efeito no próximo
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

    // Repete o processo para todos os pixeis
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Define variáveis para o Gx e Gy de cada valor rgb, dois contadores e os fatores do algoritmo
            int blueGx = 0, greenGx = 0, redGx = 0, blueGy = 0, greenGy = 0, redGy = 0;
            int cX = 0, cY = 0;
            int factors[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

            // Verifica os pixeis em vorta do pixel principal e soma o produto de seus valores pelo fator
            for (int row = y - 1; row <= y + 1; row++)
            {
                for (int column = x - 1; column <= x + 1; column++)
                {
                    // Ignore 'pixels that would be outside the border'
                    if ((row >= 0 && column >= 0) && (row <= height - 1 && column <= width - 1))
                    {
                        // Multiply the value by the factor.
                        blueGx += (copy[row][column].rgbtBlue * factors[cY][cX]);
                        greenGx += (copy[row][column].rgbtGreen * factors[cY][cX]);
                        redGx += (copy[row][column].rgbtRed * factors[cY][cX]);

                        blueGy += (copy[row][column].rgbtBlue * factors[cX][cY]);
                        greenGy += (copy[row][column].rgbtGreen * factors[cX][cY]);
                        redGy += (copy[row][column].rgbtRed * factors[cX][cY]);
                    }
                    cX++;
                }
                cY++;
                cX = 0;
            }

            // Sets and rounds RGB values according to the 'Sobel Filter' algorithm,
            int blue = round(sqrt(blueGx * blueGx + blueGy * blueGy));
            int green = round(sqrt(greenGx * greenGx + greenGy * greenGy));
            int red = round(sqrt(redGx * redGx + redGy * redGy));

            // Limit values to a maximum of 255
            if (blue > 255)
            {
                blue = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (red > 255)
            {
                red = 255;
            }

            // Changes the RGB values of the pixel to values found in the 'Sobel Filter' algorithm above
            image[y][x].rgbtBlue = blue;
            image[y][x].rgbtGreen = green;
            image[y][x].rgbtRed = red;
        }
    }
    return;
}
