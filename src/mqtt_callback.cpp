#include <iostream>
#include <string>
#include <filesystem>
#include <pb_decode.h>
#include <pb_encode.h>
#include "mqtt_callback.h"
#include "util.h"

std::string decode_string(pb_istream_t *istream)
{
    int length = istream->bytes_left;
    std::vector<uint8_t> local_buf(length);
    if (!pb_read(istream, local_buf.data(), length))
    {
        local_buf.clear();
    }
    return std::string(local_buf.begin(), local_buf.end());
}

bool encode_string(pb_ostream_t *ostream, const char * s, int strlen)
{
    return pb_encode_string(ostream, (const uint8_t*) s, strlen);
}

bool execScript_callback(pb_istream_t *istream, pb_ostream_t *ostream, const pb_field_t *field)
{
    if (istream && field->tag == jetson_mqttpacket_exec_script_tag)
    {
        std::string s = decode_string(istream);
        if (s.size() == 0)
        {
            return false;
        }
        std::cout << s << std::endl;
        

        
        return true;
    }
    else if (ostream && field->tag == jetson_mqttpacket_exec_script_tag)
    {
        const char * s = *(const char **)field->pData;
        if (!pb_encode_tag_for_field(ostream, field))
        {
            return false;
        }
        return encode_string(ostream, s, strlen(s));
    }
    return false;
}

bool firmwareUpdate_callback(pb_istream_t *istream, pb_ostream_t *ostream, const pb_field_t *field)
{
    if (istream && field->tag == jetson_mqttpacket_firmware_update_tag)
    {
        // std::vector<uint8_t> rst = decode_string(istream);
        std::string s = decode_string(istream);
        if (s.size() == 0)
        {
            return false;
        }
        std::cout << s << std::endl;
      
        std::filesystem::path pwd = std::filesystem::current_path();
        std::filesystem::path firmware = pwd/"firmware.hex";
        std::ostringstream command;
        command << "/usr/local/bin/aws s3 cp"  << " " << strip_tail_endline(s) << " " << firmware.string() << " --profile " << "ehsan.tahmasebian";
        std::cout<<"executing "<<command.str()<<std::endl;
        std::cout<<exec(command.str().c_str());

        std::cout<<"update firmware"<<std::endl;
        std::ostringstream flash_command;
        flash_command << "/usr/bin/avrdude -p atmega2560 -c wiring -P /dev/ttyACM0 -b 115200 -D -U flash:w:"<<firmware.string()<<":i";
        std::cout<<"executing "<<flash_command.str()<<std::endl;
        std::cout<<exec(flash_command.str().c_str());
        
        return true;
    }
    else if (ostream && field->tag == jetson_mqttpacket_firmware_update_tag)
    {
        const char * s = *(const char **)field->pData;
        if (!pb_encode_tag_for_field(ostream, field))
        {
            return false;
        }
        return encode_string(ostream, s, strlen(s));
    }
    return false;
}