#include <iostream>
#include <memory>
#include  <string.h>
#include "../include/post.h"
#include "../include/mqtt_cli.h"
#include "json/json.h"

using namespace std;

int timeout_param;
int flag;
do_scan_t do_scan;
turn_on_t turn_on;
scan_schedule_t scan_schedule;

bool verifyCmd(const char *cmd_action, const char *cmd_param)
{
    bool success = false;
    if (strcmp(cmd_action,DO_SCAN_CMD) == 0)
    {
       timeout_param = atoi(cmd_param);
       memcpy(do_scan.command,cmd_action,strlen(DO_SCAN_CMD)+1);
       do_scan.timeout = timeout_param;
       success = true;
    }
    else if (strcmp(cmd_action,TURN_ON_CMD) == 0)
    {
        timeout_param = atoi(cmd_param);
        memcpy(turn_on.command,cmd_action,strlen(TURN_ON_CMD)+1);
        turn_on.timeout = timeout_param;
        success = true;
    }
    else if (strcmp(cmd_action,SCAN_SCHEDULE_CMD) == 0)
    {
        timeout_param = atoi(cmd_param);
        memcpy(scan_schedule.command,cmd_action,strlen(SCAN_SCHEDULE_CMD)+1);
        success = true;
    }
    else
    {
        cout << "You entered an invalid command !" << "\n";
    }
    return success;
}


int main(int argc, char **argv)
{
    if(argc > MAX_CMD_LINE_ARGS)
    {
        cout << "Too many arguments !" << "\n";
        return 0;
    }
    // else if(!verifyCmd(argv[1],argv[2]))
    // {
    //     cout << "Invalid arguments !" << "\n";
    //     return 0;
    // }
    

    if(curlInit()) 
    {
        CURLcode res = connectServer();
        /* Check for errors */
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
        }
        else
        {
            const char* token = extractTokenId();
            std::cout << token << std::endl;
            Sleep(1000);
            res = publishMsg((uint64_t*)argv[1], argv[2], token);
            if(res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            }
            Sleep(1000);
            res = disconnectServer(token);
            if(res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            }
            curlCleanUp();
        }
    }

    return 0;
}
