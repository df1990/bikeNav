#ifndef _GPS_H_
#define _GPS_H_
#include "nmea.h"
#include <stdint.h>

#define NMEA_FILTERS_CNT 5
#define NMEA_FILTER_FIELDS_CNT 5

typedef enum {
    Gps_OK = 0,
    Gps_Sentence_Invalid,
    Gps_Sentence_List_Full,
    Gps_Sentence_List_Empty,
    Gps_Err_Not_Implemented,
    Gps_Err_Arg_Invalid
} TGpsRetVal;

TGpsRetVal gps_init(void);
TGpsRetVal gps_handler_register(void);
TGpsRetVal gps_sentence_filter_add(TNmeaSentence sentence, uint8_t field_id, uint8_t* field_buff);
TGpsRetVal gps_sentence_filter_remove(TNmeaSentence sentence, uint8_t field_id);

TNmeaSentence gps_nmea_sentence_received(void);
void gps_nmea_sentence_clear(void);

#endif
