#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <locale.h>

int getRandomInRange(int lower, int upper) 
{ 
    int num = (rand() % (upper - lower + 1)) + lower; 
    return num;
} 

unsigned char getRandomCharByte(){
    return getRandomInRange(0, 255);
}


void checkCharInArr(char chars[], int offset){

} 

int main() 
{ 
    FILE *file;
    const int bufSize = 40;
    char ar[bufSize];
 
    file = fopen("/dev/urandom", "r");

    int maxFileLen = 1;
    int i = 0;
    int readCount = 1;
 
    while (fgets(ar, bufSize, file) != NULL){

        //printf("%s", ar);
        if(i >= readCount){
            break;
        }
        i++;
    }
        
 
    fclose(file);


    srand(time(0)); 

    const int len = 16;

    char arr[len];

    for(int i = 0; i < len; i++){
        arr[i] = getRandomCharByte();
    }

    for(int i = 0; i < bufSize; i++){
        
        if(i % 4 == 0){
            ar[i] = 0;
        }else if((i - 1) % 4 == 0){
            ar[i] = ar[i] == 0x10 ? ar[i] : ar[i] & 0xf;
        }
        
    }


    char endian[4];
    endian[3] = 0;
    endian[2] = 0;
    endian[1] = 0xfe;
    endian[0] = 0xff;

    printf("%c%c%c%c", endian[3], endian[2], endian[1], endian[0]);

    for(int i = 0; i < bufSize; i += 4){
        printf("%c%c%c%c", ar[0+i], ar[1+i],ar[2+i],ar[3+i]);
    }
    


    return 0;
}