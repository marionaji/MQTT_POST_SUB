#ifndef _POST_H
#define _POST_H

#include <string.h>
#include <iostream>
#include <stdio.h>

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


typedef union
{
    do_scan_t do_scan;
    turn_on_t turn_on;
    scan_schedule_t scan_schedule;
} mqtt_pkt;

typedef enum
{
    SCAN,
    TURN_ON,
    SCHEDULE,
    INVALID
} cmd_code_t;

#endif