#ifndef _MQTT_CLI_H
#define _MQTT_CLI_H

#include <mosquitto.h>

extern bool gotOne; 

void on_connect(struct mosquitto *mosq, void *obj, int reason_code);
void on_publish(struct mosquitto *mosq, void *obj, int mid);
void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos);
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
void subscribe_topic();
void publish_data();
int msq_init();
int create_msq_client();
void configure_callbacks();
int msq_connect();
int msq_loop();
void msq_cleanup();

#endif