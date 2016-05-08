/**
 * @file kBuffer.c
 * @author Peter Kappelt
 * @date __DATE__ __TIME__
 * @brief A universal ringbuffer library
 * A (more or less) universal ringbuffer library, suitable (at least) for x86 and AVR
 * @see https://github.com/peterkappelt/kBuffer
 */

#include "kBuffer.h"

#include <stdlib.h>

/**
 * @brief init a new buffer
 * This function inits a new buffer_t.<br>
 * @param   buffer  Pointer (&) to a buffer_t object.
 * @param   bufferSize  desired size of the buffer, the total buffer size (e.g. length-of-datatype * bufferSize) may not exceed 2^16 bytes
 * @return  an element of bufferStatus_t
 */
bufferStatus_t bufferInit(buffer_t* buffer, uint16_t bufferSize){
    buffer->readPointer = 0;
    buffer->writePointer = 0;
    buffer->datacount = 0;
    
    buffer->elementLength = sizeof(bufferDatatype);
    buffer->data = malloc(buffer->elementLength * bufferSize);
    if(buffer->data != NULL){
        buffer->isInitialized = 1;
        buffer->length = bufferSize;
        return bufferOK;
    }else{
        return bufferMemoryAllocationFailed;
    }
}