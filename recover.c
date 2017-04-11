/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>

int main(void)
{
    // open input file
    FILE * raw_file = fopen("card.raw", "r");
    if (raw_file == NULL)
    {
        return 1;
    }
    //block of 512 bytes
    typedef uint8_t byte;
    
    byte buffer[512];
    
    int n = 0;
    int found = 0;
     FILE *img = NULL;
     while (fread(&buffer,sizeof(byte),512,raw_file) == 512)
    {
        char filename [10];
        if( (buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        { 
            sprintf(filename,"%03i.jpg",n);
            n++;    
            // if jpeg is open, close it
            if (img != NULL)
            {
                if( found ==1 )
                {
                    fclose(img);
                    img = NULL;
                }
            }
            //open new file
            img = fopen(filename,"w");
            //true, found a JPEG, make a new jpeg file
            found = 1;
        }
       //if found new image, write it
       if(found == 1)
       {
           fwrite(&buffer,sizeof(byte),512,img);
       }
    }
    //if found img, the img is not null, close it
    if (found == 1 )
    {
        fclose(img);
        img = NULL;
    }
    fclose(raw_file);
    return 0;
}
