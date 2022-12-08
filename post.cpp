#include <stdio.h>
#include <iostream>
#include <pb_encode.h>
#include <pb_decode.h>
//#include "mqttpkt.pb.h"

using namespace std;


// typedef struct 
// {
//     int timeout;
// } mqttpacket;

// int constructPacket(mqttpacket pkt)
// {
//     uint8_t buf[]
//     pb_ostream_t ostream;
//     ostream = pb_ostream_from_buffer(buf, sizeof(buf));
//     if (pb_encode(&ostream, Packet_fields, p))
//     {
//         packFrame(buf, ostream.bytes_written);
//     }
//     else
//     {
//         Serial.print("Encode Error: ");
//         Serial.println(PB_GET_ERROR(&ostream));
//     }
// }

int main(int argc, char **argv)
{
    cout << "turnOn" << argc-1 << " arguments:" << "\n";

    for (int i = 0; i < argc; ++i)
    {
        cout << argv[i] << "\n";
    }
    
}