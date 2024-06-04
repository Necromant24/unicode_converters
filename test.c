#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <locale.h>
//#include <uchar.h>


int main()
{ 
    unsigned char n = 129;
    unsigned char r = (n >>0b1);
    printf("%d", r);

    return 0;
} 




