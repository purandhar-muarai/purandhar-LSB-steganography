/* 
Name : Purandhar Murarisetty
Project name : Steganography
*/



#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc,char *argv[])
{
    if(argc > 1 )
    {
        if(check_operation_type(argv) == e_encode)
        {
            printf("Selected encoding opeartion\n");
            EncodeInfo encInfo;

            if(read_and_validate_encode_args(argv,&encInfo) == e_success) 
            {
                printf("Read and validate encode arguments is successful\n");
                if(do_encoding(&encInfo) == e_success) 
                {
                    printf("Encoding is success\n");
                }
                else
                {
                    printf("Encoding not done\n");
                }
            }
            else 
            {
                printf("Read and Validation failed\n");
                return 1;
            }
        }
        else if(check_operation_type(argv) == e_decode)
        {
            printf("Selected decoding operation\n");
            DecodeInfo decInfo;
            if(read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                printf("Read and validate decode arguments is success\n");
                if(do_decoding(&decInfo) == e_success)
                {
                    printf("Decoding is success\n");
                }
                else
                {
                    printf("Decoding is failed\n");
                }

            }
            else
            {
                printf("Read and validation id failed\n");
                return 1;
            }
        }
        else
        {
            printf("Invlaid input !!\nUsage for encoding:\n./a.out -e beautiful.bmp secret.txt [stego.bmp]\nUsage for decoding:\n./a.out -d stego.bmp [decode.txt]\n");
        }
    }

    else
    {
        printf("Usage for encoding:\n./a.out -e beautiful.bmp secret.txt [stego.bmp]\nUsage for decoding:\n./a.out -d stego.bmp [decode.txt]\n");
    }

    return 0;
}
