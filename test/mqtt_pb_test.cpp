#include "gtest/gtest.h"
#include <sstream>
#include <pb_parse.h>
#include <pb_decode.h>
#include <pb_encode.h>

#define TEST_URL "s3://cs-main/jetson-test/95105c07-68dd-4c3f-b642-742fa9eee797/firmware.hex"
#define LONG_STRING "pnkPLZhmqwLbjfX1VPXCtkmA9z8jX2gHQqtvClFFhECPTLSvWUQGaheDztnvkxJPJhjvRfgQjmO4S1pWqM1d7jAu5cxcLejJHUtEyf4szZ1DH0TlRm3I2rtIX0DwVkt61ghu47r5N1rNHYiWHBiacrg1UDGvBD51NhzKUtfY5USa7DdN2PGqrDrrkySUzimxCbXnqpPHVFAqy4YW0yQrhFtkSZB6o5CzPKCsdx0pmeUdjbFU6vHiPNDlZ64zSeg9pjoE068WxqKf4jswL7PZjOQdEgWYrftdpYLvdUwFlGvCRYe7GEMjfLtXMKJM1fK6DsKPIEiOhSM20dXlhDmtJm5WZMtVhPNYXNlSW0lwVsq9eYunbGf16xIeV345YP7co6Wh5cy8kaHLTNhvu9USr5nIfn7GExuHDE0sHF5zGi4LV98XJ4idW386EdzTxYTwcRgHGm1swjnVUdVyvnmUTEdJuKtEkk5cH5GXHuqdRer6CkRl9BCKQq6ZEAwREZkz5MrSq609c2BFqUvpAxHktIsbv7LeEHtm0kwvkNYRF98G1qVDF4K3p507pJEW1KE2SEkeQsXYxoPvT9heofidjAvQ5Yfr8ApPMaU6xFVz3TXcmZiPxOxBcAfVEvftQlTIihWQyiMrL5ExAyH0PQBSGPxJ5LmODUPac5pI3rG4Wx1C42YYZp0vwP58haDmF6yqSDmkzELWVJWgGCqtddiQ8vn9Q4cop67YP3r8g7gzee1jCro4jO89AJ4bH5N42kLcUQjbbIWQT1k9GOaY4GSnpTzttrI3Lc1KU5LfaPz2kRfZZtP0YWiMDtfkYlQRXjC0eic7pLyt2NtZmMVTRVIafr3O14uOOFy9brb3xzZ030fGVDnNZgJx3fMdDT7itweR6tUpZckgcU8H7hL3SIJwGLS4WpHWICrfvoijcZMs1Ccevwe6FwcFaWQUXFlyjl9luRHAb6HpsSG0LGYtq1z0e6otO2JVZKWhYrT4F9wAqFOEBQgBWiyqUlhAiEQfbubT"

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

