
#include "pb_encode.h"
#include "../include/mqtt_packet.h"
#include "mqttpkt.pb.h"
#include "../include/post.h"



int constructPacket(uint8_t * buf, mqtt_pkt packet, int cmd_type)
{
    mqttpacket pkt = mqttpacket_init_zero;
    if(cmd_type == SCAN)
    {
        pkt.payload.do_scan.timeout = packet.do_scan.timeout;
        strcpy(pkt.payload.do_scan.command,packet.do_scan.command);
        pkt.which_payload = mqttpacket_do_scan_tag; 
    }
    else if (cmd_type == TURN_ON)
    {
        pkt.payload.turn_on.timeout = packet.turn_on.timeout;
        strcpy(pkt.payload.turn_on.command,packet.turn_on.command);
        pkt.which_payload = mqttpacket_turn_on_tag;
    }
    else if (cmd_type == SCHEDULE)
    {
        strcpy(pkt.payload.scan_schedule.schedule,packet.scan_schedule.command);
        pkt.which_payload = mqttpacket_scan_schedule_tag;
    }
    else
    {
        printf("Invalid command !\n");
    }

    pb_ostream_t ostream;
    ostream = pb_ostream_from_buffer(buf, mqttpacket_size);
    if (!pb_encode(&ostream, mqttpacket_fields, &pkt))
    {
        return -1;
    }
    return ostream.bytes_written;
}