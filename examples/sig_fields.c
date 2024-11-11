#include <stdio.h>

#include <cDBC.h>

int main (int argc, char * argv[])
{
    cDBC * DBC = NULL;
    char msg_name[] = "AutopilotStatus";
    char sig_name[] = "DAS_visionOnlySpeedLimit";
    char str_tmp[128] = "";
    int i_tmp = 0;
    double d_tmp = 0.0;

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

    printf ("Finding signal by name (\"%s\"): ", sig_name);
    if (cDBC_operation(DBC, FIND_SIGNAL_BY_NAME, sig_name) == 0)
        printf ("success\n\n");
    else printf ("unsuccessfully\n\n");

    printf ("Getting signal name: ");
    if (cDBC_operation(DBC, GET_SIGNAL_NAME, str_tmp) == 0)
    {
        printf ("success\n");
        printf ("Signal name: %s\n\n", str_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting signal multiplexer indicator: ");
    if (cDBC_operation(DBC, GET_SIGNAL_MULTIPLEXER_INDICATOR, str_tmp) == 0)
    {
        printf ("success\n");
        printf ("Multiplexer indicator: %s\n\n", str_tmp);
    }
    else printf ("unsuccessfully\n\n");
    
    printf ("Getting signal bit start: ");
    if (cDBC_operation(DBC, GET_SIGNAL_BIT_START, &i_tmp) == 0)
    {
        printf ("success\n");
        printf ("Bit start: %hd\n\n", i_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting signal size: ");
    if (cDBC_operation(DBC, GET_SIGNAL_SIZE, &i_tmp) == 0)
    {
        printf ("success\n");
        printf ("Signal size: %hd\n\n", i_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting byte order: ");
    if (cDBC_operation(DBC, GET_SIGNAL_BYTE_ORDER, &i_tmp) == 0)
    {
        printf ("success\n");
        printf ("Signal byte order: %hd\n\n", i_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting value type: ");
    if (cDBC_operation(DBC, GET_SIGNAL_VALUE_TYPE, &i_tmp) == 0)
    {
        printf ("success\n");
        printf ("Signal value type: %hd (%s)\n\n", i_tmp, i_tmp == 0 ? "unsigned" : "signed");
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting signal factor: ");
    if (cDBC_operation(DBC, GET_SIGNAL_FACTOR, &d_tmp) == 0)
    {
        printf ("success\n");
        printf ("Signal factor: %f\n\n", d_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting signal offset: ");
    if (cDBC_operation(DBC, GET_SIGNAL_OFFSET, &d_tmp) == 0)
    {
        printf ("success\n");
        printf ("Signal offset: %f\n\n", d_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting signal minimum: ");
    if (cDBC_operation(DBC, GET_SIGNAL_MINIMUM, &d_tmp) == 0)
    {
        printf ("success\n");
        printf ("Signal minimum: %f\n\n", d_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting signal maximum: ");
    if (cDBC_operation(DBC, GET_SIGNAL_MAXIMUM, &d_tmp) == 0)
    {
        printf ("success\n");
        printf ("Signal maximum: %f\n\n", d_tmp);
    }
    else printf ("unsuccessfully\n\n");

    printf ("Getting unit: ");
    if (cDBC_operation(DBC, GET_SIGNAL_UNIT, str_tmp) == 0)
    {
        printf ("success\n");
        printf ("Unit: %s\n\n", str_tmp);
    }
    else printf ("unsuccessfully\n\n");
    
    printf ("Getting receiver: ");
    if (cDBC_operation(DBC, GET_SIGNAL_RECEIVER, str_tmp) == 0)
    {
        printf ("success\n");
        printf ("Receiver: %s\n\n", str_tmp);
    }
    else printf ("unsuccessfully\n\n");

    cDBC_Deinit (&DBC);
    
    printf ("Done\n");

    printf ("Prees [Enter] to quit...");
    while (getchar () != '\n');

    return 0;
}
