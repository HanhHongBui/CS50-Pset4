/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    int n = atoi(argv[1]);

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
   
    BITMAPINFOHEADER bi_resize = bi;
    
    bi_resize.biWidth = bi.biWidth * n;
    bi_resize.biHeight = bi.biHeight * n;
    
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int resize_padding =  (4 - (bi_resize.biWidth* sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi_resize.biSizeImage = (bi_resize.biWidth * sizeof(RGBTRIPLE) + resize_padding) * abs( bi_resize.biHeight);
    
    BITMAPFILEHEADER bf_resize = bf;
    bf_resize.bfSize =  bi_resize.biSizeImage + 54;
    
    // write outfile's BITMAPFILEHEADER
    
    fwrite(&bf_resize, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_resize, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over infile's scanlines
    
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for ( int l = 0; l < n; l++)
        {
             //send infile cursor back
            if (l != 0)
            fseek(inptr, - bi.biWidth * 3 - padding, SEEK_CUR);
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile n times
                    for (int m = 0; m < n; m++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // skip over infile's padding, if any
            fseek(inptr, padding, SEEK_CUR);
            //write outfile's padding
            for (int k = 0; k < resize_padding; k++)
                {
                    fputc(0x00, outptr);
                }
            
            
        }    
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
