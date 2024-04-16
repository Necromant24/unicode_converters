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
    const int len = 40;

    unsigned char s[40];
    unsigned char s2[40];


	unsigned long int * as_longs = (unsigned long int *) s;

    for(int i = 0; i < len; i++){
        s[i] = getRandomInRange(lower, upper);
//        printf("rand num : %d", s[i]);
    }

//    printf("\n\r\n\r");

    //int a = 0x10ffff;
    int xorNum = 0x10;
 
	unsigned long int mask = 0x10ffff;

//	printf("%c%c%c%c",0,0,0xFE,0xFF);

    for(int i = 0; i < len; i+=4){

        s2[i] = 0;
        s2[i+1] = s[i+1] ^ xorNum;
        s2[i+2] = s[i+2];
        s2[i+3] = s[i+3];
        
        fprintf(stderr, "num : %d %d %d %d \r\n", s2[i], s2[i+1], s2[i+2], s2[i+3]);
		    printf("%c%c%c%c",s2[i+3], s2[i+2], s2[i+1], s2[i+0]); 
    }


    return 0; 
} 




