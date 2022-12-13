#include "../include/post.h"
#include "../include/mqtt_cli.h"
#include  <string.h>
using namespace std;

int timeout_param;
int flag;

int verifyCmd(const char *cmd_action, const char *cmd_param)
{
    bool success = false;
    if (strcmp(cmd_action,DO_SCAN_CMD) == 0)
    {
       do_scan_t do_scan;
       timeout_param = atoi(cmd_param);
       memcpy(do_scan.command,cmd_action,strlen(DO_SCAN_CMD)+1);
       do_scan.timeout = timeout_param;
       success = true;
    }
    else if (strcmp(cmd_action,TURN_ON_CMD) == 0)
    {
        turn_on_t turn_on;
        timeout_param = atoi(cmd_param);
        memcpy(turn_on.command,cmd_action,strlen(TURN_ON_CMD)+1);
        turn_on.timeout = timeout_param;
        success = true;
    }
    else if (strcmp(cmd_action,SCAN_SCHEDULE_CMD) == 0)
    {
        scan_schedule_t scan_schedule;
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
    int check;
    //args are cmd -p param topic clientId
 
    // if(argc > MAX_CMD_LINE_ARGS)
    // {
    //     cout << "Invalid arguments !" << "\n";
    //     return 0;
    // }
    
    
    msq_init();
    create_msq_client();
    configure_callbacks();
    msq_connect();
       

    // do
    // {
    //     check = msq_loop();
    //     fprintf(stderr, "Error: %s\n", strerror(check));
    //     std::cout << check << "\n";
        
    // } while (check == MOSQ_ERR_SUCCESS);
       

       while (true)
       {
         check = msq_loop();
         fprintf(stderr, "Error: %s\n", strerror(check));
         std::cout << check << "\n";
       }
       
   
    return 0;
}