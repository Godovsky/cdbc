#ifndef CDBC_H
#define CDBC_H

#include <stdint.h>

typedef struct cDBC cDBC;

typedef enum functions
{
    PRINT_ALL,
    PRINT_ONE,

    READ_FILE,
    
    FIND_MESSAGE_BY_NAME,
    FIND_MESSAGE_BY_ID,
    GET_MESSAGE_SIZE,
    GET_MESSAGE_TRANSMITTER,
    GET_MESSAGE_NAME,
    GET_MESSAGE_ID,

    FIND_SIGNAL_BY_NAME,
    GET_SIGNAL_MULTIPLEXER_INDICATOR,
    GET_SIGNAL_NAME,
    GET_SIGNAL_BIT_START,
    GET_SIGNAL_SIZE,
    GET_SIGNAL_BYTE_ORDER,
    GET_SIGNAL_VALUE_TYPE,
    GET_SIGNAL_FACTOR,
    GET_SIGNAL_OFFSET,
    GET_SIGNAL_MINIMUM,
    GET_SIGNAL_MAXIMUM,
    GET_SIGNAL_UNIT,
    GET_SIGNAL_RECEIVER
} FUNCTION;

int cDBC_Init (cDBC **);
int cDBC_operation (cDBC *, FUNCTION, void *);
void cDBC_Deinit (cDBC **);


#endif