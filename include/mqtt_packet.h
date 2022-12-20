#ifndef _MQTT_PACKET_H
#define _MQTT_PACKET_H

#include "../include/post.h"

int constructPacket(uint8_t * buf, mqtt_pkt packet, int cmd_type);

#endif