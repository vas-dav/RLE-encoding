#define  _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<time.h>

#define HEX_DELIM 128
#define FILE_APPEND ".rle"

//Removing the unnecessary characters from an input string
void rm_n_r(char *str);
//Compressing the file and returning new filename
char *compress(FILE *fr, FILE *fw, char *f_name, uint16_t crc);
//Decompresses the file
bool decompress(FILE *fr, FILE *fw, char *comp_f_nm, char *new_n);
//CRC calc
uint16_t crc16(const uint8_t *data_p, unsigned int length);
//Getting the size of the file via pointer
long getfile_size(FILE *f);
//Displaying the menu of the program
void main_menu();

