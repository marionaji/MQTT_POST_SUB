#include "../include/post.h"
#include "../include/mqtt_cli.h"
#include "../include/mqtt_packet.h"
#include  <string.h>

int timeout_param;
int flag;

static mqtt_pkt packet;
uint8_t buf[256];
int pkt_len;


int verifyCmd(const char *cmd_action, const char *cmd_param)
{
    cmd_code_t cmd_code = INVALID;
    bool success = false;
    if (strcmp(cmd_action,DO_SCAN_CMD) == 0)
    {
       cmd_code = SCAN;
       timeout_param = atoi(cmd_param);
       memcpy(packet.do_scan.command,cmd_action,strlen(DO_SCAN_CMD)+1);
       packet.do_scan.timeout = timeout_param;
       success = true;
    }
    else if (strcmp(cmd_action,TURN_ON_CMD) == 0)
    {
        cmd_code = TURN_ON;
        timeout_param = atoi(cmd_param);
        memcpy(packet.turn_on.command,cmd_action,strlen(TURN_ON_CMD)+1);
        packet.turn_on.timeout = timeout_param;
        success = true;
    }
    else if (strcmp(cmd_action,SCAN_SCHEDULE_CMD) == 0)
    {
        cmd_code = SCHEDULE;
        memcpy(packet.scan_schedule.command,cmd_action,strlen(SCAN_SCHEDULE_CMD)+1);
        success = true;
    }
    else
    {
        printf("You entered an invalid command !\n");
    }
    pkt_len = constructPacket(buf,packet,cmd_code);
    return success;
}

int main(int argc, char **argv)
{
    int check;
    //args are cmd -p param topic clientId
 
    if(argc > MAX_CMD_LINE_ARGS)
    {
        printf("Invalid arguments !\n");
        return 0;
    }
    
    if (verifyCmd(argv[3],argv[4]))
    {
        mqtt_client_init(argv[2], buf, pkt_len);
        msq_init();
        create_msq_client(argv[1]);
        configure_callbacks();
        msq_connect();
    }
    else
    {
        printf("Operation Failed !");
        exit(0);
    }
       
    while (true)
    {
        check = msq_loop();
        fprintf(stderr, "Error: %s\n", strerror(check));
        std::cout << check << "\n";
    }
       
   
    return 0;
}