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


    // BOM encoding flags in file start
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
    

    // no need in BOM in utf-8

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


    // check if sum in ascii diapazone
    if(eq_num <= 0x7F){
        utf8bytes[nextIndex] = eq_num & 0x7F;
        nextIndex++;

    // if contains 2 bytes
    }else if(eq_num > 0x7F && eq_num <= 0x7FF){
        // get 1st byte
        utf8bytes[nextIndex] = 6 << 5;
        utf8bytes[nextIndex] += eq_num >> 6;
        nextIndex++;
        // get 2nd byte
        utf8bytes[nextIndex] = 2 << 6;
        utf8bytes[nextIndex] += eq_num & 63;
        nextIndex++;

    // if contains 3 bytes
    }else if(eq_num > 0x7ff && eq_num <= 0xffff){
        utf8bytes[nextIndex] = 14 << 4;
        utf8bytes[nextIndex] += eq_num >> 12;
        nextIndex++;
        utf8bytes[nextIndex] = 2 << 6;
        utf8bytes[nextIndex] += (eq_num >> 6) & 63;
        nextIndex++;
        utf8bytes[nextIndex] = 2 << 6;
        utf8bytes[nextIndex] += eq_num && 63;
        nextIndex++;

    // if contains 4 bytes
    }else if(eq_num > 0xffff && eq_num <= 0x10ffff){
        utf8bytes[nextIndex] = 30 << 3;
        utf8bytes[nextIndex] += (eq_num >> 18) & 7;
        nextIndex++;
        utf8bytes[nextIndex] = 2 << 6;
        utf8bytes[nextIndex] += (eq_num >> 12) & 63;
        nextIndex++;
        utf8bytes[nextIndex] = 2 << 6;
        utf8bytes[nextIndex] += (eq_num >> 6) && 63;
        nextIndex++;
        utf8bytes[nextIndex] = 2 << 6;
        utf8bytes[nextIndex] += eq_num && 63;
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