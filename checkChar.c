#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <locale.h>

int getRandomInRange(int lower, int upper) 
{ 
    int num = (rand() % (upper - lower + 1)) + lower; 
    return num;
} 

int getRandomCharByte(){
    return getRandomInRange(0, 255);
}


void checkCharInArr(char chars[], int offset){

} 

int main() 
{ 
    srand(time(0)); 

    const int len = 16;

    char arr[len];

    for(int i = 0; i < len; i++){
        arr[i] = getRandomCharByte();
    }

    for(int i = 0; i < len; i++){
        unsigned short rnum = abs(arr[i]);
        if(i % 4 == 0){
            arr[i] = 0;
        }else if((i - 1) % 4 == 0){
            arr[i] = rnum;
            if(rnum>0x10 && rnum != 0x10){
                rnum = rnum & 0x0f;
                arr[i] = rnum;
            }
            
        }else if(i - 2 % 4 == 0){
            arr[i] = rnum;
        }else if(i - 3 % 4 == 0){
            arr[i] = rnum;
            if(arr[i-2] == 0x10 && arr[i-1] == 0xff && arr[i] == 0xff){
                arr[i] = 0xfd;
            }
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