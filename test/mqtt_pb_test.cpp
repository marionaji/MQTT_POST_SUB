#include "gtest/gtest.h"
#include <sstream>
#include <pb_parse.h>
#include <pb_decode.h>
#include <pb_encode.h>

#define TEST_URL "s3://cs-main/jetson-test/95105c07-68dd-4c3f-b642-742fa9eee797/firmware.hex"
//2048 in length
#define LONG_STRING "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"

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
    if (istream && field->tag == execScript_url_tag)
    {
        std::string s = decode_string(istream);
        if (s.size() == 0)
        {
            return false;
        }
        EXPECT_STREQ(TEST_URL, s.c_str()) << s.c_str() << std::endl;
        return true;
    }
    else if (ostream && field->tag == execScript_url_tag)
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
    std::cout << field->tag << std::endl;
    if (istream && field->tag == firmwareUpdate_url_tag)
    {
        std::string s = decode_string(istream);
        if (s.size() == 0)
        {
            return false;
        }
        EXPECT_STREQ(LONG_STRING, s.c_str()) << s.c_str() << std::endl;
        return true;
    }
    else if (ostream && field->tag == firmwareUpdate_url_tag)
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

TEST(PB_PARSE_TEST, test_parse)
{
    jetson_mqttpacket srcpkt;
    srcpkt.which_payload = jetson_mqttpacket_exec_script_tag;
    srcpkt.payload.exec_script.url = TEST_URL;
    std::vector<uint8_t> buf = encode_stream(srcpkt);
    
    jetson_mqttpacket rstpkt;
    rstpkt = decode_stream(buf.data(), buf.size());

    EXPECT_EQ(srcpkt.which_payload, rstpkt.which_payload);
}

TEST(PB_PARSE_TEST, test_long_string)
{
    jetson_mqttpacket srcpkt;
    srcpkt.which_payload = jetson_mqttpacket_firmware_update_tag;
    srcpkt.payload.firmware_update.url = LONG_STRING;
    std::vector<uint8_t> buf = encode_stream(srcpkt);
    
    jetson_mqttpacket rstpkt;
    rstpkt = decode_stream(buf.data(), buf.size());

    EXPECT_EQ(srcpkt.which_payload, rstpkt.which_payload);
}

