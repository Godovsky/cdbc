#ifndef CDBC_H
#define CDBC_H

#include <stdint.h>

typedef struct cDBC cDBC;

enum {
    FUNC_ERR = -1,
    FUNC_OK
};

enum {
    BIG_ENDIAN,
    LITTLE_ENDIAN
};

enum {
    UNSIGNED = '+',
    SIGNED = '-'
};

/* Listing the available operations on messages */
typedef enum functions {
    /* Printing all the messages. Returns nothing. Example:
    ```cDBC_operation(cdbc, FUNC_PRINT_ALL, NULL)``` */
    FUNC_PRINT_ALL,

    /* Printing the current message. Returns nothing. Example:
    ```cDBC_operation(cdbc, FUNC_PRINT_ONE, NULL)``` */
    FUNC_PRINT_ONE,



    /* Reading a file. Example:
    ```cDBC_operation(cdbc, FUNC_READ_FILE, "tesla_can.dbc")``` */
    FUNC_READ_FILE,

    /* Find the message by the entered name. Example:
    ```char msgName[] = "DAS_steeringControl";```
    ```if (cDBC_operation(cdbc, FUNC_FIND_MESSAGE_BY_NAME, msgName) == FUNC_OK) {```
    ```int id = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_ID, &id) == FUNC_OK) {```
    ```printf("%d\n", id);```
    ```}```
    ```}``` */
    FUNC_FIND_MESSAGE_BY_NAME,

    /* Find the message by the entered id. Example:
    ```int id = 1160;```
    ```if (cDBC_operation(cdbc, FUNC_FIND_MESSAGE_BY_ID, &id) == FUNC_OK) {```
    ```char msgName[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_NAME, msgName) == FUNC_OK) {```
    ```printf("%s\n", msgName);```
    ```}```
    ```}``` */
    FUNC_FIND_MESSAGE_BY_ID,

    /* Getting the name of the current message. Example:
    ```char msgName[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_NAME, msgName) == FUNC_OK) {```
    ```printf("%s\n", msgName);```
    ```}``` */
    FUNC_GET_MESSAGE_NAME,

    /* Getting the ID of the current message. Example:
    ```int id = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_ID, &id) == FUNC_OK) {```
    ```printf("%d\n", id);```
    ```}``` */
    FUNC_GET_MESSAGE_ID,



    /* Getting the size of the current message. Example:
    ```int size = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_SIZE, &size) == FUNC_OK) {```
    ```printf("%d\n", size);```
    ```}``` */
    FUNC_GET_MESSAGE_SIZE,

    /* Getting the transmitter of the current message. Example:
    ```char transmitter[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_TRANSMITTER, transmitter) == FUNC_OK) {```
    ```printf("%s\n", transmitter);```
    ```}``` */
    FUNC_GET_MESSAGE_TRANSMITTER,

    /* Getting the number of the messages. Example:
    ```int num = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_NUMBER_OF_MESSAGES, &num) == FUNC_OK) {```
    ```printf("%d\n", num);```
    ```}``` */
    FUNC_GET_NUMBER_OF_MESSAGES,

    /* Shifts the position to the next message. Example:
    ```if (cDBC_operation(cdbc, FUNC_NEXT_MESSAGE, NULL) == FUNC_OK) {```
    ```int id = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_ID, &id) == FUNC_OK) {```
    ```printf("%d\n", id);```
    ```}```
    ```}``` */
    FUNC_NEXT_MESSAGE,

    /* Shifts the position to the previous message. Example:
    ```if (cDBC_operation(cdbc, FUNC_PREVIOUS_MESSAGE, NULL) == FUNC_OK) {```
    ```int id = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_ID, &id) == FUNC_OK) {```
    ```printf("%d\n", id);```
    ```}```
    ```}``` */
    FUNC_PREVIOUS_MESSAGE,

    /* Sets the position to the first message. Example:
    ```if (cDBC_operation(cdbc, FUNC_RESET_MESSAGE_POSITION, NULL) == FUNC_OK) {```
    ```int id = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_MESSAGE_ID, &id) == FUNC_OK) {```
    ```printf("%d\n", id);```
    ```}```
    ```}``` */
    FUNC_RESET_MESSAGE_POSITION,



    /* Find the signal of the current message by the entered name. Example:
    ```char sigName[] = "DAS_steeringControlType";```
    ```if (cDBC_operation(cdbc, FUNC_FIND_SIGNAL_BY_NAME, sigName) == FUNC_OK) {```
    ```double factor = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_FACTOR, &factor) == FUNC_OK) {```
    ```printf("%lf\n", factor);```
    ```}```
    ```}``` */
    FUNC_FIND_SIGNAL_BY_NAME,

    /* Getting the name of the signal of the current message. Example:
    ```char sigName[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_NAME, sigName) == FUNC_OK) {```
    ```printf("%s\n", sigName);```
    ```}``` */
    FUNC_GET_SIGNAL_NAME,

    /* Getting the multiplexer indicator of the signal of the current message. Example:
    ```char mulInd[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_MULTIPLEXER_INDICATOR, mulInd) == FUNC_OK) {```
    ```printf("%s\n", mulInd);```
    ```}``` */
    FUNC_GET_SIGNAL_MULTIPLEXER_INDICATOR,

    /* Getting the "bit start" parameter of the signal of the current message. Example:
    ```int bitstart = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_BIT_START, &bitstart) == FUNC_OK) {```
    ```printf("%d\n", bitstart);```
    ```}``` */
    FUNC_GET_SIGNAL_BIT_START,

    /* Getting the size of the signal of the current message. Example:
    ```int size = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_LENGTH, &size) == FUNC_OK) {```
    ```printf("%d\n", size);```
    ```}``` */
    FUNC_GET_SIGNAL_LENGTH,

    /* Getting the byte order of the signal of the current message. Example:
    ```int byteOrder = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_BYTE_ORDER, &byteOrder) == FUNC_OK) {```
    ```if (byteOrder == BIG_ENDIAN) {```
    ```printf("%s\n", "BIG ENDIAN");```
    ```} else if (byteOrder == LITTLE_ENDIAN) {```
    ```printf("%s\n", "LITTLE ENDIAN");```
    ```}
    ```}``` */
    FUNC_GET_SIGNAL_BYTE_ORDER,

    /* Getting the value type of the signal of the current message. Example:
    ```int valueType = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_VALUE_TYPE, &valueType) == FUNC_OK) {```
    ```if (valueType == UNSIGNED) {```
    ```printf("%s\n", "UNSIGNED");```
    ```} else if (valueType == SIGNED) {```
    ```printf("%s\n", "SIGNED");```
    ```}
    ```}``` */
    FUNC_GET_SIGNAL_VALUE_TYPE,

    /* Getting the factor of the signal of the current message. Example:
    ```double factor = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_FACTOR, &factor) == FUNC_OK) {```
    ```printf("%lf\n", factor);```
    ```}``` */
    FUNC_GET_SIGNAL_FACTOR,

    /* Getting the offset of the signal of the current message. Example:
    ```double offset = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_OFFSET, &offset) == FUNC_OK) {```
    ```printf("%lf\n", offset);```
    ```}``` */
    FUNC_GET_SIGNAL_OFFSET,

    /* Getting the minimum value of the signal of the current message. Example:
    ```double min = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_MINIMUM, &min) == FUNC_OK) {```
    ```printf("%lf\n", min);```
    ```}``` */
    FUNC_GET_SIGNAL_MINIMUM,

    /* Getting the maximum value of the signal of the current message. Example:
    ```double max = 0;```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_MAXIMUM, &max) == FUNC_OK) {```
    ```printf("%lf\n", max);```
    ```}``` */
    FUNC_GET_SIGNAL_MAXIMUM,

    /* Getting the unit of the signal of the current message. Example:
    ```char unit[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_UNIT, unit) == FUNC_OK) {```
    ```printf("%s\n", unit);```
    ```}``` */
    FUNC_GET_SIGNAL_UNIT,

    /* Getting the receiver of the signal of the current message. Example:
    ```char receiver[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_RECEIVER, receiver) == FUNC_OK) {```
    ```printf("%s\n", receiver);```
    ```}``` */
    FUNC_GET_SIGNAL_RECEIVER,

    /* Getting the number of the signals of the current message. Example:
    ```int num = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_NUMBER_OF_SIGNALS, &num) == FUNC_OK) {```
    ```printf("%d\n", num);```
    ```}``` */
    FUNC_GET_NUMBER_OF_SIGNALS,

    /* Shifts the position to the next signal of the current message. Example:
    ```if (cDBC_operation(cdbc, FUNC_NEXT_SIGNAL, NULL) == FUNC_OK) {```
    ```char sigName[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_NAME, sigName) == FUNC_OK) {```
    ```printf("%s\n", sigName);```
    ```}```
    ```}``` */
    FUNC_NEXT_SIGNAL,

    /* Shifts the position to the previous signal of the current message. Example:
    ```if (cDBC_operation(cdbc, FUNC_NEXT_SIGNAL, NULL) == FUNC_OK) {```
    ```char sigName[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_NAME, sigName) == FUNC_OK) {```
    ```printf("%s\n", sigName);```
    ```}```
    ```}``` */
    FUNC_PREVIOUS_SIGNAL,

    /* Sets the position at the beginnig of the signals of the current message. Example:
    ```if (cDBC_operation(cdbc, FUNC_RESET_SIGNAL_POSITION, NULL) == FUNC_OK) {```
    ```char sigName[128] = "";```
    ```if (cDBC_operation(cdbc, FUNC_GET_SIGNAL_NAME, sigName) == FUNC_OK) {```
    ```printf("%s\n", sigName);```
    ```}```
    ```}``` */
    FUNC_RESET_SIGNAL_POSITION
} FUNCTION;

int cDBC_Init(cDBC **);
int cDBC_operation(cDBC *, FUNCTION, void *);
void cDBC_Deinit(cDBC **);

#endif /* CDBC_H */