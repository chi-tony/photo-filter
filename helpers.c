#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through rows and columns of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average of the RGB values and average them
            float average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Assign the average to each RGB value
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through rows and columns of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate sepia values
            int sepiaRed = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);

            // Define maximum values; if exceeded, assign maximum
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

            // Assign sepia RGB values to each pixel
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize shift distance for each reflected pixel in each row
    int shift = width - 1;

    // Initialize temporary integer for switching RGB values
    int tmp;

    // Loop through rows and columns of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Reset shift if at new row
            if (shift <= 0)
            {
                shift = width - 1;
            }

            // Switch RGB values from left side and right side
            tmp = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][j + shift].rgbtRed;
            image[i][j + shift].rgbtRed = tmp;

            tmp = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][j + shift].rgbtGreen;
            image[i][j + shift].rgbtGreen = tmp;

            tmp = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][j + shift].rgbtBlue;
            image[i][j + shift].rgbtBlue = tmp;

            // Increment shift distance downwards
            shift -= 2;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Define image copy and copy each pixel from original image
    RGBTRIPLE copied_image[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copied_image[i][j] = image[i][j];
        }
    }

    // Initialize adjacent cell variables
    RGBTRIPLE mid_mid = image[0][0];
    RGBTRIPLE bot_mid = image[0][0];
    RGBTRIPLE bot_right = image[0][0];
    RGBTRIPLE bot_left = image[0][0];
    RGBTRIPLE top_mid = image[0][0];
    RGBTRIPLE top_right = image[0][0];
    RGBTRIPLE top_left = image[0][0];
    RGBTRIPLE mid_right = image[0][0];
    RGBTRIPLE mid_left = image[0][0];

    // Initialize number of RGB values to average by
    int sum_red = 0;
    int sum_green = 0;
    int sum_blue = 0;
    float count = 1.0;
    float average_red = 0;
    float average_green = 0;
    float average_blue = 0;

    // Loop through rows and columns of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reset count value
            count = 1.0;

            // Define middle pixel, which will always be used in RGB calculations
            mid_mid = copied_image[i][j];

            // Add RGB values to respective running total
            sum_red = mid_mid.rgbtRed;
            sum_green = mid_mid.rgbtGreen;
            sum_blue = mid_mid.rgbtBlue;

            // Check for pixel directly below middle
            if (i + 1 < height)
            {
                bot_mid = copied_image[i + 1][j];

                // Add RGB values to running total
                sum_red += bot_mid.rgbtRed;
                sum_green += bot_mid.rgbtGreen;
                sum_blue += bot_mid.rgbtBlue;

                // Increment count of pixels to average by
                count += 1.0;

                // If bottom middle pixel exists, check for bottom right pixel
                if (j + 1 < width)
                {
                    bot_right = copied_image[i + 1][j + 1];

                    // Add RGB values to respective running totals
                    sum_red += bot_right.rgbtRed;
                    sum_green += bot_right.rgbtGreen;
                    sum_blue += bot_right.rgbtBlue;

                    count += 1.0;
                }

                // If bottom middle pixel exists, check for bottom left pixel
                if (j - 1 >= 0)
                {
                    bot_left = copied_image[i + 1][j - 1];

                    // Add RGB values to respective running totals
                    sum_red += bot_left.rgbtRed;
                    sum_green += bot_left.rgbtGreen;
                    sum_blue += bot_left.rgbtBlue;

                    count += 1.0;
                }
            }

            // Check for pixel directly above middle
            if (i - 1 >= 0)
            {
                top_mid = copied_image[i - 1][j];

                // Add RGB values to respective running totals
                sum_red += top_mid.rgbtRed;
                sum_green += top_mid.rgbtGreen;
                sum_blue += top_mid.rgbtBlue;

                count += 1.0;

                // If top middle pixel exists, check for top right pixel
                if (j + 1 < width)
                {
                    top_right = copied_image[i - 1][j + 1];

                    // Add RGB values to respective running totals
                    sum_red += top_right.rgbtRed;
                    sum_green += top_right.rgbtGreen;
                    sum_blue += top_right.rgbtBlue;

                    count += 1.0;
                }

                // If top middle pixel exists, check for top left pixel
                if (j - 1 >= 0)
                {
                    top_left = copied_image[i - 1][j - 1];

                    // Add RGB values to respective running totals
                    sum_red += top_left.rgbtRed;
                    sum_green += top_left.rgbtGreen;
                    sum_blue += top_left.rgbtBlue;

                    count += 1.0;
                }
            }

            // Check for pixel directly to the right of middle
            if (j + 1 < width)
            {
                mid_right = copied_image[i][j + 1];

                // Add RGB values to respective running totals
                sum_red += mid_right.rgbtRed;
                sum_green += mid_right.rgbtGreen;
                sum_blue += mid_right.rgbtBlue;

                count += 1.0;
            }

            // Check for pixel directly to the left of middle
            if (j - 1 >= 0)
            {
                mid_left = copied_image[i][j - 1];

                // Add RGB values to respective running totals
                sum_red += mid_left.rgbtRed;
                sum_green += mid_left.rgbtGreen;
                sum_blue += mid_left.rgbtBlue;

                count += 1.0;
            }

            // Calculate the average RGB values of that pixel
            average_red = round(sum_red / count);
            average_green = round(sum_green / count);
            average_blue = round(sum_blue / count);

            // Assign the RGB values of the copied pixel to the original image
            image[i][j].rgbtRed = average_red;
            image[i][j].rgbtGreen = average_green;
            image[i][j].rgbtBlue = average_blue;
        }
    }
    return;
}
