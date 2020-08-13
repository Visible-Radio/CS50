#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    //check usage
    if (argc !=2)
    {
        return 1;
    }

    //Open file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        return 1;
    }

    long byte_count=0;
    while (fgetc(file) != EOF)
    {
      byte_count++;

    }

    printf("byte count: %li\n", byte_count);
    rewind(file);


    unsigned char arr[512];
    int jpeg_count = 0;
    char *filename = NULL;
    bool write_enable = false;
    FILE *img = NULL;

    for (long i=0; i < byte_count; i+=512)
    {
        fread(arr,512, 1, file);

        if (arr[0] == 0xff && arr[1] == 0xd8 && arr[2] == 0xff && (arr[3] & 0xe0) == 0xe0)
        {
            // we've found a new jpeg header

            printf("Found a jpeg header at byte: %li \n", ftell(file));
                if (jpeg_count > 0)
                {
                    //close the current file being written
                    fclose(img);

                }

                //create a new filename string
                //open a new file
                filename = malloc(sizeof(char)*8);
                sprintf(filename, "%03i.jpg", jpeg_count);
                img = fopen(filename, "w");
                write_enable=true;

                jpeg_count++;

        }

        if (write_enable==true)
        {
        fwrite(arr,512,1,img);

        }





    }


printf("JPEGs found: %i \n", jpeg_count);
free(filename);
}//END OF PROGRAM


  /*
    fread(data, size, number, inptr)

    where data a pointer to the buffer into which the file will be read
    size of each element in bytes to be read from the file
    how many elements to be read
    pointer to file from which to read

    jpeg header begins with 0xff 0xd8 0xff 0xe_
    look for the the first three bytes

    if 4th byte & 1110 0000 == 1110 0000


    1110 0000   examined btye
    1110 0000   bitwise comparator
    =========
    1110 0000

    1110 0000
    1110 0001
    1110 0010
    1110 0011
    1110 0100
    1110 0101
    1110 0110
    1110 0111
    etc etc etc

    */

    //Read some of the file and print it to the terminal
    // formatted 16 bytes wide, printing an extra \n at the begining of each 512 byte block
    /*
    unsigned char arr[16384];
    fread(arr,1, 16384, file);
    int block_count=0;

    for (int i=0; i < 16384; i+=16)
    {
        if (block_count == 512)
        {
            printf("\n");
            block_count = 0;
        }

        printf("byte location: %00004i ", i);
        for (int j=0; j<16; j++)
        {

            printf("%02x ", arr[i+j]);



        }
        block_count +=16;
        printf("\n");
    }
    */