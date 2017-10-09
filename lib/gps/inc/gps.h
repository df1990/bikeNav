#ifndef _GPS_H_
#define _GPS_H_
#include "nmea.h"
#include <stdint.h>

#define NMEA_FILTERS_CNT_MAX 5
#define NMEA_FIELDS_CNT_MAX 6

typedef enum {
    Gps_OK = 0,
    Gps_Err_Sentence_Invalid,
    Gps_Err_Filter_List_Full,
    Gps_Err_Fields_List_Full,
    Gps_Err_Arg_Invalid,
    Gps_Err_Filter_Invalid,
    Gps_Err_Field_Invalid,
    Gps_Err_Not_Implemented,
} TGpsRetVal;

TGpsRetVal gps_init(void);
uint8_t gps_nmea_rx_complete(void);
void gps_nmea_rx_clear(void);
char *gps_get_nmea_rx_buff(void);
TGpsRetVal gps_filter_add(const char *nmea_name, uint8_t *filter_id);
TGpsRetVal gps_field_add(uint8_t filter_id, uint8_t field_id, char *field_buff);
TGpsRetVal gps_nmea_rx_check(uint8_t *filter_id);



#endif
