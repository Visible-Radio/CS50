#include "helpers.h"
#include "math.h"
/*
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
*/

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
// iterate through rows with i
    // iterate through columns with j
        // for each pixel in row, take the average of the colour channels
        // set each colour channel to this average

    float average;

    for (int i=0; i < height; i++)
    {
        for ( int j=0; j < width; j++)
        {
            average = round(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed)/3);
            //average = round(average);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }


    return;
}
























// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through rows with i
        // iterate through columns with j
            // copy pixel at [i][j] to [i][width-j]

    RGBTRIPLE swapHolder;

    for (int i =0; i < height; i++)
    {
        for (int j=0; j < width/2; j++)
        {
            swapHolder = image[i][j];
            image[i][j] = image[i][width-j-1];
            image[i][width-j-1] = swapHolder;

            /*
            this code can all be replaced with the three lines above

            swapHolder.rgbtBlue = image[i][j].rgbtBlue;
            swapHolder.rgbtGreen = image[i][j].rgbtGreen;
            swapHolder.rgbtRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width-j-1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width-j-1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width-j-1].rgbtRed;

            image[i][width-j-1].rgbtBlue = swapHolder.rgbtBlue;
            image[i][width-j-1].rgbtGreen = swapHolder.rgbtGreen;
            image[i][width-j-1].rgbtRed = swapHolder.rgbtRed;
            */


        }
    }

    return;
}















// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make a copy of image for reference

    RGBTRIPLE image_copy[height][width];

    for (int i =0; i < height; i ++)
    {
        for (int j=0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
        }
    }


    // iterate through height with i
        // iterate through width with j
            // for each pixel, find the average of it with the neighbouring 9 pixels
            // assign this average to the pixel

            // i-1
                // j-1, j, j + 1

            // i
                // j-1, j, j + 1

            // i+1
                // j-1, j, j + 1

        for (int i =0; i < height; i ++)
        {
            for (int j=0; j < width; j++)
            {

                 /* if (i > 0 && i < height -1 && j > 0 && j < width -1)

                    // quick and dirty, don't bother with edges or corners, just the meat of the image

                    // default settings for pixels that aren't edges or corners
                */

                    int row_offset = -1;
                    int column_offset = -1;
                    int row_limit = 1;
                    int column_limit = 1;






                // set bounds for edge cases

                if (i==0 && j > 0 && j < width-1)
                    // top row meat
                {
                    row_offset = 0;
                    row_limit = 1;
                    column_offset = -1;
                    column_limit = 1;
                }


                else if (i==height-1 && j > 0 && j < width-1)
                {
                    // bottom row meat
                    row_offset = -1;
                    row_limit = 0;
                    column_offset = -1;
                    column_limit = 1;
                }

                else if (j==0 && i != 0 && i != height-1)
                    // left side meat
                {
                    column_offset = 0;
                    column_limit = 1;
                    row_offset = -1;
                    row_limit = 1;
                }

                else if (j==width-1 && i != 0 && i != height-1)
                {
                    // right side meat
                    column_offset = -1;
                    column_limit = 0;
                    row_offset = -1;
                    row_limit = 1;
                }

                else if (i==0 && j==0)
                {
                    // top left corner
                    column_offset = 0;
                    column_limit = 1;
                    row_offset = 0;
                    row_limit = 1;
                }

                else if (i==0 && j==width-1)
                {
                    // top right corner
                    column_offset = -1;
                    column_limit = 0;
                    row_offset = 0;
                    row_limit = 1;
                }

                else if (i==height-1 && j==0)
                {
                    // bottom left corner
                    column_offset = 0;
                    column_limit = 1;
                    row_offset = -1;
                    row_limit = 0;
                }

                else if (i==height-1 && j==width-1)
                {
                    // bottom right corner
                    column_offset = -1;
                    column_limit = 0;
                    row_offset = -1;
                    row_limit = 0;
                }


                float blue =0;
                float green =0;
                float red =0;
                int neighbor_count = 0;

                for (int k=row_offset; k <= row_limit; k++)
                {
                    for (int l=column_offset; l <= column_limit; l++)
                    {
                        blue += image_copy[i+k][j+l].rgbtBlue;
                        green += image_copy[i+k][j+l].rgbtGreen;
                        red += image_copy[i+k][j+l].rgbtRed;

                        neighbor_count ++;

                    }
                }

                image[i][j].rgbtBlue = round(blue/neighbor_count);
                image[i][j].rgbtGreen = round(green/neighbor_count);
                image[i][j].rgbtRed = round(red/neighbor_count);



            }
        }


    return;
}










// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // make a copy of image for reference

    RGBTRIPLE image_copy[height][width];

    for (int i =0; i < height; i ++)
    {
        for (int j=0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
        }
    }


    // iterate through rows with i
    // iterate through columns with j
    // iterate through [i][j]'s neighboring pixels with k and l

    // if (i > 0 && i < height -1 && j > 0 && j < width -1) condition for pixels where a complete 3x3 square exists
    // for each channel compute gx and gy
    /*

    gx

    -1  0  1
    -2  0  2
    -1  0  1

    gy

    -1 -2 -1
     0  0  0
     1  2  1

    */
    // square root of the sum of the squares of gx and gy
    // write this new value at [i][j] for each channel, cap it at 255

    int gx_mults[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy_mults[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    RGBTRIPLE gx;
    RGBTRIPLE gy;

    int new_pixelBlue =0;
    int new_pixelGreen =0;
    int new_pixelRed =0;

    gx.rgbtBlue =0;
    gx.rgbtGreen =0;
    gx.rgbtRed =0;

    gx.rgbtBlue =0;
    gx.rgbtGreen =0;
    gx.rgbtRed =0;

    gx.rgbtBlue =0;
    gx.rgbtGreen =0;
    gx.rgbtRed =0;


    for (int i=0; i < height; i++)
    {
        for (int j=0; j < width; j++)
        {
            if (i > 0 && i < height -1 && j > 0 && j < width -1) // quick and dirty condition ignoring edge cases
            {

                /*  k and l provide offsets for the main cursor at [i][j]
                    so as to look at the surounding pixels in a 3x3 square
                */
                // reset all variables that are used as accumulators inside k l loops

                int mult_index = 0;

                gx.rgbtBlue =0;
                gx.rgbtGreen =0;
                gx.rgbtRed =0;

                gx.rgbtBlue =0;
                gx.rgbtGreen =0;
                gx.rgbtRed =0;

                gx.rgbtBlue =0;
                gx.rgbtGreen =0;
                gx.rgbtRed =0;

                for (int k=-1; k <= 1; k ++)
                {
                    for (int l=-1; l <= 1; l++)
                    {
                        /*  here's where we do the math for gx and gy
                            can not store the multipliers in an array that gets indexed with [k][l] because the indexes would be negative
                            use a 1d array with its own index

                        */
                        gx.rgbtBlue     += (image_copy[i+k][j+l].rgbtBlue)*gx_mults[mult_index];
                        gx.rgbtGreen    += (image_copy[i+k][j+l].rgbtGreen)*gx_mults[mult_index];
                        gx.rgbtRed      += (image_copy[i+k][j+l].rgbtRed)*gx_mults[mult_index];

                        gy.rgbtBlue     += (image_copy[i+k][j+l].rgbtBlue)*gy_mults[mult_index];
                        gy.rgbtGreen    += (image_copy[i+k][j+l].rgbtGreen)*gy_mults[mult_index];
                        gy.rgbtRed      += (image_copy[i+k][j+l].rgbtRed)*gy_mults[mult_index];

                        mult_index ++;
                    }

                }

                // combine results of gx and gy
                // cap results at 255

                new_pixelBlue = round(sqrt(pow(gx.rgbtBlue,2) + pow(gy.rgbtBlue,2)));
                new_pixelGreen = round(sqrt((gx.rgbtGreen*gx.rgbtGreen) + (gy.rgbtGreen*gy.rgbtGreen)));
                new_pixelRed = round(sqrt((gx.rgbtRed*gx.rgbtRed) + (gy.rgbtRed*gy.rgbtRed)));

                if (new_pixelBlue > 255)
                {
                    image[i][j].rgbtBlue = 255;
                }
                else
                {
                    image[i][j].rgbtBlue = new_pixelBlue;
                }

                   if (new_pixelGreen > 255)
                {
                    image[i][j].rgbtGreen = 255;
                }
                else
                {
                    image[i][j].rgbtGreen = new_pixelGreen;
                }

                   if (new_pixelRed > 255)
                {
                    image[i][j].rgbtRed = 255;
                }
                else
                {
                    image[i][j].rgbtRed = new_pixelRed;
                }



            }
        }
    }

    return;
}
