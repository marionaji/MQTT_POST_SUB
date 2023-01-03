#ifndef _PB_PARSE_H
#define _PB_PARSE_H

#include <vector>
#include <jetson_mqttpkt.pb.h>

jetson_mqttpacket decode_stream(uint8_t * buffer, int len);
std::vector<uint8_t> encode_stream(const jetson_mqttpacket p);

#endif