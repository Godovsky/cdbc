#include <stdio.h>

#include <cDBC.h>

int main (int argc, char * argv[])
{
    cDBC * DBC = NULL;
    char msg_name[] = "AutopilotStatus";
    char sig_name[] = "DAS_summonObstacle";
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
    {
        printf ("found\n");
        cDBC_operation(DBC, PRINT_ONE, NULL);   /* Always returns zero */
    }
    else printf ("not found\n");

    printf ("\n");

    printf ("Finding signal by name (%s): ", sig_name);
    if (cDBC_operation(DBC, FIND_SIGNAL_BY_NAME, sig_name) == 0)
    {
        printf ("found\n");
    }
    else printf ("not found\n");

    printf ("Getting signal name: ");
    if (cDBC_operation(DBC, GET_SIGNAL_NAME, str_tmp) == 0)
    {
        printf ("success\n");
        printf ("  Signal name: %s\n\n", str_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting signal size: ");
    if (cDBC_operation(DBC, GET_SIGNAL_SIZE, &i_tmp) == 0)
    {
        printf ("success\n");
        printf ("  Signal size: %hd\n\n", i_tmp);
    }
    else printf ("unsuccessfully\n\n");

    cDBC_Deinit (&DBC);
    
    printf ("Done\n");

    printf ("Prees [Enter] to quit...");
    while (getchar () != '\n');

    return 0;
}
