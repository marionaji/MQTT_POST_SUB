#include <iostream>
#include <vector>
#include <cstring>
#include <pb_decode.h>
#include <pb_encode.h>
#include "jetson_mqttpkt.pb.h"

#define OVERHEAD_SIZE 1024 //this is very generous

jetson_mqttpacket decode_stream(uint8_t * buffer, int len)
{
    jetson_mqttpacket received;// = jetson_mqttpacket_init_zero;
    pb_istream_t istream = pb_istream_from_buffer(buffer, len);
    if (!pb_decode(&istream, jetson_mqttpacket_fields, &received))
    {
        std::cout << "pb decode error:" << PB_GET_ERROR(&istream) << std::endl;
    }
    return received;
}

std::vector<uint8_t> encode_stream(const jetson_mqttpacket p)
{
    jetson_mqttpacket local = p;
    int buffer_length = strlen(p.payload.exec_script.url);
    std::cout << buffer_length << std::endl;
    std::vector<uint8_t> buffer(buffer_length + OVERHEAD_SIZE);
    pb_ostream_t ostream;
    ostream = pb_ostream_from_buffer(buffer.data(), buffer.capacity());
    if (!pb_encode(&ostream, jetson_mqttpacket_fields, &local))
    {
        std::cout << "pb encode_error:" << PB_GET_ERROR(&ostream) << std::endl;
    }
    buffer.resize(ostream.bytes_written);
    return buffer;
}
