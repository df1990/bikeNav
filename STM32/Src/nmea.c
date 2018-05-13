/*
 * nmea.c
 *
 *  Created on: May 11, 2018
 *      Author: dariusz
 */
#include "nmea.h"
#include <stddef.h>
#include <string.h>

typedef enum {
	wait_start,
	wait_end
} nmea_rx_state;


static nmea_trigger nmea_trigger_array[NMEA_MAX_HANDLER_CNT];
static uint8_t nmea_trigger_cnt = 0;


#define NMEA_MAX_LEN 82
static char nmea_rx_buff[NMEA_MAX_LEN];
static char nmea_rx_sentence[NMEA_MAX_LEN];
static uint8_t nmea_rx_cnt = 0;
static uint8_t nmea_sentence_length = 0;



void nmea_init(void)
{
	uint8_t i;
	for(i = 0; i < NMEA_MAX_HANDLER_CNT; i++)
	{
		nmea_trigger_array[i].type = Invalid;
		nmea_trigger_array[i].handler = NULL;
	}
	nmea_trigger_cnt = 0;

	for(i = 0; i < NMEA_MAX_LEN; i++)
	{
		nmea_rx_buff[i] = '\0';
	}
	nmea_rx_cnt = 0;

	for(i = 0; i < NMEA_MAX_LEN; i++)
	{
		nmea_rx_sentence[i] = '\0';
	}
	nmea_sentence_length = 0;
}

uint8_t nmea_register_handler(nmea_type type, nmea_handler handler)
{
	if(nmea_trigger_cnt >= NMEA_MAX_HANDLER_CNT)
	    return 0;

	nmea_trigger_array[nmea_trigger_cnt].handler = handler;
	nmea_trigger_array[nmea_trigger_cnt].type = type;
	nmea_trigger_cnt++;
	return 1;
}

uint8_t _rx_buff_push(char d)
{
	if(nmea_rx_cnt >= NMEA_MAX_LEN)
		return 0;

	nmea_rx_buff[nmea_rx_cnt++] = d;
	    return 1;
}

void _rx_buff_copy(void)
{
	memcpy(nmea_rx_sentence, nmea_rx_buff, nmea_rx_cnt);
	nmea_sentence_length = nmea_rx_cnt;
}

void _rx_buff_clear(void)
{
	nmea_rx_cnt = 0;
}

uint8_t nmea_receive(char data)
{
	static nmea_rx_state state = wait_start;
    uint8_t ret_val = 0;

	switch(state)
	{
	    case wait_start:
	    	if(data == '$'){
	    		state = wait_end;
	    	}
	    break;

	    case wait_end:
	    	if((data == '\r') || (data == '\n')) {
	    		state = wait_start;
	    		_rx_buff_copy();
	    		_rx_buff_clear();
	    		ret_val = 1;
	    	}
	    	else if(!_rx_buff_push(data)) {
	    		state = wait_start;
	    		_rx_buff_clear();
	    	}
	    break;

	    default:
	    	state = wait_start;
	    	_rx_buff_clear();
	    break;
	}

	return ret_val;
}

uint8_t nmea_decode(void)
{
	return 0;
}

uint8_t nmea_execute(void)
{
	return 0;
}

nmea_type nmea_get_type(void)
{
	return 0;
}

