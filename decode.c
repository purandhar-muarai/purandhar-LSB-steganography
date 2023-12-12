#include <stdio.h>
#include "decode.h"
#include <string.h>
#include "common.h"
#include <stdlib.h>


Status open__files(DecodeInfo *decInfo)
{
    //opening image file
    decInfo -> fptr_img = fopen(decInfo -> img_name,"r");
    // Do Error handling
    if (decInfo->fptr_img == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->img_name);

        return e_failure;
    }

    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strcmp(strchr(argv[2],'.'),".bmp") == 0)
    {
        decInfo -> img_name = argv[2];
        decInfo -> file_name = argv[3];
        return e_success;
    }
    else
        return e_failure;
}

Status decode_bytes_form_lsb(char *str, DecodeInfo *decInfo)
{
    decInfo -> ch = 0;
    for(int i = 0,j = 7; i < 8; i++)
    {
        decInfo -> ch = decInfo -> ch | ((str[i] & 1) << (7-i)) ;
    }
    return e_success;
}

Status decode_data_form_lsb(int size, DecodeInfo *decInfo)
{
    char str[8];
    int i,j;
    for(i = 0,j = 0; i < size*8; i+=8)
    {
        fread(str, 8, 1, decInfo -> fptr_img);
        decode_bytes_form_lsb(str, decInfo);
        decInfo -> data[j++] = decInfo -> ch;
    }
    decInfo -> data[j] = '\0';
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo) 
{
    if((decode_data_form_lsb(strlen(MAGIC_STRING), decInfo)) == e_success)
    {

        if(strcmp(decInfo -> data,MAGIC_STRING) == 0)
        {
            //printf("%s\n",decInfo -> data);
            return e_success;
        }
        else
        {
            return e_failure;
        }
    }
    else
        return e_failure;

}

Status decode_size_form_lsb(DecodeInfo *decInfo)
{
    char str[32];
    decInfo -> val = 0;
    fread(str, 32, 1, decInfo -> fptr_img);
    for(int i = 0; i < 32; i++)
    {
        decInfo -> val = decInfo -> val | ((str[i] & 1) << (31 - i)) ;
    }
    return e_success;
}

Status decode_extn_size(DecodeInfo *decInfo)
{
    if(decode_size_form_lsb(decInfo) == e_success)
    {
        decInfo -> file_extn_size = decInfo -> val;
        //printf("%d\n",decInfo->val);
        return e_success;
    }
    else 
        return e_failure;
}

Status decode_file_extn(DecodeInfo *decInfo)
{
    if(decode_data_form_lsb(4,decInfo) == e_success)
    {
        strcpy(decInfo -> file_extn, decInfo -> data);
        //printf("%s\n",decInfo -> file_extn);
        return e_success;
    }
    else
        return e_failure;
}

Status decode_file_size(DecodeInfo *decInfo)
{
    if(decode_size_form_lsb(decInfo) == e_success)
    {
        decInfo -> file_size = decInfo -> val;
        //printf("%d\n",decInfo -> file_size);
        return e_success;
    }
    else
        return e_failure;
}

Status validate_file(DecodeInfo *decInfo)
{
    if(decInfo -> file_name == NULL)
    {
        decInfo -> file_name = "decode.txt";
    }
    else
    {
        char *dot = strchr(decInfo -> file_name,'.');
        *dot = '\0';
        strcat(decInfo -> file_name, ".txt");
    }

    //opening text file
    decInfo -> fptr_file = fopen(decInfo -> file_name, "w");
    // Do Error handling
    if (decInfo->fptr_file == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->file_name);

        return e_failure;
    }
    return e_success;
}

Status decode_secret_data(DecodeInfo *decInfo)
{
    if(decode_data_form_lsb(decInfo -> file_size,decInfo) == e_success)
    {
        fwrite(decInfo -> data, decInfo -> file_size, 1, decInfo -> fptr_file);
        return e_success;
    }
    else
        return e_failure;
}

Status do_decoding(DecodeInfo *decInfo)
{
    /* if the secret text size increases increase the size of calloc for the decoding(if you encountered stack smashing at the end  */
    decInfo -> data = calloc(100, sizeof(char));

    if(open__files(decInfo) == e_success)
    {
        printf("Files are opened Sucessfully\n");
        fseek(decInfo -> fptr_img, 0, SEEK_SET);
        fseek(decInfo -> fptr_img, 54, SEEK_SET);
        if(decode_magic_string(decInfo) == e_success)
        {
            printf("Decoded magic string sucessfully\n");
            if(decode_extn_size(decInfo) == e_success)
            {
                printf("Decoded file extension size sucessfully\n");
                if(decode_file_extn(decInfo) == e_success)
                {
                    printf("Decoded file extension sucessfully\n");
                    if(decode_file_size(decInfo) == e_success)
                    {
                        printf("Decoded file size Sucessfully\n");
                        if(validate_file(decInfo) == e_success)
                        {
                            printf("Output file is validate and opened sucessfully\n");
                            if(decode_secret_data(decInfo) == e_success)
                            {
                                printf("Decoded decret data successfully\n");
                            }

                            else
                            {
                                printf("Failed to open output file\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("Failed to decode secret data\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failed to decode file size\n");
                        return e_failure;
                    }
                }
            }
            else
            {
                printf("Failed to decode file extension size\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open files\n");
        return e_failure;
    }
    return e_success;
}
