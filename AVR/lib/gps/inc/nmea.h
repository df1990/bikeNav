#ifndef _NMEA_H_
#define _NMEA_H_

#define NMEA_SENTENCE_LENGTH_MAX 82
#define NMEA_SENTENCE_NAME_LENGHT 5

#define NMEA_SENTENCE_START_DELIM '$'
#define NMEA_SENTENCE_FIELD_DELIM ','
#define NMEA_SENTENCE_PREFIX "GP"

#define NMEA_SENTENCE_GGA "GGA"
#define NMEA_SENTENCE_GSA "GSA"
#define NMEA_SENTENCE_GSV "GSV"
#define NMEA_SENTENCE_RMC "RMC"
#define NMEA_SENTENCE_VTG "VTG"
#define NMEA_SENTENCE_DUP "DUP"

typedef enum {
    nmea_no_sentence = 0,
    nmea_gga,
    nmea_gsa,
    nmea_gsv,
    nmea_rmc,
    nmea_vtg,
} TNmeaSentence;

typedef struct {
    TNmeaSentence nmea_sentence;
    const char *nmea_sentence_name;
} SNmeaMapper;

static const SNmeaMapper nmea_sentences_decoder[] = {
    {nmea_gga, NMEA_SENTENCE_GGA},
    {nmea_gsa, NMEA_SENTENCE_GSA},
    {nmea_gsv, NMEA_SENTENCE_GSV},
    {nmea_rmc, NMEA_SENTENCE_RMC},
    {nmea_vtg, NMEA_SENTENCE_VTG}
};

#endif