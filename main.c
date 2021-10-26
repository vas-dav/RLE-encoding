#include "rle_vas.h"
int main() {
    FILE *fr = NULL, *fw = NULL;
    char *fn = NULL, *c = NULL, *dfn = NULL;
    char *fnn;
    size_t fn_s;
    uint16_t crc = 0;
    int ch = 808;
    bool stop = false;
    do{
        main_menu();
        getline(&c, &fn_s, stdin);
        sscanf(c, "%d", &ch);
        switch (ch) {
            case 1:
                printf("Enter name of the file << ");
                getline(&fn, &fn_s, stdin);
                rm_n_r(fn);
                fr = fopen(fn, "rb");
                if (fr == NULL){
                    printf("%s\n", strerror(2));
                }else {
                    fnn = compress(fr, fw, fn, crc);
                    printf("File compressed successfully! Your compressed filename is %s\nMore info in [RLE_log.txt]\n", fnn);
                }
                free(fn);
                fn = NULL;
                fclose(fr);
                break;
            case 2:
                fflush(stdin);
                printf("Enter name of the file << ");
                getline(&fn, &fn_s, stdin);
                rm_n_r(fn);
                fr = fopen(fn, "rb");
                if (fr == NULL){
                    printf("%s\n", strerror(2));
                }else {
                    printf("What name do you want your file to have after decompression? << ");
                    getline(&dfn, &fn_s, stdin);
                    if (decompress(fw, fr, fn, dfn)){
                        printf("File decompressed successfully! Your new filename is %s\n", dfn);
                    } else{
                        printf("Something went wrong. Check logfile and pay attention to CRC...\n");
                    }

                }
                free(fn);
                free(dfn);
                dfn = NULL;
                fclose(fr);
                break;
            case 0:
                stop = true;
                break;
            default:
                printf("Wrong value...\n");
                break;
        }

    }while(!stop);
    free(c);
    return 0;
}


void rm_n_r(char *str){
    for (int i = 0; i < strlen(str); ++i) {
        if(str[i] == '\r' || str[i] == '\n'){
            str[i] = '\0';
        }
    }
}

bool decompress(FILE *fr, FILE *fw, char* comp_f_nm, char *new_n){
    fr = fopen(comp_f_nm, "rb");
    fw = fopen(new_n, "wb");
    uint8_t buffer, delim_buff, *data_for_crc = NULL;
    uint8_t count = 0;
    uint16_t crc_check, new_crc;
    bool crc_valid = false;
    long data_s;
    fscanf(fr, "%d", &crc_check);
        while (!feof(fr)) {
            fread(&buffer, 1, 1, fr);
            if (buffer == HEX_DELIM) {
                delim_buff = buffer;
                fread(&buffer, 1, 1, fr);
                if (buffer == 0x00) {
                    fwrite(&delim_buff, 1, 1, fw);
                } else {
                    count = buffer;
                    fread(&buffer, 1, 1, fr);
                    for (int j = 0; j < count; ++j) {
                        fwrite(&buffer, 1, 1, fw);
                    }
                }
            }
        }
        fclose(fw);
        //Checking the CRC
        fw = fopen(new_n, "rb");
    if (fw == NULL){
        printf("%s\n", strerror(2));
    }else{
        data_s = getfile_size(fw);
        data_for_crc = calloc(data_s, 1);
        fread(data_for_crc, 1, data_s, fw);
        new_crc = crc16(data_for_crc, data_s);
        if (crc_check == new_crc) crc_valid = true;
    }
    //Closing actions
    free(data_for_crc);
    data_for_crc = NULL;
    fclose(fr);
    fclose(fw);
    return crc_valid;
}

char *compress(FILE *fr, FILE *fw, char *f_name, uint16_t crc){
    uint8_t buffer, nextbuff;
    uint8_t *init_data;
    int count = 0;
    int flag = 0;
    char *crc_s;
    time_t t;
    time(&t);
    long file_s;

    FILE *log = fopen("RLE_log.txt", "a");
    file_s = getfile_size(fr);
    fprintf(log, "%s| Filename: %s | Original size: %lu bytes | ", ctime(&t), f_name, file_s);
    strncat(f_name, FILE_APPEND, 4);
    fw = fopen(f_name, "wb");
        //CRC actions
        init_data = calloc(file_s, 1);
        fread(init_data, 1, file_s, fr);
        rewind(fr);
        crc = crc16(init_data, file_s);
        crc_s = calloc(2, 1);
        sprintf(crc_s, "%d", crc);
        fprintf(fw, "%d", crc);
        fprintf(log, "CRC: %s | Compressed filename: %s | ", crc_s, f_name);
        free(crc_s);
        free(init_data);
        do{
            flag = (fread(&buffer, 1,1, fr) == 0);
            if (flag || nextbuff != buffer) {

                if (count > 0) {
                    if (nextbuff == HEX_DELIM && count < 2){
                        fwrite(&nextbuff,1,1,fw);
                        fputc(0x00, fw);
                    }else{
                        fputc(HEX_DELIM, fw);
                        fwrite(&count,1,1,fw);
                        fwrite(&nextbuff,1,1,fw);
                    }
                }
                nextbuff = buffer;
                count = 1;
            } else{
                count++;
            }
        }while(!feof(fr));
    fprintf(log, "Compressed size: %lu bytes |\n", getfile_size(fw));
    fclose(log);
    return f_name;
}
uint16_t crc16(const uint8_t *data_p, unsigned int length) {
    uint8_t x;
    uint16_t crc = 0xFFFF;
    while (length--) {
        x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }
    return crc;
}

long getfile_size(FILE *f){
    long fs;
    fseek(f, 0, SEEK_END);
    fs = ftell(f);
    rewind(f);
    return fs;
}

void main_menu(){
    printf("| You have choices to encode or decode your file with RLE |\nPlease choose either:\n0. EXIT\n1. ENCODING(pack)\n2. DECODING(unpack)\n Your choice <<< ");
}