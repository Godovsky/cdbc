#include <stdio.h>

#include <cDBC.h>

int main (int argc, char * argv[])
{
    cDBC * DBC = NULL;
    char msg_name[] = "AutopilotStatus";
    char str_tmp[128] = "";
    int i_tmp = 0;

    printf ("Init: ");
    if (cDBC_Init (&DBC))
    {
        printf ("Unsuccessful\n");
        return 1;
    }
    else printf ("success\n");

    printf ("File reading: ");
    if (cDBC_operation(DBC, READ_FILE, "tesla_can.dbc") == 0)
        printf ("success\n");

    printf ("Finding message by name (\"%s\"): ", msg_name);
    if (cDBC_operation(DBC, FIND_MESSAGE_BY_NAME, msg_name) == 0)
        printf ("found\n");
    else printf ("not found\n");

    printf ("Getting message name: ");
    if (cDBC_operation(DBC, GET_MESSAGE_NAME, str_tmp) == 0)
    {
        printf ("success\n");
        printf ("  Message name: %s\n\n", str_tmp);
    }
    else printf ("unsuccessfully\n");
        
    printf ("Getting message ID: ");
    if (cDBC_operation(DBC, GET_MESSAGE_ID, &i_tmp) == 0)
    {
        printf ("success\n");
        printf ("  Message ID: %d\n\n", i_tmp);
    }
    else printf ("unsuccessfully\n");
    
    printf ("Getting message size: ");
    if (cDBC_operation(DBC, GET_MESSAGE_SIZE, &i_tmp) == 0)
    {
        printf ("success\n");
        printf ("  Message size: %d\n\n", i_tmp);
    }
    else printf ("unsuccessfully\n");

    printf ("Getting message transmitter: ");
    if (cDBC_operation(DBC, GET_MESSAGE_TRANSMITTER, str_tmp) == 0)
    {
        printf ("success\n");
        printf ("  Transmitter: %s\n\n", str_tmp);
    }
    else printf ("unsuccessfully\n\n");

    cDBC_Deinit (&DBC);
    
    printf ("Done\n");

    printf ("Prees [Enter] to quit...");
    while (getchar () != '\n');

    return 0;
}
