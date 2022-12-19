

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/mqtt_cli.h"

bool is_client_connected = false;
CURL *curl;
std:: string http_reply;


CURL *curlInit()
{
    curl_global_init(CURL_GLOBAL_ALL);
    return  curl = curl_easy_init();
}

size_t write_data(void* contents, size_t size, size_t nmemb, void *s)
{
    ((std::string*)s)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

CURLcode connectServer()
{   
    long http_response_code;    
    curl_easy_setopt(curl, CURLOPT_URL, "https://node02.myqtthub.com/login");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"clientId\" : \"super_dude\", \"userName\" : \"123\", \"password\" : \"123\", \"cleanSession\" : false}");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookie_store.txt");
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &http_reply);

    CURLcode status = curl_easy_perform(curl);
    std::cout << http_response_code << std::endl;

    return  status;
}

const char* extractTokenId()
{
    std::string token_id;
    Json::Value root;   
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( http_reply.c_str(), root );     //parse process
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse"<< reader.getFormattedErrorMessages();
        return NULL;
    }
    token_id = root.get("tokenId", "A Default Value if not exists" ).asString();

    return token_id.c_str();
}

void curlCleanUp()
{
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}


CURLcode publishMsg(uint64_t *msg, const char* topic, const char* tokenId)
{
    char post_msg[140];
    long http_response_code;
    snprintf(post_msg,sizeof(post_msg),"{\"topic\" : \"%s\", \"qos\" : 2, \"payload\" : \"%s\", \"retain\" : false, \"dup\" : false, \"tokenId\" : \"%s\"}",topic,"hell",tokenId);
    std::cout << post_msg << std::endl;
    curl_easy_setopt(curl, CURLOPT_URL, "https://node02.myqtthub.com/publish");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_msg);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie_store.txt");
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
    CURLcode status = curl_easy_perform(curl);
    std::cout << http_response_code << std::endl;
    
    return  status;
}

CURLcode disconnectServer(const char* tokenId)
{
    char post_msg[53];
    long http_response_code;
    snprintf(post_msg,sizeof(post_msg),"{\"tokenId\" : \"%s\"}",tokenId);
    std::cout << post_msg << std::endl;
    curl_easy_setopt(curl, CURLOPT_URL, "https://node02.myqtthub.com/logout");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_msg);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie_store.txt");
    // char cookie_msg[47];
    // snprintf(cookie_msg,sizeof(cookie_msg),"tokenId:\"%s\"",tokenId);
    //std::cout << cookie_msg << std::endl;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
    //curl_easy_setopt(curl, CURLOPT_COOKIE, cookie_msg);
    CURLcode status = curl_easy_perform(curl);
    std::cout << http_response_code << std::endl;

    return status;
}



void resetCurl()
{
   curl_easy_reset(curl);
}

CURLcode connectServer2()
{
    // long http_response_code;    
    // curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
    // CURLcode status = curl_easy_perform(curl);
    // std::cout << http_response_code << std::endl;

    //return  res;
}

	
