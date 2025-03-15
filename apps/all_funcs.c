#include <stdio.h>
#include <string.h>

#include <cDBC.h>

int main(int argc, char * argv[]) {
    cDBC * DBC = NULL;
    char * name = "ESP_B";
    char str_tmp[128] = "";
    double d_tmp = 0.0;
    int i_tmp = 0;

    printf("Init: ");
    if (cDBC_Init(&DBC)) {
        printf("Unsuccessful\n");
        return 1;
    } else printf("success\n");

    printf("File reading: ");
    if (cDBC_operation(DBC, FUNC_READ_FILE, "tesla_can.dbc") == FUNC_OK)
        printf("success\n");

    printf("------------------------------------------------------------\n");

    cDBC_operation(DBC, FUNC_PRINT_ALL, NULL);       /* Always returns zero */

    printf("------------------------------------------------------------\n");

    printf("Finding message by name (\"%s\"): ", name);
    if (cDBC_operation(DBC, FUNC_FIND_MESSAGE_BY_NAME, name) == FUNC_OK) {
        printf("found\n");
        cDBC_operation(DBC, FUNC_PRINT_ONE, NULL);   /* Always returns zero */
    } else printf("not found\n");

    printf("------------------------------------------------------------\n");

    i_tmp = 792;
    printf("Finding message by id (%d): ", i_tmp);
    if (cDBC_operation(DBC, FUNC_FIND_MESSAGE_BY_ID, &i_tmp) == FUNC_OK) {
        printf("found\n");
        cDBC_operation(DBC, FUNC_PRINT_ONE, NULL);   /* Always returns zero */
    } else printf("not found\n");

    printf("------------------------------------------------------------\n");

    printf("Getting message name: ");
    if (cDBC_operation(DBC, FUNC_GET_MESSAGE_NAME, str_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Message name: %s\n\n", str_tmp);
    } else printf("unsuccessfully\n");

    printf("Getting message ID: ");
    if (cDBC_operation(DBC, FUNC_GET_MESSAGE_ID, &i_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Message ID: %d\n\n", i_tmp);
    } else printf("unsuccessfully\n");

    printf("Getting message size: ");
    if (cDBC_operation(DBC, FUNC_GET_MESSAGE_SIZE, &i_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Message size: %d\n\n", i_tmp);
    } else printf("unsuccessfully\n");

    printf("Getting message transmitter: ");
    if (cDBC_operation(DBC, FUNC_GET_MESSAGE_TRANSMITTER, str_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Transmitter: %s\n\n", str_tmp);
    } else printf("unsuccessfully\n\n");



    printf("Finding signal by name: ");
    if (cDBC_operation(DBC, FUNC_FIND_SIGNAL_BY_NAME, "DOOR_STATE_RL") == FUNC_OK)
        printf("success\n\n");
    else printf("unsuccessfully\n\n");

    printf("Getting signal name: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_NAME, str_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Signal name: %s\n\n", str_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting signal multiplexer indicator: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_MULTIPLEXER_INDICATOR, str_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Multiplexer indicator: %s\n\n", str_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting signal bit start: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_BIT_START, &i_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Bit start: %hd\n\n", i_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting signal size: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_LENGTH, &i_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Signal size: %hd\n\n", i_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting byte order: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_BYTE_ORDER, &i_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Signal byte order: %hd\n\n", i_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting value type: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_VALUE_TYPE, &i_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Signal value type: %hd (%s)\n\n", i_tmp, i_tmp == FUNC_OK ? "unsigned" : "signed");
    } else printf("unsuccessfully\n\n");

    printf("Getting signal factor: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_FACTOR, &d_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Signal factor: %f\n\n", d_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting signal offset: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_OFFSET, &d_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Signal offset: %f\n\n", d_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting signal minimum: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_MINIMUM, &d_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Signal minimum: %f\n\n", d_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting signal maximum: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_MAXIMUM, &d_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Signal maximum: %f\n\n", d_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting unit: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_UNIT, str_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Unit: %s\n\n", str_tmp);
    } else printf("unsuccessfully\n\n");

    printf("Getting receiver: ");
    if (cDBC_operation(DBC, FUNC_GET_SIGNAL_RECEIVER, str_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Receiver: %s\n\n", str_tmp);
    } else printf("unsuccessfully\n\n");

    printf("------------------------------------------------------------\n");

    printf("First message: ");
    if (cDBC_operation(DBC, FUNC_RESET_MESSAGE_POSITION, NULL) == FUNC_OK) {
        printf("success\n");
        printf("  Getting message name: ");
        if (cDBC_operation(DBC, FUNC_GET_MESSAGE_NAME, str_tmp) == FUNC_OK) {
            printf("  success\n");
            printf("  Message name: %s\n\n", str_tmp);
        } else printf("  unsuccessfully\n");
    } else printf("unsuccessfully\n\n");

    printf("Previous message: ");
    if (cDBC_operation(DBC, FUNC_PREVIOUS_MESSAGE, NULL) == FUNC_OK) {
        printf("success\n");
        printf("  Getting message name: ");
        if (cDBC_operation(DBC, FUNC_GET_MESSAGE_NAME, str_tmp) == FUNC_OK) {
            printf("  success\n");
            printf("  Message name: %s\n\n", str_tmp);
        } else printf("  unsuccessfully\n");
    } else printf("unsuccessfully\n\n");

    printf("Next message: ");
    if (cDBC_operation(DBC, FUNC_NEXT_MESSAGE, NULL) == FUNC_OK) {
        printf("success\n");
        printf("  Getting message name: ");
        if (cDBC_operation(DBC, FUNC_GET_MESSAGE_NAME, str_tmp) == FUNC_OK) {
            printf("  success\n");
            printf("  Message name: %s\n\n", str_tmp);
        } else printf("  unsuccessfully\n");
    } else printf("unsuccessfully\n\n");

    printf("Previous signal: ");
    if (cDBC_operation(DBC, FUNC_PREVIOUS_SIGNAL, NULL) == FUNC_OK) {
        printf("success\n");
        printf("  Getting signal name: ");
        if (cDBC_operation(DBC, FUNC_GET_SIGNAL_NAME, str_tmp) == FUNC_OK) {
            printf("  success\n");
            printf("  Signal name: %s\n\n", str_tmp);
        } else printf("  unsuccessfully\n");
    } else printf("unsuccessfully\n\n");

    printf("Next signal: ");
    if (cDBC_operation(DBC, FUNC_NEXT_SIGNAL, NULL) == FUNC_OK) {
        printf("success\n");
        printf("  Getting signal name: ");
        if (cDBC_operation(DBC, FUNC_GET_SIGNAL_NAME, str_tmp) == FUNC_OK) {
            printf("  success\n");
            printf("  Signal name: %s\n\n", str_tmp);
        } else printf("  unsuccessfully\n");
    } else printf("unsuccessfully\n\n");

    printf("First signal: ");
    if (cDBC_operation(DBC, FUNC_RESET_SIGNAL_POSITION, NULL) == FUNC_OK) {
        printf("success\n");
        printf("  Getting signal name: ");
        if (cDBC_operation(DBC, FUNC_GET_SIGNAL_NAME, str_tmp) == FUNC_OK) {
            printf("  success\n");
            printf("  Signal name: %s\n\n", str_tmp);
        } else printf("  unsuccessfully\n");
    } else printf("unsuccessfully\n\n");

    printf("Getting number of messages: ");
    if (cDBC_operation(DBC, FUNC_GET_NUMBER_OF_MESSAGES, &i_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Number of messages: %d\n\n", i_tmp);
    } else printf("unsuccessfully\n");

    printf("Getting number of current message's signals: ");
    if (cDBC_operation(DBC, FUNC_GET_NUMBER_OF_SIGNALS, &i_tmp) == FUNC_OK) {
        printf("success\n");
        printf("Number of current message's signals: %d\n\n", i_tmp);
    } else printf("unsuccessfully\n");

    cDBC_Deinit(&DBC);

    printf("Done\n");

    printf("Prees [Enter] to quit...");
    while (getchar() != '\n');

    return 0;
}
