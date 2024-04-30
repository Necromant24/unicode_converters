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
    char ar[10];
 
    file = fopen("data.txt", "r");

    int maxFileLen = 1;
    int i = 0;
    int readCount = 3;
 
    while (fgets(ar, readCount, file) != NULL){

        printf("%s", ar);
        break;
    }
        
 
    printf("\n");
    fclose(file);


    return 0;
    srand(time(0)); 

    const int len = 16;

    char arr[len];

    for(int i = 0; i < len; i++){
        arr[i] = getRandomCharByte();
    }

    for(int i = 0; i < len; i++){
        
        if(i % 4 == 0){
            arr[i] = 0;
        }else if((i - 1) % 4 == 0){
            arr[i] = arr[i] == 0x10 ? arr[i] : arr[i] & 0xf;
        }
        
    }


    char endian[4];
    endian[3] = 0;
    endian[2] = 0;
    endian[1] = 0xfe;
    endian[0] = 0xff;

    printf("%c%c%c%c", endian[3], endian[2], endian[1], endian[0]);

    for(int i = 0; i < len; i += 4){
        printf("%c%c%c%c", arr[0+i], arr[1+i],arr[2+i],arr[3+i]);
    }
    


    return 0;
}