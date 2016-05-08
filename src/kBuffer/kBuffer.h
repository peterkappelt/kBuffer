/**
 * @file kBuffer.h
 * @author Peter Kappelt
 * @date xxx
 * @brief A universal ringbuffer library
 * A (more or less) universal ringbuffer library, suitable (at least) for x86 and AVR
 * @see https://github.com/peterkappelt/kBuffer
 */

#ifndef KBUFFER_H
#define KBUFFER_H

#include <stdint.h>

#define bufferDatatype uint8_t

/**
 * @brief Struct for buffer handling.
 * If you need a ringbuffer in your software, you should instantiate a buffer_t, and run the neccessary functions with a pointer to your instance
 */
typedef struct{
    /// is 0 if the buffer is not initialized
    uint8_t isInitialized;
    /// The write pointer of the buffer. At a write procedure, data gets written and the pointer is incremented
    uint16_t writePointer;
    /// The read pointer of the buffer. At a read procedure, data gets read and the pointer is incremented
    uint16_t readPointer;
    /// The number of elements in the buffer
    uint16_t length;
    /// The number of bytes of one buffer element. The total memory consumption in Bytes is equal to length * elementLength
    uint8_t elementLength;  
    /// A variable which is increased by one when new data gets written and decremented by one when data is read
    uint16_t datacount;
    /// A pointer to the first element of the buffer. Length * elementLength bytes of memory are allocated after this pointer
    bufferDatatype* data;   
}buffer_t;

/**
 * @enum bufferStatus_t
 * @brief buffer function return codes
 */
typedef enum{
    /** it seems, as everything worked as expected */
    bufferOK = 0,
    /** happens while allocating memory,<br>there is not enough free memory (->malloc failed) */
    bufferMemoryAllocationFailed,
    /** happens at reading data,<br>buffer is empty and there is no more data to read */
    bufferEmpty,
    /** happens at writing data,<br>buffer is full, no more data can be written */
    bufferFull
}bufferStatus_t;


//Function declearations

bufferStatus_t bufferInit(buffer_t* buffer, uint16_t bufferSize);

#endif