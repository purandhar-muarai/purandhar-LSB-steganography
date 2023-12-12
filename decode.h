#ifndef DECODE_H
#define DECODE_H
#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct DECODE_INFO
{
    /* image data */
    char *img_name;
    FILE *fptr_img;
    char img_extn[MAX_FILE_SUFFIX];

    /* file data */
    char *file_name;
    FILE *fptr_file;
    int file_extn_size;
    int file_size;
    char file_extn[MAX_FILE_SUFFIX];
    char in_file_name[MAX_FILE_SUFFIX * 2];

    /* char storing */
    char ch;
    char *data;
    /* integer value storing */
    int val;

}DecodeInfo;


/* if the secret text size increases increase the size of calloc for the decoding(if you encountered stack smashing at the end  

   decInfo -> data = calloc(100, sizeof(char));
 */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open__files(DecodeInfo *decInfo);

/* Decoding magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/* decode char form image */
Status decode_data_form_lsb(int size,DecodeInfo *decInfo);

/* decoding bytes form lsb*/
Status decode_bytes_form_lsb(char *img_buffer, DecodeInfo *decInfo);

/* extracting file extension size */
Status decode_file_extn(DecodeInfo *decInfo);

/* decoding size form file */
Status decode_size_form_lsb(DecodeInfo *decInfo);

/* decoding file extension size */
Status decode_extn_size(DecodeInfo *decInfo);

/* decoding file size */
Status decode_ile_size(DecodeInfo *decInfo);

/*  validate given file as input */
Status validate_file(DecodeInfo *decInfo);

/* decoding secret file data */
Status decode_secret_data(DecodeInfo *decInfo);


#endif
