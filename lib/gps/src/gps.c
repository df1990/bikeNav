#include "gps.h"
#include "nmea.h"
#include "uart.h"
#include <string.h>

void gps_rx_data(uint8_t data);

volatile char nmea_rx_buffer[NMEA_SENTENCE_LENGTH_MAX];
volatile uint8_t nmea_rx_w_idx;
volatile uint8_t nmea_rx_r_idx;
volatile uint8_t nmea_sentence_received;
volatile uint8_t nmea_sentence_length;

volatile TNmeaSentence nmea_received_sentence_type;

typedef enum {
    nmea_start_wait = 0,
    nmea_cr_wait,
    nmea_lf_wait
} nmea_rx_state;

volatile nmea_rx_state rx_state;

typedef struct {
    TNmeaSentence nmea_sentence;
    uint8_t nmea_fields[NMEA_FILTER_FIELDS_CNT];
    uint8_t *nmea_fields_buffers[NMEA_FILTER_FIELDS_CNT];
    uint8_t nmea_fields_cnt;
} SNmeaFilter;

static SNmeaFilter nmea_filters[NMEA_FILTERS_CNT];
uint8_t nmea_filters_cnt;


TGpsRetVal gps_init(void)
{
    uint8_t filter_idx;
    uint8_t field_idx;
    memset((void*)nmea_rx_buffer, 0, NMEA_SENTENCE_LENGTH_MAX);
    //memset((void*)nmea_rx_sentence, 0, NMEA_SENTENCE_LENGTH_MAX);
    nmea_rx_w_idx = 0;
    nmea_rx_r_idx = 0;
    nmea_sentence_received = 0;
    nmea_sentence_length = 0;
    rx_state = nmea_start_wait;
    
    nmea_received_sentence_type = nmea_no_sentence;
    
    for(filter_idx = 0; filter_idx < NMEA_FILTERS_CNT; filter_idx++)
    {
        nmea_filters[filter_idx].nmea_sentence = nmea_no_sentence;
        nmea_filters[filter_idx].nmea_fields_cnt = 0;
        for(field_idx = 0; field_idx < NMEA_FILTER_FIELDS_CNT; field_idx++)
        {
            nmea_filters[field_idx].nmea_fields_buffers[field_idx] = NULL;
        }
    }
    nmea_filters_cnt = 0;
    
    uart_puts("GSP init [OK]\n");
    return Gps_OK;
}

TGpsRetVal gps_handler_register(void)
{
    uart_rx_handler_register(gps_rx_data);
    return Gps_OK;
}

TGpsRetVal gps_sentence_filter_add(TNmeaSentence sentence, uint8_t field_id, uint8_t* field_buff)
{
    uint8_t filter_idx;
    uint8_t fields_cnt;
    
    if((NULL == field_buff) || (nmea_no_sentence == sentence))
    {
        return Gps_Err_Arg_Invalid;
    }
    
    for(filter_idx = 0; filter_idx < NMEA_FILTERS_CNT; filter_idx++)
    {
        if(nmea_filters[filter_idx].nmea_sentence == sentence)
        {
            fields_cnt = nmea_filters[filter_idx].nmea_fields_cnt;
            if(fields_cnt < NMEA_FILTER_FIELDS_CNT)
            {
                nmea_filters[filter_idx].nmea_fields[fields_cnt] = field_id;
                nmea_filters[filter_idx].nmea_fields_buffers[fields_cnt] = field_buff;
                nmea_filters[filter_idx].nmea_fields_cnt++;
                return Gps_OK;
            }
            else
            {
                return Gps_Sentence_List_Full;
            }
        }
        else if(nmea_filters[filter_idx].nmea_sentence == nmea_no_sentence)
        {
            nmea_filters[filter_idx].nmea_sentence = sentence;
            nmea_filters[filter_idx].nmea_fields[0] = field_id;
            nmea_filters[filter_idx].nmea_fields_buffers[0] = field_buff;
            nmea_filters[filter_idx].nmea_fields_cnt = 1;
        }
    }
    return Gps_Sentence_List_Full;
}

TGpsRetVal gps_sentence_filter_remove(TNmeaSentence sentence, uint8_t field_id)
{
    return Gps_Err_Not_Implemented;
}

TGpsRetVal gps_sentence_get_field(uint8_t sentence_id, uint8_t field_nr)
{
    return Gps_Err_Not_Implemented;
}

TNmeaSentence gps_nmea_sentence_received(void)
{
    return nmea_received_sentence_type;
}

void gps_nmea_sentence_clear(void)
{
    nmea_received_sentence_type = nmea_no_sentence;
}

static TNmeaSentence nmea_sentence_validate(void)
{
    char *token;
    uint8_t decode_idx = 0;
    uint8_t filters_idx = 0;
    uint8_t fields_idx = 0;
    uint8_t fields_cnt = 0;
    
    uint8_t field_id = 0;
    
    TNmeaSentence received_sentence = nmea_no_sentence;
    uint8_t sentences_decoder_length = sizeof(nmea_sentences_decoder) / sizeof(nmea_sentences_decoder[0]);
    
    token = strtok(nmea_rx_buffer,NMEA_SENTENCE_FIELD_DELIM);
    if(NULL == token)
    {
        /* Invalid frame format, no field delimeters */
        return nmea_no_sentence;
    }
    
    if(0 != strncmp(token,NMEA_SENTENCE_PREFIX,2))
    {
        /* Invalid frame prefix */
        return nmea_no_sentence;
    }
    
    for(decode_idx = 0; decode_idx < sentences_decoder_length; decode_idx++)
    {
        if(0 == strncmp(nmea_sentences_decoder[decode_idx].nmea_sentence_name,&token[2],3))
        {
                received_sentence = nmea_sentences_decoder[decode_idx].nmea_sentence;
                break;
        }
    }
    
    if(nmea_no_sentence == received_sentence)
    {
        /* Received sentence not recognized */
        return nmea_no_sentence;
    }
    
    for( ; filters_idx < NMEA_FILTERS_CNT; filters_idx++)
    {
        if(nmea_filters[filters_idx].nmea_sentence == received_sentence)
        {
            break;
        }
    }

    if(NMEA_FILTERS_CNT == filters_idx)
    {
        /* Received sentence not found on filters list */
        return received_sentence;
    }

    for( ; fields_idx < nmea_filters[filters_idx].nmea_fields_cnt; fields_idx++)
    {
        field_id = nmea_filters[filters_idx].nmea_fields[fields_idx];
        
        while((NULL != (token = strtok(NULL,NMEA_SENTENCE_FIELD_DELIM))) && (fields_cnt < field_id))
        {
            fields_cnt++;
        }
        
        if(NULL == token)
        {
            break;
        }
        else
        {
            strcpy(nmea_filters[filters_idx].nmea_fields_buffers[fields_idx],token);
        }
    }

    return received_sentence;
}

void gps_rx_data(uint8_t data)
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
            else if(nmea_rx_w_idx < NMEA_SENTENCE_LENGTH_MAX-1)
            {
                nmea_rx_buffer[nmea_rx_w_idx++] = rx_data;
            }
            else
            {
                rx_state = nmea_start_wait;  
                nmea_rx_w_idx = 0;
            }    
        break;
            
        case nmea_lf_wait:
            if('\n' == rx_data)
            {
                nmea_sentence_received = 1;
                nmea_rx_buffer[nmea_rx_w_idx] = 0x00;
                nmea_received_sentence_type = nmea_sentence_validate();
            }
            rx_state = nmea_start_wait;
            nmea_rx_w_idx = 0;
            //nmea_received_sentence_type = nmea_no_sentence;
        break;
    }
    
}

