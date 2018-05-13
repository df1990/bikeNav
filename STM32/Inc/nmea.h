/*
 * nmea.h
 *
 *  Created on: May 11, 2018
 *      Author: dariusz
 */

#ifndef NMEA_H_
#define NMEA_H_

#include "nmea_types.h"
#include "nmea_config.h"
#include <stdint.h>

void nmea_init(void);
uint8_t nmea_register_handler(nmea_type type, nmea_handler handler);
uint8_t nmea_receive(char data);
uint8_t nmea_decode(void);
uint8_t nmea_execute(void);
nmea_type nmea_get_type(void);


#endif /* NMEA_H_ */
