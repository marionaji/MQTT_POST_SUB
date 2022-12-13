#ifndef _POST_H
#define _POST_H

#include <string.h>
#include <iostream>
#include <pb_encode.h>
#include <pb_decode.h>
#include "mqttpkt.pb.h"

#define TURN_ON_CMD        ((const char*) "turn-on")
#define DO_SCAN_CMD        ((const char*) "do-scan")
#define SCAN_SCHEDULE_CMD  ((const char*)  "u-scan")
#define MAX_CMD_LINE_ARGS  5

typedef struct 
{
    char command[strlen(DO_SCAN_CMD)+1];
    int timeout;
} do_scan_t;

typedef struct 
{
    char command[strlen(TURN_ON_CMD)+1];
    int timeout;  
} turn_on_t;

typedef struct 
{
    char command[strlen(SCAN_SCHEDULE_CMD)+1];
} scan_schedule_t;

int verifyCmd(const char *cmd_action, const char *cmd_param);


#endif