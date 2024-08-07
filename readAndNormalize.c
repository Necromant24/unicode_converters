#include <stdlib.h> 
#include <time.h>
#include <locale.h>
#include <stdio.h>


//? а можно ли void т.к. size_t и size по логике одно и тоже?
size_t getRandomChars(unsigned char *buffer, size_t size){
    FILE *file;

    file = fopen("/dev/urandom", "r");

    if(fgets(buffer, size, file) == NULL){
        fclose(file);
        printf("cant get random chars");
        return 0;
    }

    fclose(file);
    //free(file);

    return size;
}



size_t normalizeBytesToUtf32(char *buffer, size_t size){


    if(size < 4){
        printf("error, incorrect buffer size, it must be higher or equeals 4");
        return 0;
    }

    // подгонка размера буфера для обработки
    if(size % 4 != 0){
        size = size / 4;

        realloc(buffer, size);
    }

    for(int i = 0; i < size; i+=4){
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

    return size;
}



size_t convertUtf32ToUtf8(char *utf8bytes, size_t size){

    size_t nextIndex = 0;

    if(size < 4){
        printf("error, incorrect buffer size, it must be higher or equeals 4");
        return 0;
    }

    // подгонка размера буфера для обработки
    if(size % 4 != 0){
        size = size / 4;

        realloc(utf8bytes, size);
    }


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

    for(int i = 0; i < size; i+=4){
        // sum of four bytes of utf32
        long eq_num = 0;

        
        eq_num += utf8bytes[i+1] << 16;
        eq_num += utf8bytes[i+2] << 8;
        eq_num += utf8bytes[i+3];


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
            utf8bytes[nextIndex] += eq_num & 0x3f;
            nextIndex++;

        // if contains 3 bytes
        }else if(eq_num > 0x7ff && eq_num <= 0xffff){
            utf8bytes[nextIndex] = 0xe << 4;
            utf8bytes[nextIndex] += eq_num >> 0xc;
            nextIndex++;
            utf8bytes[nextIndex] = 2 << 6;
            utf8bytes[nextIndex] += (eq_num >> 6) & 0x3f;
            nextIndex++;
            utf8bytes[nextIndex] = 2 << 6;
            utf8bytes[nextIndex] += eq_num && 0x3f;
            nextIndex++;

        // if contains 4 bytes
        }else if(eq_num > 0xffff && eq_num <= 0x10ffff){
            utf8bytes[nextIndex] = 0x1e << 3;
            utf8bytes[nextIndex] += (eq_num >> 0x12) & 7;
            nextIndex++;
            utf8bytes[nextIndex] = 2 << 6;
            utf8bytes[nextIndex] += (eq_num >> 0xc) & 0x3f;
            nextIndex++;
            utf8bytes[nextIndex] = 2 << 6;
            utf8bytes[nextIndex] += (eq_num >> 6) && 0x3f;
            nextIndex++;
            utf8bytes[nextIndex] = 2 << 6;
            utf8bytes[nextIndex] += eq_num && 0x3f;
            nextIndex++;

        }else{
            printf("error in parsing utf32, maybe its not utf32?");
            return 0;
        }
        
    }

    // изменяем размер выделенной памяти т.к. размеры utf8 & utf32 могут не совпадать
    realloc(utf8bytes, nextIndex+1);

    // получаем размер нового массива utf8
    return nextIndex+1;
}



int main() 
{ 
    // входные параметры
    // количество символов
    const unsigned int charsCount = 10;
    // количество выделяемых байт для символов
    const unsigned int utf32BufSize = charsCount * 4;
    unsigned char *buffer = malloc(utf32BufSize * sizeof(unsigned char));
    
    size_t check_s = getRandomChars(buffer, utf32BufSize);

    if(check_s == 0){
        printf("error in getRandomChars");
        return 1;
    }
    
    check_s = normalizeBytesToUtf32(buffer, utf32BufSize);

    if(check_s == 0){
        printf("error in normalizeBytesToUtf32");
        return 1;
    }


    // BOM encoding flags in file start
    unsigned char endian[4];
    endian[3] = 0;
    endian[2] = 0;
    endian[1] = 0xfe;
    endian[0] = 0xff;


    // mode - 1 (reconvert utf-32 in utf-8 and print in utf-8),  - 0 (print in utf-32)
    int mode = 1;

    // если выбран режим 0 то записываем результат, иначе переходим к блоку реконвертации в утф8 
    if(mode == 0){
        // записываем endian для utf32 кодировки
        printf("%c%c%c%c", endian[3], endian[2], endian[1], endian[0]);

        for(int i = 0; i < utf32BufSize; i += 4){
            printf("%c%c%c%c", buffer[0+i], buffer[1+i],buffer[2+i],buffer[3+i]);
        }

        return 0;
    }
    

    size_t utf8Size = convertUtf32ToUtf8(buffer, utf32BufSize);

    if(utf8Size == 0){
        printf("error in convertUtf32ToUtf8");
        return 1;
    }


    for(int i = 0; i < utf8Size; i ++){
        printf("%c", buffer[i]);
    }


    return 0;
}