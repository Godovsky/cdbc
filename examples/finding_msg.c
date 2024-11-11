#include <stdio.h>

#include <cDBC.h>

int main (int argc, char * argv[])
{
    cDBC * DBC = NULL;
    char msg_name[] = "AutopilotStatus";
    int id = 824;

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

    printf ("Finding message by ID (%d): ", id);
    if (cDBC_operation(DBC, FIND_MESSAGE_BY_ID, &id) == 0)
    {
        printf ("found\n");
        cDBC_operation(DBC, PRINT_ONE, NULL);   /* Always returns zero */
    }
    else printf ("not found\n");

    cDBC_Deinit (&DBC);
    
    printf ("Done\n");

    printf ("Prees [Enter] to quit...");
    while (getchar () != '\n');

    return 0;
}
