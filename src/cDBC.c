/* Copyright (c) 2025 Godov Andrey <andygodov@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cDBC.h>

#define LINE_LENGHT 1024

typedef struct _S S;
typedef struct _M M;

struct _S {
    char signal_name[128];
    char multiplexer_indicator[128];
    unsigned short bit_start;
    unsigned short signal_size;
    unsigned short byte_order;
    unsigned char value_type;
    double factor;
    double offset;
    double minimum;
    double maximum;
    char unit[32];
    char receiver[32];
    S * next;
    S * prev;
};

struct _M {
    int number_of_signals;
    unsigned int message_id;
    char message_name[128];
    unsigned char message_size;
    char transmitter[128];
    S * Signals_begin;
    S * Signals;
    M * next;
    M * prev;
};

typedef struct _D {
    int number_of_mesages;
    M * Messages_begin;
    M * Messages;
} D;

static void cDBC_print_all(cDBC * p_cDBC) {
    D * Tmp = (D *)p_cDBC;
    M * Mes = NULL;
    S * Sig = NULL;

    if (NULL == Tmp) {
        printf("cDBC isn't init\n");
        return;
    }

    Mes = Tmp->Messages_begin;

    if (Mes)
        do {
            printf("BO_ %u %s: %c %s\n",
                Mes->message_id,
                Mes->message_name,
                Mes->message_size,
                Mes->transmitter);

            Sig = Mes->Signals_begin;
            if (Sig)
                do {
                    printf(" SG_ %s %s : %hu|%hu@%hu%c (%f,%f) [%f|%f] %s %s\n",
                        Sig->signal_name,
                        Sig->multiplexer_indicator[0] != '\0' ? Sig->multiplexer_indicator : "\b",
                        Sig->bit_start,
                        Sig->signal_size,
                        Sig->byte_order,
                        Sig->value_type,
                        Sig->factor,
                        Sig->offset,
                        Sig->minimum,
                        Sig->maximum,
                        Sig->unit,
                        Sig->receiver);

                    Sig = Sig->next;
                } while (Sig != Mes->Signals);

            printf("\n");

            Mes = Mes->next;
        } while (Mes != Tmp->Messages_begin);
}

static void cDBC_print_one(cDBC * p_cDBC) {
    D * Tmp = (D *)p_cDBC;
    M * Mes = NULL;
    S * Sig = NULL;

    if (NULL == Tmp) {
        printf("cDBC isn't init\n");
        return;
    }

    Mes = Tmp->Messages;

    if (Mes) {
        printf("BO_ %u %s: %c %s\n",
            Mes->message_id,
            Mes->message_name,
            Mes->message_size,
            Mes->transmitter);

        Sig = Mes->Signals_begin;
        if (Sig)
            do {
                printf(" SG_ %s %s : %hu|%hu@%hu%c (%f,%f) [%f|%f] %s %s\n",
                    Sig->signal_name,
                    Sig->multiplexer_indicator[0] != '\0' ? Sig->multiplexer_indicator : "\b",
                    Sig->bit_start,
                    Sig->signal_size,
                    Sig->byte_order,
                    Sig->value_type,
                    Sig->factor,
                    Sig->offset,
                    Sig->minimum,
                    Sig->maximum,
                    Sig->unit,
                    Sig->receiver);

                Sig = Sig->next;
            } while (Sig != Mes->Signals_begin);
    }
}

static void cDBC_sig_free(S * p_Sig) {
    S * Sig_begin = p_Sig;
    if (Sig_begin) {
        do {
            S ** Sig_tmp = &p_Sig;
            p_Sig = p_Sig->next;
            if (*Sig_tmp)
                free(*Sig_tmp);
            *Sig_tmp = NULL;
        } while (p_Sig != Sig_begin && p_Sig != NULL);
    }
}

int convert_start_bit(int start_bit, int num_bytes) {
    int byte_index = start_bit / 8;
    int bit_index = start_bit % 8;
    return (num_bytes - 1 - byte_index) * 8 + (7 - bit_index);
}

static int cDBC_read_file(cDBC * p_cDBC, const char * path) {
    D * Tmp = (D *)p_cDBC;

    M * Mes = NULL;
    M * Mes_head = NULL;
    M Mes_tmp = { 0 };
    S * Sig = NULL;
    S * Sig_head = NULL;
    S Sig_tmp;

    char tmp_str[128] = "";
    int sscanf_res = 0;

    char str[LINE_LENGHT] = "";

    FILE * fp = NULL;

    if (NULL == Tmp) {
        printf("cDBC isn't init\n");
        return FUNC_ERR;
    }

    fp = fopen(path, "r");
    if (NULL == fp) {
        printf("Unsuccessful file opening!\n");
        return FUNC_ERR;
    }

    Tmp->number_of_mesages = 0;

    while (fgets(str, LINE_LENGHT, fp)) {
        if (strncmp(str, "BO_ ", 4) == 0) {
            sscanf_res = sscanf(str, "%*[^0-9]%u%*[ \t]%[^:]%*[: \t]%c%*[ \t]%[^ \t\n]",
                &Mes_tmp.message_id,
                Mes_tmp.message_name,
                &Mes_tmp.message_size,
                Mes_tmp.transmitter);

            if (sscanf_res == 3 || sscanf_res == 4) {
                if (NULL == Mes_head) {
                    Mes = malloc(sizeof(M));
                    if (NULL == Mes) {
                        printf("cDBC_read_file: memory allocation for message error!\n");
                        if (Mes_head)
                            cDBC_Deinit(&p_cDBC);
                        fclose(fp);
                        return FUNC_ERR;
                    }
                    Mes->Signals = NULL;
                    if (NULL == Mes_head)
                        Mes_head = Mes;
                    memcpy(Mes, &Mes_tmp, sizeof(M));
                    Mes->next = Mes_head;
                    Mes->prev = Mes_head;
                } else {
                    M * Mtmp = Mes;
                    Mes->next = malloc(sizeof(M));
                    Mes = Mes->next;
                    if (NULL == Mes) {
                        printf("cDBC_read_file: memory allocation for message error!\n");
                        if (Mes_head)
                            cDBC_Deinit(&p_cDBC);
                        fclose(fp);
                        return FUNC_ERR;
                    }
                    memcpy(Mes, &Mes_tmp, sizeof(M));
                    Mes->next = Mes_head;
                    Mes->prev = Mtmp;
                    Mes->Signals = NULL;
                }
                Tmp->number_of_mesages++;
                Mes->number_of_signals = 0;

                Sig = NULL;
                Sig_head = NULL;
                while (fgets(str, LINE_LENGHT, fp)) {
                    if (strncmp(str, " SG_ ", 5) == 0) {
                        sscanf_res = sscanf(str, "%*[ \t]%*[SG_]%*[ \t]%[^:]%*[ :\t]%hu|%hu@%hu%c%*[ \t](%lf,%lf)%*[ \t][%lf|%lf]%*[ \t]%[^ \t]%*[ \t]%s",
                            tmp_str,
                            &Sig_tmp.bit_start,
                            &Sig_tmp.signal_size,
                            &Sig_tmp.byte_order,
                            &Sig_tmp.value_type,
                            &Sig_tmp.factor,
                            &Sig_tmp.offset,
                            &Sig_tmp.minimum,
                            &Sig_tmp.maximum,
                            Sig_tmp.unit,
                            Sig_tmp.receiver);
                        if (sscanf_res == 11) {
                            sscanf(tmp_str, "%[^: \t]%*[: \t]%[^: \t]%*[: \t]", Sig_tmp.signal_name, Sig_tmp.multiplexer_indicator);
                            tmp_str[0] = '\0';

                            if (NULL == Sig_head) {
                                Sig = malloc(sizeof(S));
                                if (NULL == Sig) {
                                    printf("cDBC_read_file: memory allocation for signal failed!\n");
                                    if (Sig_head)
                                        cDBC_sig_free(Sig_head);
                                    fclose(fp);
                                    return FUNC_ERR;
                                }
                                if (NULL == Sig_head)
                                    Sig_head = Sig;
                                memcpy(Sig, &Sig_tmp, sizeof(S));
                                Sig->next = Sig_head;
                                Sig->prev = Sig_head;
                            } else {
                                S * Stmp = Sig;
                                Sig->next = malloc(sizeof(S));
                                Sig = Sig->next;
                                if (NULL == Sig) {
                                    printf("cDBC_read_file: memory allocation for signal failed!\n");
                                    if (Sig_head)
                                        cDBC_sig_free(Sig_head);
                                    fclose(fp);
                                    return FUNC_ERR;
                                }
                                memcpy(Sig, &Sig_tmp, sizeof(S));
                                Sig->next = Sig_head;
                                Sig->prev = Stmp;
                            }
                            Mes->number_of_signals++;

                            memset(&Sig_tmp, 0, sizeof(S));
                        } else
                            printf("Signal syntax error (Message: %s)\n", Mes->message_name);
                    } else
                        break;
                }

                Sig_head->prev = Sig;
                Mes->Signals_begin = Sig_head;
                Mes->Signals = Sig_head;
            } else
                printf("Message syntax error\n");
        }
    }

    fclose(fp);

    Mes_head->prev = Mes;
    Tmp->Messages_begin = Mes_head;
    Tmp->Messages = Mes_head;

    return FUNC_OK;
}

static int cDBC_find_message_by_name(cDBC * p_cDBC, const char * name) {
    D * Tmp = (D *)p_cDBC;
    M * Mes_begin = NULL;

    if (NULL == Tmp) {
        printf("cDBC isn't init\n");
        return FUNC_ERR;
    }

    Mes_begin = Tmp->Messages;

    if (Mes_begin)
        do {
            if (strcmp(Tmp->Messages->message_name, name) == 0)
                return FUNC_OK;
            Tmp->Messages = Tmp->Messages->next;
        } while (Tmp->Messages != Mes_begin);

    return FUNC_ERR;
}

static int cDBC_find_message_by_id(cDBC * p_cDBC, const unsigned int id) {
    D * Tmp = (D *)p_cDBC;
    M * Mes_begin = NULL;

    if (NULL == Tmp) {
        printf("cDBC isn't init\n");
        return FUNC_ERR;
    }

    Mes_begin = Tmp->Messages;

    if (Tmp->Messages)
        do {
            if (Tmp->Messages->message_id == id)
                return FUNC_OK;

            Tmp->Messages = Tmp->Messages->next;
        } while (Tmp->Messages != Mes_begin);

    return FUNC_ERR;
}

static int cDBC_get_message_name(cDBC * p_cDBC, const char * str) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;

    if (NULL == strcpy((void *)str, Tmp->Messages->message_name))
        return FUNC_ERR;

    return FUNC_OK;
}

static int cDBC_get_message_id(cDBC * p_cDBC, int * id) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages || NULL == id)
        return FUNC_ERR;

    *id = Tmp->Messages->message_id;

    return FUNC_OK;
}

static int cDBC_get_message_size(cDBC * p_cDBC, int * size) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages || NULL == size)
        return FUNC_ERR;

    *size = Tmp->Messages->message_size - 48;

    return FUNC_OK;
}

static int cDBC_get_message_transmitter(cDBC * p_cDBC, const char * str) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;

    if (NULL == strcpy((void *)str, Tmp->Messages->transmitter))
        return FUNC_ERR;

    return FUNC_OK;
}

static int cDBC_get_number_of_messages(cDBC * p_cDBC, int * number_of_messages) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;

    *number_of_messages = Tmp->number_of_mesages;

    return FUNC_OK;
}

static int cDBC_next_message(cDBC * p_cDBC) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->next)
        return FUNC_ERR;

    Tmp->Messages = Tmp->Messages->next;

    return FUNC_OK;
}

static int cDBC_previous_message(cDBC * p_cDBC) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->prev)
        return FUNC_ERR;

    Tmp->Messages = Tmp->Messages->prev;

    return FUNC_OK;
}

static int cDBC_reset_message_position(cDBC * p_cDBC) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages_begin)
        return FUNC_ERR;

    Tmp->Messages = Tmp->Messages_begin;

    return FUNC_OK;
}

static int cDBC_find_signal_by_name(cDBC * p_cDBC, const char * name) {
    D * Tmp = (D *)p_cDBC;
    S * Sig_begin = NULL;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (Tmp->Messages) {
        Sig_begin = Tmp->Messages->Signals;
        if (Sig_begin)
            do {
                if (strcmp(Tmp->Messages->Signals->signal_name, name) == 0)
                    return FUNC_OK;

                Tmp->Messages->Signals = Tmp->Messages->Signals->next;
            } while (Tmp->Messages->Signals != Sig_begin);
    }

    return FUNC_ERR;
}

static int cDBC_get_signal_name(cDBC * p_cDBC, const char * str) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;

    if (NULL == strcpy((void *)str, Tmp->Messages->Signals->signal_name))
        return FUNC_ERR;

    return FUNC_OK;
}

static int cDBC_get_signal_multiplexer_indicator(cDBC * p_cDBC, const char * str) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;

    if (NULL == strcpy((void *)str, Tmp->Messages->Signals->multiplexer_indicator))
        return FUNC_ERR;

    return FUNC_OK;
}

static int cDBC_get_signal_bit_start(cDBC * p_cDBC, int * bit_start) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals || NULL == bit_start)
        return FUNC_ERR;

    *bit_start = (int)Tmp->Messages->Signals->bit_start;

    return FUNC_OK;
}

static int cDBC_get_signal_size(cDBC * p_cDBC, int * signal_size) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals || NULL == signal_size)
        return FUNC_ERR;

    *signal_size = (int)Tmp->Messages->Signals->signal_size;

    return FUNC_OK;
}

static int cDBC_get_signal_byte_order(cDBC * p_cDBC, int * byte_order) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals || NULL == byte_order)
        return FUNC_ERR;

    *byte_order = (int)Tmp->Messages->Signals->byte_order;

    return FUNC_OK;
}

static int cDBC_get_signal_value_type(cDBC * p_cDBC, char * value_type) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals || NULL == value_type)
        return FUNC_ERR;

    *value_type = Tmp->Messages->Signals->value_type;

    return FUNC_OK;
}

static int cDBC_get_signal_factor(cDBC * p_cDBC, double * factor) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;

    *factor = Tmp->Messages->Signals->factor;

    return FUNC_OK;
}

static int cDBC_get_signal_offset(cDBC * p_cDBC, double * offset) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;

    *offset = Tmp->Messages->Signals->offset;

    return FUNC_OK;
}

static int cDBC_get_signal_minimum(cDBC * p_cDBC, double * minimum) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;

    *minimum = Tmp->Messages->Signals->minimum;

    return FUNC_OK;
}

static int cDBC_get_signal_maximum(cDBC * p_cDBC, double * maximum) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;

    *maximum = Tmp->Messages->Signals->maximum;

    return FUNC_OK;
}

static int cDBC_get_signal_unit(cDBC * p_cDBC, const char * str) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;

    if (NULL == strcpy((void *)str, Tmp->Messages->Signals->unit))
        return FUNC_ERR;

    return FUNC_OK;
}

static int cDBC_get_signal_receiver(cDBC * p_cDBC, const char * str) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;

    if (NULL == strcpy((void *)str, Tmp->Messages->Signals->receiver))
        return FUNC_ERR;

    return FUNC_OK;
}

static int cDBC_get_number_of_signals(cDBC * p_cDBC, int * number_of_signals) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;

    *number_of_signals = Tmp->Messages->number_of_signals;

    return FUNC_OK;
}

static int cDBC_next_signal(cDBC * p_cDBC) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals->next)
        return FUNC_ERR;

    Tmp->Messages->Signals = Tmp->Messages->Signals->next;

    return FUNC_OK;
}

static int cDBC_previous_signal(cDBC * p_cDBC) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals->prev)
        return FUNC_ERR;

    Tmp->Messages->Signals = Tmp->Messages->Signals->prev;

    return FUNC_OK;
}

static int cDBC_reset_signal_position(cDBC * p_cDBC) {
    D * Tmp = (D *)p_cDBC;

    if (NULL == Tmp)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals)
        return FUNC_ERR;
    else if (NULL == Tmp->Messages->Signals_begin)
        return FUNC_ERR;

    Tmp->Messages->Signals = Tmp->Messages->Signals_begin;

    return FUNC_OK;
}

int cDBC_operation(cDBC * p_cDBC, FUNCTION function, void * data) {
    int res = 0;
    switch (function) {
        case FUNC_PRINT_ALL:
            cDBC_print_all(p_cDBC);
            break;
        case FUNC_PRINT_ONE:
            cDBC_print_one(p_cDBC);
            break;
        case FUNC_READ_FILE:
            res = cDBC_read_file(p_cDBC, (const char *)data);
            break;
        case FUNC_FIND_MESSAGE_BY_NAME:
            res = cDBC_find_message_by_name(p_cDBC, (const char *)data);
            break;
        case FUNC_FIND_MESSAGE_BY_ID:
            res = cDBC_find_message_by_id(p_cDBC, *((const unsigned int *)data));
            break;
        case FUNC_GET_MESSAGE_NAME:
            res = cDBC_get_message_name(p_cDBC, (const char *)data);
            break;
        case FUNC_GET_MESSAGE_ID:
            res = cDBC_get_message_id(p_cDBC, (int *)data);
            break;
        case FUNC_GET_MESSAGE_SIZE:
            res = cDBC_get_message_size(p_cDBC, (int *)data);
            break;
        case FUNC_GET_MESSAGE_TRANSMITTER:
            res = cDBC_get_message_transmitter(p_cDBC, (const char *)data);
            break;
        case FUNC_GET_NUMBER_OF_MESSAGES:
            res = cDBC_get_number_of_messages(p_cDBC, (int *)data);
            break;
        case FUNC_NEXT_MESSAGE:
            res = cDBC_next_message(p_cDBC);
            break;
        case FUNC_PREVIOUS_MESSAGE:
            res = cDBC_previous_message(p_cDBC);
            break;
        case FUNC_RESET_MESSAGE_POSITION:
            res = cDBC_reset_message_position(p_cDBC);
            break;
        case FUNC_FIND_SIGNAL_BY_NAME:
            res = cDBC_find_signal_by_name(p_cDBC, (const char *)data);
            break;
        case FUNC_GET_SIGNAL_MULTIPLEXER_INDICATOR:
            res = cDBC_get_signal_multiplexer_indicator(p_cDBC, (const char *)data);
            break;
        case FUNC_GET_SIGNAL_NAME:
            res = cDBC_get_signal_name(p_cDBC, (const char *)data);
            break;
        case FUNC_GET_SIGNAL_BIT_START:
            res = cDBC_get_signal_bit_start(p_cDBC, (int *)data);
            break;
        case FUNC_GET_SIGNAL_LENGTH:
            res = cDBC_get_signal_size(p_cDBC, (int *)data);
            break;
        case FUNC_GET_SIGNAL_BYTE_ORDER:
            res = cDBC_get_signal_byte_order(p_cDBC, (int *)data);
            break;
        case FUNC_GET_SIGNAL_VALUE_TYPE:
            res = cDBC_get_signal_value_type(p_cDBC, (char *)data);
            break;
        case FUNC_GET_SIGNAL_FACTOR:
            res = cDBC_get_signal_factor(p_cDBC, (double *)data);
            break;
        case FUNC_GET_SIGNAL_OFFSET:
            res = cDBC_get_signal_offset(p_cDBC, (double *)data);
            break;
        case FUNC_GET_SIGNAL_MINIMUM:
            res = cDBC_get_signal_minimum(p_cDBC, (double *)data);
            break;
        case FUNC_GET_SIGNAL_MAXIMUM:
            res = cDBC_get_signal_maximum(p_cDBC, (double *)data);
            break;
        case FUNC_GET_SIGNAL_UNIT:
            res = cDBC_get_signal_unit(p_cDBC, (const char *)data);
            break;
        case FUNC_GET_SIGNAL_RECEIVER:
            res = cDBC_get_signal_receiver(p_cDBC, (const char *)data);
            break;
        case FUNC_GET_NUMBER_OF_SIGNALS:
            res = cDBC_get_number_of_signals(p_cDBC, (int *)data);
            break;
        case FUNC_NEXT_SIGNAL:
            res = cDBC_next_signal(p_cDBC);
            break;
        case FUNC_PREVIOUS_SIGNAL:
            res = cDBC_previous_signal(p_cDBC);
            break;
        case FUNC_RESET_SIGNAL_POSITION:
            res = cDBC_reset_signal_position(p_cDBC);
            break;
    }

    return res;
}

int cDBC_Init(cDBC ** DBC) {
    cDBC * p_cDBC = NULL;
    *DBC = NULL;
    p_cDBC = malloc(sizeof(D));
    if (p_cDBC) {
        ((D *)p_cDBC)->Messages_begin = NULL;
        ((D *)p_cDBC)->Messages = NULL;

        *DBC = p_cDBC;

        return FUNC_OK;
    }

    return FUNC_ERR;
}

void cDBC_Deinit(cDBC ** pp_cDBC) {
    D * Tmp = (D *)(*pp_cDBC);

    if (Tmp) {
        M * Mes_begin = Tmp->Messages;
        if (Mes_begin) {
            do {
                M * Mes_tmp = Tmp->Messages;
                cDBC_sig_free(Tmp->Messages->Signals_begin);
                Tmp->Messages = Tmp->Messages->next;
                free(Mes_tmp);
            } while (Tmp->Messages != Mes_begin && Tmp->Messages != NULL);
        }

        Tmp->Messages_begin = NULL;
        Tmp->Messages = NULL;
        Tmp->number_of_mesages = 0;

        free(Tmp);
    }
    *pp_cDBC = NULL;
}
