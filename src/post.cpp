#include "../include/post.h"
using namespace std;

int timeout_param;

int constructPacket(const char *cmd_action, const char *cmd_param)
{
    if (strcmp(cmd_action,DO_SCAN_CMD) == 0)
    {
       do_scan_t do_scan;
       timeout_param = atoi(cmd_param);
       memcpy(do_scan.command,cmd_action,strlen(DO_SCAN_CMD)+1);
       cout << timeout_param << "\n";
    }
    else if (strcmp(cmd_action,TURN_ON_CMD) == 0)
    {
        turn_on_t turn_on;
        timeout_param = atoi(cmd_param);
        memcpy(turn_on.command,cmd_action,strlen(TURN_ON_CMD)+1);
        cout << timeout_param << "\n";
    }
    else if (strcmp(cmd_action,SCAN_SCHEDULE_CMD) == 0)
    {
        scan_schedule_t scan_schedule;
        timeout_param = atoi(cmd_param);
        memcpy(scan_schedule.command,cmd_action,strlen(SCAN_SCHEDULE_CMD)+1);
        cout << timeout_param << "\n";
    }
    else
    {
        cout << "You entered an invalid command !" << "\n";
    }
    return 1;
}

int main(int argc, char **argv)
{
    if(argc > 4)
    {
        cout << "Invalid arguments !" << "\n";
        return 0;
    }
    cout << "you have" << argc-1 << " arguments:" << "\n";
    
    for(int i = 1; i<argc; i++)
    {
        cout << i << ":" << argv[i] << "\n";
    }
    constructPacket(argv[1], argv[3]);

    return 1;
}