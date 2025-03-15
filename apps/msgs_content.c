#include <stdio.h>

#include <cDBC.h>

int main(int argc, char * argv[]) {
    cDBC * DBC = NULL;
    char str_tmp[128] = "";
    int num_of_msgs = 0;
    int num_of_sigs = 0;
    int i, j;

    printf("Init: ");
    if (cDBC_Init(&DBC)) {
        printf("Unsuccessful\n");
        return 1;
    } else printf("success\n");

    printf("File reading: ");
    if (cDBC_operation(DBC, FUNC_READ_FILE, "tesla_can.dbc") == FUNC_OK)
        printf("success\n");

    printf("Getting number of messages: ");
    if (cDBC_operation(DBC, FUNC_GET_NUMBER_OF_MESSAGES, &num_of_msgs) == FUNC_OK) {
        printf("success\n");
        printf("  Number of messages: %d\n\n", num_of_msgs);
    } else printf("unsuccessfully\n");

    for (i = 0; i < num_of_msgs; i++) {
        if (cDBC_operation(DBC, FUNC_GET_MESSAGE_NAME, str_tmp) == FUNC_OK)
            printf("%d Message name: %s\n", i + 1, str_tmp);

        if (cDBC_operation(DBC, FUNC_GET_NUMBER_OF_SIGNALS, &num_of_sigs) == FUNC_OK) {
            for (j = 0; j < num_of_sigs; j++) {
                if (cDBC_operation(DBC, FUNC_GET_SIGNAL_NAME, str_tmp) == FUNC_OK)
                    printf("  %d Signal name: %s\n", j + 1, str_tmp);

                if (cDBC_operation(DBC, FUNC_NEXT_SIGNAL, NULL) != 0)
                    printf("Unsuccessfully the next signal getting\n");
            }
            printf("\n");
        }

        if (cDBC_operation(DBC, FUNC_NEXT_MESSAGE, NULL) != 0)
            printf("Unsuccessfully the next message getting\n");
    }

    cDBC_Deinit(&DBC);

    printf("Done\n");

    printf("Prees [Enter] to quit...");
    while (getchar() != '\n');

    return 0;
}
