#include <stdlib.h>
#include <stdio.h>

#include <kBuffer.h>

int bufferLength = 8;

int main(void){
 
    printf("kBuffer Demo. Built at ");
    printf(__DATE__);
    printf(" ");
    printf(__TIME__);
    printf("\n");
    
    buffer_t buffer;
    
    printf("Try to init buffer with %d elements: ", bufferLength);
    
    if(bufferInit(&buffer, bufferLength) == bufferOK){
        printf("Success!\n");
    }else{
        printf("Error!\n");
        exit(0);
    }
    
    return 0;
}