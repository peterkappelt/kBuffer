#include <stdlib.h>
#include <stdio.h>

#include <kBuffer.h>

int bufferLength = 8;
bufferStatus_t status;

void verboseBuffer(buffer_t* buffer){
    printf("------BUFFER VERBOSE------\n");
    printf("isInitialized: %d\n", buffer->isInitialized);
    printf("writePointer: %d\n", buffer->writePointer);
    printf("readPointer: %d\n", buffer->readPointer);
    printf("length: %d\n", buffer->length);
    printf("elementLength: %d\n", buffer->elementLength);
    printf("datacount: %d\n", buffer->datacount);
    printf("--------------------------\n");
}

int main(void){
 
    printf("kBuffer Demo. Built at ");
    printf(__DATE__);
    printf(" ");
    printf(__TIME__);
    printf("\n\n\n");
    
    buffer_t buffer;
    
    printf("Try to init buffer with %d elements: ", bufferLength);
    status = bufferInit(&buffer, bufferLength);
    //status = bufferOK;
    if(status == bufferOK){
        printf("Success, bytes acquired: %d\n\n", buffer.elementLength * buffer.length);
    }else{
        printf("Error!\n\n");
        exit(0);
    }
    
    int index = 7, testdata = 295;
    uint16_t datanew;
    
    printf("bufferWriteToIndex at index %d with data %d: ", index, testdata);
    status = bufferWriteToIndex(&buffer, index, testdata);
    if(status == bufferOK){
        printf("OK!\n\n");
    }else if(status == bufferNotInitialized){
        printf("Not initialized!\n\n");
    }else if(status == bufferError){
        printf("Not a valid index!\n\n");
    }
    
    printf("bufferReadFromIndex at index %d: ", index);
    status = bufferReadFromIndex(&buffer, index, &datanew);
    if(status == bufferOK){
        printf("OK!\n");
    }else if(status == bufferNotInitialized){
        printf("Not initialized!\n");
    }else if(status == bufferError){
        printf("Not a valid index!\n");
    }
    printf("Got %d: ", datanew);
    if(datanew == testdata){
        printf("Verify OK\n\n");
    }else{
        printf("Verify failure\n\n");
    }
    
    int flag = 0;
    char cmd = '\0';
    uint16_t data;
    bufferStatus_t status;
    
    printf("Commands: \n\t [q]uit\n\t [w]rite data\n\t [r]ead data\n\t [v]erbose buffer\n");
    
    while(!flag){
        printf("Input: ");
        __fpurge(stdin);
        scanf("%c", &cmd);
        printf("\n");
        if(cmd == 'q'){
            printf("Bye!\n");
            flag = 1;
        }else if(cmd == 'w'){
            printf("Data to write (integer): ");
            int temp;
            scanf("%d", &temp);
            data = (uint16_t)temp;
            printf("Try to write %d: ", data);
            status = bufferWrite(&buffer, data);
            if(status == bufferFull){
                printf("Is Full!\n");
            }else if(status == bufferOK){
                printf("OK\n");
            }else{
                printf("Other error\n");
            }
        }else if(cmd == 'r'){
            printf("Try to read data: ");
            status = bufferRead(&buffer, &data);
            if(status == bufferEmpty){
                printf("Is Empty!\n");
            }else if(status == bufferOK){
                printf("%d\n", data);
            }else{
                printf("Other error\n");
            }
        }else if(cmd == 'v'){
            verboseBuffer(&buffer);
        }else{
            printf("Command not recognized!\n");
        }
    }
    
    return 0;
}