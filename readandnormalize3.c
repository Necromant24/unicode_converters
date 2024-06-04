#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <locale.h>


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

        // normalized char
        unsigned char nCh = buffer[i+1];
        unsigned char nChp1 = nCh & 0xf;
        unsigned char nChp2 = nCh & 0x10;


        if(nCh == 0x10 || nCh == 0 || (nChp1 == 0 && nChp2 == 0x10)){
            buffer[i+1] = 0x10;
        }else{
            buffer[i+1] = buffer[i+1] & 0xf;
        }


        //buffer[i+1] = buffer[i+1] == 0x10 ? buffer[i+1] : (buffer[i+1] == 0x0 ? 0x10 : buffer[i+1] & 0xf);
    }


    unsigned char endian[4];
    endian[3] = 0;
    endian[2] = 0;
    endian[1] = 0xfe;
    endian[0] = 0xff;

    printf("%c%c%c%c", endian[3], endian[2], endian[1], endian[0]);

    for(int i = 0; i < utf32BufSize; i += 4){
        printf("%c%c%c%c", buffer[0+i], buffer[1+i],buffer[2+i],buffer[3+i]);
    }



    return 0;
}