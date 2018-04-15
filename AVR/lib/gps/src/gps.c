#include "gps.h"
#include "nmea.h"
#include "uart.h"
#include <string.h>

/* Structures and enums */
typedef enum {
    nmea_start_wait = 0,
    nmea_cr_wait,
    nmea_lf_wait
} TNmeaRxState;

typedef struct {
    uint8_t field_id;
    char *field_buffer;
} SNmeaField;

typedef struct {
    char name[NMEA_SENTENCE_NAME_LENGHT+1];
    SNmeaField fields[NMEA_FIELDS_CNT_MAX];
    uint8_t fields_cnt;
} SNmeaFilter;

/* Static variables */

//Rx state machine
volatile TNmeaRxState rx_state;
static volatile uint8_t nmea_sentence_rx_complete;

//Rx buffer
static char nmea_rx_buff[2][NMEA_SENTENCE_LENGTH_MAX+1];
static volatile uint8_t nmea_rx_buff_w_idx;
static volatile uint8_t nmea_rx_buff_current;

//Nmea filters
static SNmeaFilter nmea_filters[NMEA_FILTERS_CNT_MAX];
static uint8_t nmea_filters_cnt;

/* Static functions declaration */
static void gps_rx_data(uint8_t data);
static TGpsRetVal nmea_sentence_get_field(uint8_t field_id, char *buff);


/* Public functions */
TGpsRetVal gps_init(void)
{
    uint8_t filter_idx;
    uint8_t field_idx;
    
    for(filter_idx = 0; filter_idx < NMEA_FILTERS_CNT_MAX; filter_idx++)
    {
        memset(nmea_filters[filter_idx].name,0x00,(NMEA_SENTENCE_NAME_LENGHT+1));
        nmea_filters[filter_idx].fields_cnt = 0;
        for(field_idx = 0; field_idx < NMEA_FIELDS_CNT_MAX; field_idx++)
        {
            nmea_filters[filter_idx].fields[field_idx].field_id = 0;
            nmea_filters[filter_idx].fields[field_idx].field_buffer = NULL;
        }
    }
    nmea_filters_cnt = 0;
    
    rx_state = nmea_start_wait;
    nmea_sentence_rx_complete = 0;
    
    nmea_rx_buff_w_idx = 0;
    nmea_rx_buff_current = 0;
    memset((void*)nmea_rx_buff, 0x00, 2 * (NMEA_SENTENCE_LENGTH_MAX + 1));
    
    uart_rx_handler_register(gps_rx_data);
    uart_puts("GSP init [OK]\n");
    return Gps_OK;
}

TGpsRetVal gps_filter_add(const char *nmea_name, uint8_t *filter_id)
{
    TGpsRetVal retVal;
    uint8_t filter_idx;
    
    retVal = Gps_Err_Filter_List_Full;
    
    for(filter_idx = 0; filter_idx < NMEA_FILTERS_CNT_MAX; filter_idx++)
    {
        if(0x00 == nmea_filters[filter_idx].name[0])
        {
            strncpy(nmea_filters[filter_idx].name, nmea_name, NMEA_SENTENCE_NAME_LENGHT);
            *filter_id = filter_idx;
            nmea_filters_cnt++;
            retVal = Gps_OK;
            break;
        }
        else if(0 == strncmp(nmea_filters[filter_idx].name,nmea_name,NMEA_SENTENCE_NAME_LENGHT))
        {
            *filter_id = filter_idx;
            nmea_filters_cnt++;
            retVal = Gps_OK;
            break;
        }
    }
    return retVal;
    
}


TGpsRetVal gps_field_add(uint8_t filter_id, uint8_t field_id, char *field_buff)
{
    uint8_t fields_cnt;
    TGpsRetVal retVal;
    
    retVal = Gps_Err_Fields_List_Full;
    
    if((filter_id >= nmea_filters_cnt) || (NULL == field_buff))
    {
        return Gps_Err_Arg_Invalid;
    }
    
    fields_cnt = nmea_filters[filter_id].fields_cnt;
    if(fields_cnt < NMEA_FIELDS_CNT_MAX)
    {
        nmea_filters[filter_id].fields[fields_cnt].field_id = field_id;
        nmea_filters[filter_id].fields[fields_cnt].field_buffer = field_buff;
        nmea_filters[filter_id].fields_cnt++;
        retVal = Gps_OK;
    }

    return retVal;
}


TGpsRetVal gps_nmea_rx_check(uint8_t *filter_id)
{
    TGpsRetVal retVal;
    uint8_t filter_idx;
    uint8_t field_idx;
    uint8_t field_cnt;
    uint8_t nmea_read_buff;
    
    retVal = Gps_Err_Sentence_Invalid;
    nmea_read_buff = (nmea_rx_buff_current + 1) % 2;
    
    for(filter_idx = 0; filter_idx < NMEA_FILTERS_CNT_MAX; filter_idx++)
    {
        if(0 == strncmp(nmea_rx_buff[nmea_read_buff],nmea_filters[filter_idx].name,NMEA_SENTENCE_NAME_LENGHT))
        {
            *filter_id = filter_idx;
            retVal = Gps_OK;
            break;
        }
    }
    if(filter_idx < NMEA_FILTERS_CNT_MAX)
    {
        field_cnt = nmea_filters[filter_idx].fields_cnt;
        
        for(field_idx = 0; field_idx < field_cnt; field_idx++)
        {
            nmea_sentence_get_field(nmea_filters[filter_idx].fields[field_idx].field_id, nmea_filters[filter_idx].fields[field_idx].field_buffer);
        }
    }
    return retVal;
}

uint8_t gps_nmea_rx_complete(void)
{
    return nmea_sentence_rx_complete;
}

void gps_nmea_rx_clear(void)
{
    nmea_sentence_rx_complete = 0;
}

char *gps_get_nmea_rx_buff(void)
{
    uint8_t nmea_read_buff;
    nmea_read_buff = (nmea_rx_buff_current + 1) % 2;
    return &nmea_rx_buff[nmea_read_buff][0];
}

/* Static functions */
static TGpsRetVal nmea_sentence_get_field(uint8_t field_id, char *buff)
{
    TGpsRetVal retVal;
    uint8_t nmea_read_buff;
    uint8_t field_cnt;
    uint8_t field_len;
    uint8_t idx;
    
    retVal = Gps_Err_Field_Invalid;
    nmea_read_buff = (nmea_rx_buff_current + 1) % 2;
    idx = 0;
    field_len = 0;
    field_cnt = 0;
    field_id += 1;
    

    if(NULL == buff)
    {
        return Gps_Err_Arg_Invalid;
    }

    while((idx < NMEA_SENTENCE_LENGTH_MAX) && (nmea_rx_buff[nmea_read_buff][idx]))
    {
        if(NMEA_SENTENCE_FIELD_DELIM == nmea_rx_buff[nmea_read_buff][idx])
        {
            if(field_cnt++ == field_id)
            {
                if(0 == field_len)
                {
                    buff[0] = '-';
                    buff[1] = 0x00;
                }
                else
                {
                    strncpy(buff, &nmea_rx_buff[nmea_read_buff][idx - field_len],field_len);
                    buff[field_len] = 0x00;
                }
                retVal = Gps_OK;
            }
            field_len = 0;
        }
        else
        {
            field_len++;
        }
        idx++;
    }
    
    return retVal;
}

static void gps_rx_data(uint8_t data)
{
    char rx_data = (char)data;
    switch(rx_state)
    {
        case  nmea_start_wait:
            if(NMEA_SENTENCE_START_DELIM == rx_data)
            {
                rx_state = nmea_cr_wait; 
            }
        break;
            
        case nmea_cr_wait:
            if('\r' == rx_data)
            {
                rx_state = nmea_lf_wait;
            }
            else if(nmea_rx_buff_w_idx < NMEA_SENTENCE_LENGTH_MAX)
            { 
                nmea_rx_buff[nmea_rx_buff_current][nmea_rx_buff_w_idx++] = rx_data;
            }
            else
            {
                rx_state = nmea_start_wait;
                nmea_rx_buff_w_idx = 0;
            }
        break;
            
        case nmea_lf_wait:
            if('\n' == rx_data)
            {
                nmea_sentence_rx_complete = 1;
                nmea_rx_buff[nmea_rx_buff_current][nmea_rx_buff_w_idx] = 0x00;
                nmea_rx_buff_current = (nmea_rx_buff_current + 1) % 2;
            }
            rx_state = nmea_start_wait;
            nmea_rx_buff_w_idx = 0;
        break;
    }
}