#ifndef _MQTT_CALLBACK_H
#define _MQTT_CALLBACK_H

#include <vector>
#include "jetson_mqttpkt.pb.h"

bool execScript_callback(pb_istream_t *istream, pb_ostream_t *ostream, const pb_field_t *field);
bool firmwareUpdate_callback(pb_istream_t *istream, pb_ostream_t *ostream, const pb_field_t *field);

#endif