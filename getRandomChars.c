#include <stdlib.h> 
#include <time.h>
#include <locale.h>
#include <stdio.h>

// just for test
void test(){
    printf("wesd");
}

unsigned char * getRandomChars(const unsigned short size){
    unsigned char *buffer = calloc(size, sizeof(char));
    FILE *file;

    file = fopen("/dev/urandom", "r");

    if(fgets(buffer, size, file) == NULL){
        fclose(file);
        return NULL;
    }

    fclose(file);

    return buffer;
}
