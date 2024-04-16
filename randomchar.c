#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <locale.h>
//#include <uchar.h>

int getRandomInRange(int lower, int upper) 
{ 
    int num = (rand() % (upper - lower + 1)) + lower; 
    return num;
} 





int main() 
{ 
    srand(time(0)); 

    int lower = 0, upper = 255;
    
    long int max = 0x0010fffd;

    char ch = max;


    char endian[4];
    endian[3] = 0;
    endian[2] = 0;
    endian[1] = 0xfe;
    endian[0] = 0xff;


    //long u = '\U0001f0a0';


    char chars[4];
    chars[0] = 0;
    chars[1] = 0x9;
    chars[2] = 0xff;
    chars[3] = 0xfd;


    //printf("%c%c%c%c", endian[0], endian[1], endian[2], endian[3]);
    printf("%c%c%c%c", endian[3], endian[2], endian[1], endian[0]);

    printf("%c%c%c%c", chars[0], chars[1], chars[2], chars[3]);


    return 0; 
} 




