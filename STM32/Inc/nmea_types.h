/*
 * nmea_types.h
 *
 *  Created on: May 11, 2018
 *      Author: dariusz
 */

#ifndef NMEA_TYPES_H_
#define NMEA_TYPES_H_
#include <stdint.h>

typedef enum {
    GGA,
    GSA,
    GSV,
    RMC,
    VTG,
    Invalid
} nmea_type;

typedef struct {
    const char* name;
    nmea_type type;
} nmea_decoder;

static const nmea_decoder decoder_array[] = {
        {"GPGGA",GGA},
        {"GPGSA",GSA},
        {"GPGSV",GSV},
        {"GPRMC",RMC},
        {"GPVTG",VTG}
};

typedef uint8_t (*nmea_handler)(const void *);

typedef struct {
	nmea_type type;
	nmea_handler handler;
} nmea_trigger;

#endif /* NMEA_TYPES_H_ */
