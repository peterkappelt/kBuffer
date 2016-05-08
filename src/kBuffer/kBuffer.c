/**
 * @file kBuffer.c
 * @author Peter Kappelt
 * @brief A universal ringbuffer library
 * @see https://github.com/peterkappelt/kBuffer
 * @copyright Peter Kappelt 2016; MIT License (see LICENSE.txt in the root of this repository)
 */

/**
 * @mainpage
 * @section intro Introduction
 * kBuffer is a universal library for a ring- / circular buffer.
 * @section function Functions and Datatypes
 * buffer_t<br>
 * #bufferStatus_t<br>
 * <br>
 * bufferInit()<br>
 * <br>
 * bufferWrite();<br>
 * bufferRead();<br>
 * <br>
 * bufferIsFull()<br>
 * bufferIsEmpty()<br>
 * bufferWriteToIndex()<br>
 * bufferReadFromIndex()<br>
 * @section usage Usage and Examples
 * @subsection init Initializing a ringbuffer
 * At first, you have to include the kBuffer library into your project.
 * This can be done by copying the files from src/kBuffer to your project's directory.
 * You can include the header as usual:
 * @code
 * #include "kBuffer.h"
 * @endcode
 * In your code, you have to define an instance of buffer_t. You have to init this instance with the function bufferInit(). If you want to have a ringbuffer with 8 elements:
 * @code
 * buffer_t ringbuffer;
 * bufferInit(&ringbuffer, 8);
 * @endcode
 * To check, if the initialization was successfull, you need to parse the return value of bufferInit():
 * @code
 * buffer_t ringbuffer;
 * if(bufferInit(&ringbuffer, 8) == bufferOK){
 *  do_something_it_worked_ok();
 * }else{
 *  do_something_there_was_an_error();
 * }
 * @endcode
 * @subsection write Writing data to the buffer
 * To write data to the buffer, you can use the bufferWrite() function:
 * @code
 * #include "kBuffer.h"
 * 
 * int main(void){
 * 
 *  buffer_t ringbuffer;            // Declare an buffer instance
 *  bufferInit(&ringbuffer, 8);     // Init the buffer with 8 elements
 *  //Notice, that no errorhandling has been done. We just expect a success
 *  
 *  bufferWrite(&ringbuffer, 42);   // Write the integer "42" to the buffer.
 * 
 *  return 0;
 * }
 * @endcode
 * @subsection read Reading data from the buffer
 * To read data from the buffer, you can use the bufferRead() function:
 * @code
 * #include "kBuffer.h"
 * 
 * int main(void){
 * 
 *  buffer_t ringbuffer;                // Declare an buffer instance
 *  bufferInit(&ringbuffer, 8);         // Init the buffer with 8 elements
 *  //Notice, that no errorhandling has been done. We just expect a success
 *  
 *  bufferWrite(&ringbuffer, 42);       // Write the integer "42" to the buffer.
 * 
 *  uint16_t dataRead;                  // Declare an integer, where the read data should be stored
 *  bufferRead(&ringbuffer, &dataRead); // We expect, that dataRead is now 42 (because we have written 42 to the buffer before)
 * 
 *  return 0;
 * }
 * @endcode
 * @section example Example code
 * An example code project is available under ../../test/x86. It isn't well documented, but you can compile it for your system.
 */

#include "kBuffer.h"

#include <stdlib.h>

/**
 * @brief init a new buffer
 * This function inits a new buffer_t.<br>
 * @param   buffer  Pointer (&) to a buffer_t object.
 * @param   bufferSize  desired size of the buffer, the total buffer size (e.g. length-of-datatype * bufferSize) may not exceed 2^16 bytes
 * @return  an element of #bufferStatus_t
 * @retval  bufferMemoryAllocationFailed The memory allocation with malloc failed. Make sure, you have enough memory available
 * @retval  bufferOK    It seems, like everything went well
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
        buffer->isInitialized = 0;
        return bufferMemoryAllocationFailed;
    }
}

/**
 * @brief write data to a specific index of the buffer. 
 * WARNING: Take care when using this function, it is against the main concept of a ringbuffer
 * @param   buffer  Pointer to a buffer_t instance
 * @param   index   The index, where data should be written.<br>It can be in range 0 to length - 1
 * @param   data    The actual data which should be written
 * @return  an element of #bufferStatus_t
 * @retval  bufferOK    It went successfull
 * @retval  bufferNotInitialized    The buffer is not initialized. You have to call bufferInit before (or the init failed before)
 * @retval  bufferError The desired data index is out of range
 */
bufferStatus_t bufferWriteToIndex(buffer_t* buffer, uint16_t index, bufferDatatype data){
    if(buffer->isInitialized){
        if(index <= (buffer->length - 1)){
            *(buffer->data + (buffer->elementLength * index)) = data;
            return bufferOK;
        }else{
            return bufferError;
        }
    }else{
        return bufferNotInitialized;
    }
}

/**
 * @brief read data from a specifig index of the buffer
 * WARNING: Take care when using this function, it is against the main concept of a ringbuffer
 * @param   buffer  Pointer to a buffer_t instance
 * @param   index   The index, where data should be written.<br>It can be in range 0 to length - 1
 * @param   data    Pointer to a variable where the read data should be written to.
 * @return  an element of #bufferStatus_t
 * @retval  bufferOK    It went successfull
 * @retval  bufferNotInitialized    The buffer is not initialized. You have to call bufferInit before (or the init failed before)
 * @retval  bufferError The desired data index is out of range
 */
bufferStatus_t bufferReadFromIndex(buffer_t* buffer, uint16_t index, bufferDatatype* data){
    if(buffer->isInitialized){
        if(index <= (buffer->length - 1)){
            *data = *(buffer->data + (buffer->elementLength * index));
            return bufferOK;
        }else{
            return bufferError;
        }
    }else{
        return bufferNotInitialized;
    }
}

/**
 * @brief Checks, wheter the buffer is empty
 * @param buffer Pointer to a buffer_t instance
 * @retval  1   buffer is empty
 * @retval  0   buffer is not empty
 */
uint8_t bufferIsEmpty(buffer_t* buffer){
    if((buffer->writePointer == buffer->readPointer) && (buffer->datacount == 0)){
        return 1;
    }else{
        return 0;
    }
}

/**
 * @brief Checks, wheter the buffer is full
 * @param buffer Pointer to a buffer_t instance
 * @retval  1   buffer is full
 * @retval  0   buffer is not full
 */
uint8_t bufferIsFull(buffer_t* buffer){
    if((buffer->writePointer == buffer->readPointer) && (buffer->datacount == buffer->length)){
        return 1;
    }else{
        return 0;
    }
}

/**
 * @brief add data to the end of the ringbuffer
 * @param   buffer  pointer to a buffer_t instance
 * @param   data    data which should be written
 * @return  a element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected
 * @retval  bufferNotInitialized    the bufferInit() method hasn't been called or failed before
 * @retval  bufferFull  the buffer is full an no more data can be written
 */
bufferStatus_t bufferWrite(buffer_t* buffer, bufferDatatype data){
    if(!bufferIsFull(buffer)){
        bufferStatus_t temp = bufferWriteToIndex(buffer, buffer->writePointer, data);
        if(temp == bufferOK){
            buffer->datacount++;
            buffer->writePointer = (buffer->writePointer + 1) % buffer->length;
            return temp;
        }else{
            return temp;
        }
    }else{
        return bufferFull;
    }
}

/**
 * @brief read data from the beginning of the buffer
 * @param   buffer  pointer to a buffer_t instance
 * @param   data    pointer to a variable where data should be stored
 * @return  a element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected
 * @retval  bufferNotInitialized    the bufferInit() method hasn't been called or failed before
 * @retval  bufferEmpty  the buffer is empty an no more data can be read
 */
bufferStatus_t bufferRead(buffer_t* buffer, bufferDatatype* data){
    if(!bufferIsEmpty(buffer)){
        bufferStatus_t temp = bufferReadFromIndex(buffer, buffer->readPointer, data);
        if(temp == bufferOK){
            buffer->datacount--;
            buffer->readPointer = (buffer->readPointer + 1) % buffer->length;
            return temp;
        }else{
            return temp;
        }
    }else{
        return bufferEmpty;
    }
}