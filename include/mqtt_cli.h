    #ifndef _MQTT_CLI_H
    #define _MQTT_CLI_H
    
    #include <curl/curl.h>
    #include <memory>
    #include  <string.h>
    #include "../include/post.h"
    #include "../include/mqtt_cli.h"
    #include "json/json.h"

    CURL *curlInit();
    size_t write_data(void* contents, size_t size, size_t nmemb, void *s);
    CURLcode connectServer();
    CURLcode connectServer2();
    CURLcode disconnectServer(const char* tokenId);
    CURLcode publishMsg(uint64_t *msg, const char* topic, const char* tokenId);
    const char* extractTokenId();
    void resetCurl();
    void curlCleanUp();

    #endif