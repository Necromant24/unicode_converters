#include <stdlib.h> 
#include <time.h>
#include <locale.h>
#include <stdio.h>

int main() 
{ 
    FILE *file;
    const unsigned int charsCount = 10;
    const unsigned int utf32BufSize = charsCount * 4;
    unsigned char buffer[utf32BufSize];
 
    file = fopen("/dev/urandom", "r");
 
    if(fgets(buffer, utf32BufSize, file) == NULL){
        fclose(file);
        return 1;
    }


    fclose(file);
 

    for(int i = 0; i < utf32BufSize; i+=4){
        buffer[i] = 0;

        /* Второй байт utf-32 принимает значения от 0x00 до 0x10 */
                /* И нам надо сделать так чтобы появление этих значений было */
                /* Как можно равномернее распределено по значениям исходного байта */

                if ( (buffer[i+1] & 0xF0) == 0xF0) // вероятность этого 1/16
                {
                   buffer[i+1] = 0x10;
                } else
                {
                   buffer[i+1] &= 0x0F;  // Значения от 0 до 0x0F, вероятность каждого из них 1/16 - 1/256 (с учетом предыдущей ветки if'а)
                }

        // байты 3 и 4 оставляем как есть
    }


    unsigned char endian[4];
    endian[3] = 0;
    endian[2] = 0;
    endian[1] = 0xfe;
    endian[0] = 0xff;


    // mode - 1 (reconvert utf-32 in utf-8 and print in utf-8),  - 0 (print in utf-32)
    int mode = 1;

    if(mode == 0){
        printf("%c%c%c%c", endian[3], endian[2], endian[1], endian[0]);

        for(int i = 0; i < utf32BufSize; i += 4){
            printf("%c%c%c%c", buffer[0+i], buffer[1+i],buffer[2+i],buffer[3+i]);
        }

        return 0;
    }
    

    // no need in BOM?
    // unsigned char utf8endian[3];
    // utf8endian[0] = 0xEF;
    // utf8endian[1] = 0xBB;
    // utf8endian[2] = 0xBF;
    //printf("%c%c%c", utf8endian[0], utf8endian[1], utf8endian[2]);

    unsigned char utf8test[3];
    utf8test[0] = 0x74;
    utf8test[1] = 0x65;
    utf8test[2] = 0x73;

    printf("%c%c%c", utf8test[0], utf8test[1], utf8test[2]);



    unsigned char utf32to8bytes[8];
    utf32to8bytes[0] = 0;
    utf32to8bytes[1] = 0xf;
    utf32to8bytes[2] = 0xad;
    utf32to8bytes[3] = 0x63;

    utf32to8bytes[4] = 0;
    utf32to8bytes[5] = 0x1;
    utf32to8bytes[6] = 0x1;
    utf32to8bytes[7] = 0x1;


    unsigned char utf8bytes[utf32BufSize];
    unsigned int nextIndex = 0;


    // 2^11 - ( 11 бит в 2х битном варианте )
    /*

    utf 8 bytes maps and orders
    0xxx xxxx                       0xxx-xxxx

    0000 0yyy yyxx xxxx             110y-yyyy 10xx-xxxx

    zzzz уууу ууxx xxxx             1110-zzzz 10yy-yyyy 10xx-xxxx

    000u uuuu zzzz yyyy yyxx xxxx   1111-0uuu 10uu-zzzz 10yy-yyyy 10xx-xxxx




    utf32
    max
    10FFFF - hex
    000100001111111111111111  - bin
    000xxxxxxxxxxxxxxxxxxxxx  -   x _ available bits
    000uuuuuzzzzyyyyyyxxxxxx



    0000-0000__000u-uuuu__zzzz-yyyy__yyxx-xxxx


    */

for(int i = 0; i < utf32BufSize; i+=4){
    // sum of four bytes of utf32
    long eq_num = 0;

    unsigned char d[4];
    d[0] = buffer[i];
    d[1] = buffer[i+1];
    d[2] = buffer[i+2];
    d[3] = buffer[i+3];

    eq_num += buffer[i+1] << 16;
    eq_num += buffer[i+2] << 8;
    eq_num += buffer[i+3];


    // if sum in ascii diapazone
    if(eq_num <= 0b1111111){
        utf8bytes[nextIndex] = eq_num & 0b1111111;
        nextIndex++;

    // if contains 2 bytes
    }else if(eq_num > 0b1111111 && eq_num <= 0b11111111111){
        // get 1st byte
        utf8bytes[nextIndex] = 0b110 << 5;
        utf8bytes[nextIndex] += eq_num >> 6;
        nextIndex++;
        // get 2nd byte
        utf8bytes[nextIndex] = 0b10 << 6;
        utf8bytes[nextIndex] += eq_num & 0b111111;
        nextIndex++;

    // if contains 3 bytes
    }else if(eq_num > 0b11111111111 && eq_num <= 0b1111111111111111){
        utf8bytes[nextIndex] = 0b1110 << 4;
        utf8bytes[nextIndex] += eq_num >> 12;
        nextIndex++;
        utf8bytes[nextIndex] = 0b10 << 6;
        utf8bytes[nextIndex] += (eq_num >> 6) & 0b111111;
        nextIndex++;
        utf8bytes[nextIndex] = 0b10 << 6;
        utf8bytes[nextIndex] += eq_num && 0b111111;
        nextIndex++;

    // if contains 4 bytes
    }else if(eq_num > 0b1111111111111111 && eq_num <= 0x10ffff){
        utf8bytes[nextIndex] = 0b11110 << 3;
        utf8bytes[nextIndex] += (eq_num >> 18) & 0b111;
        nextIndex++;
        utf8bytes[nextIndex] = 0b10 << 6;
        utf8bytes[nextIndex] += (eq_num >> 12) & 0b111111;
        nextIndex++;
        utf8bytes[nextIndex] = 0b10 << 6;
        utf8bytes[nextIndex] += (eq_num >> 6) && 0b111111;
        nextIndex++;
        utf8bytes[nextIndex] = 0b10 << 6;
        utf8bytes[nextIndex] += eq_num && 0b111111;
        nextIndex++;

    }else{
        printf("error");
        return 1;
    }
    
}



    for(int i = 0; i < nextIndex; i ++){
        printf("%c", utf8bytes[i]);
    }


    return 0;
}