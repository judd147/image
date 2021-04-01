/**
 * This program reads an image file and a flag for hiding or recovering messages. 
 * The text file name and image file name are provided using command line arguments. If the arguments are not 
 * sufficient or the file is not readable, the program will exit and provide an error message.
 * The recovered messages will output to another file.
 * 
 * @author Liyao Zhang {@literal <zhanl217@wfu.edu>}
 * @date Oct. 26, 2020
 * @assignment Lab 5
 * @course CSC 250
 **/
 
#include "get_image_args.h"
#include "ppm_read_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hide_msg(struct Image *img, int bit, char text_file_name[]);
int recover_msg(struct Image *img, int bit, char text_file_name[]);

int main(int argc, char* argv[])
{
    struct Image *img;
    char image_file_name[20];
    char text_file_name[20];
    int hide;
    int bit;
    int status_ok;
    
    status_ok = process_image_args(argc, argv, &hide, &bit, image_file_name, text_file_name);
    
    if(status_ok){
        if(hide){
            status_ok = read_image(&img, image_file_name);
            hide_msg(img, bit, text_file_name);
            write_image(image_file_name, img);
        }
        else{
            status_ok = read_image(&img, image_file_name);
            recover_msg(img, bit, text_file_name);
        }
    }
    
    free(img->data);
    free(img);

    return status_ok;
}

int hide_msg(struct Image *img, int bit, char text_file_name[])
{
    unsigned char *pixel_comp = (unsigned char *)img->data;
    FILE *file_ptr;
    char ch;
    unsigned char ch_mask;
    unsigned char cmp_mask;
    unsigned char temp;
    int i;
    int index = 0;
    
    file_ptr = fopen(text_file_name, "r");
    if(!file_ptr){
        printf("Error -- Couldn't open file\n");
        exit(-1);
    }
    
    while((ch = fgetc(file_ptr)) != EOF){
        unsigned char temp_ch = ch;
        if(bit == 1){
            ch_mask = 0x01;
            cmp_mask = ~ch_mask;
            
            for(i = 0; i < 8; i++){
                int k = 7-i;
                temp_ch = temp_ch>>k;
                pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp_ch&ch_mask);
                index++;
            }
        }
        else if(bit == 2){
            ch_mask = 0x03;
            cmp_mask = ~ch_mask;
            
            for(i = 0; i < 4; i++){
                int k = 8-2*(i+1);
                temp_ch = temp_ch>>k;
                pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp_ch&ch_mask);
                index++;
            }
        }
        else if(bit == 4){
            ch_mask = 0x0f;
            cmp_mask = ~ch_mask;
            
            for(i = 0; i < 2; i++){
                int k = 8-4*(i+1);
                temp_ch = temp_ch>>k;
                pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp_ch&ch_mask);
                index++;
            }
        }
    }
    fclose(file_ptr);
    
    /* :) Why is the face distorted here? */
    temp = ':';
    if(bit == 1){
        ch_mask = 0x01;
        cmp_mask = ~ch_mask;
            
        for(i = 0; i < 8; i++){
            int k = 7-i;
            temp = temp>>k;
            pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp&ch_mask);
            index++;
        }
    }
    else if(bit == 2){
        ch_mask = 0x03;
        cmp_mask = ~ch_mask;
            
        for(i = 0; i < 4; i++){
            int k = 8-2*(i+1);
            temp = temp>>k;
            pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp&ch_mask);
            index++;
        }
    }
    else if(bit == 4){
        ch_mask = 0x0f;
        cmp_mask = ~ch_mask;
            
        for(i = 0; i < 2; i++){
            int k = 8-4*(i+1);
            temp = temp>>k;
            pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp&ch_mask);
            index++;
        }
    }

    /* :) DJ Khaled, another one! */
    temp = ')';
    if(bit == 1){
        ch_mask = 0x01;
        cmp_mask = ~ch_mask;
            
        for(i = 0; i < 8; i++){
            int k = 7-i;
            temp = temp>>k;
            pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp&ch_mask);
            index++;
        }
    }
    else if(bit == 2){
        ch_mask = 0x03;
        cmp_mask = ~ch_mask;
            
        for(i = 0; i < 4; i++){
            int k = 8-2*(i+1);
            temp = temp>>k;
            pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp&ch_mask);
            index++;
        }
    }
    else if(bit == 4){
        ch_mask = 0x0f;
        cmp_mask = ~ch_mask;
            
        for(i = 0; i < 2; i++){
            int k = 8-4*(i+1);
            temp = temp>>k;
            pixel_comp[index] = (pixel_comp[index]&cmp_mask)|(temp&ch_mask);
            index++;
        }
    }
    printf("%d characters hidden in %d pixels", (index+1)/(8/bit), (index+1)/3);
    return 1;
}

int recover_msg(struct Image *img, int bit, char text_file_name[])
{
    unsigned char *pixel_comp = (unsigned char *)img->data;
    FILE *file_ptr;
    unsigned char ch_mask;
    unsigned char temp_ch;
    int i;
    unsigned char prev_ch = 0;
    unsigned char ch = 0;
    int index = 0;
    file_ptr = fopen(text_file_name, "w");
    while(prev_ch!=':'&&ch!=')'){
        if(bit == 1){
            ch = 0;
            temp_ch = 0;
            ch_mask = 0x01;
            for(i = 1; i < 9; i++){
                int k = 8-i;
                temp_ch = (pixel_comp[index]&ch_mask)<<k;
                ch = ch|temp_ch;
                index++;
            }
            fprintf(file_ptr, "%c", ch);
            prev_ch = ch;
        
        }
        else if(bit == 2){
            ch = 0;
            temp_ch = 0;
            ch_mask = 0x03;
            for(i = 1; i < 5; i++){
                int k = 8-2*i;
                temp_ch = (pixel_comp[index]&ch_mask)<<k;
                ch = ch|temp_ch;
                index++;
            }
            fprintf(file_ptr, "%c", ch);
            prev_ch = ch;
        }
        else if(bit == 4){
            ch = 0;
            temp_ch = 0;
            ch_mask = 0x0f;
            for(i = 1; i < 3; i++){
                int k = 8-4*i;
                temp_ch = (pixel_comp[index]&ch_mask)<<k;
                ch = ch|temp_ch;
                index++;
            }
            fprintf(file_ptr, "%c", ch);
            prev_ch = ch;
        }
    }   
    fclose(file_ptr);
    return 1;
}
