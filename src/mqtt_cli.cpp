#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/mqtt_cli.h"
#include "../include/post.h"

bool is_client_connected = false;
struct mosquitto *mosq;
int rc;
const char *pubTopic;
uint8_t *payload;
int mqtt_payload_len;

void mqtt_client_init(const char *pub_topic, uint8_t *pay_load, int packet_len)
{
    pubTopic = pub_topic;
    payload = pay_load;
    mqtt_payload_len = packet_len;
    std::cout << mqtt_payload_len << std::endl;
}

void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
	printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
	if(reason_code != 0)
    {
        is_client_connected = false;
		mosquitto_disconnect(mosq);
	}
    else
    {
        is_client_connected = true;
        publish_data();
    }
}

/* Callback called when the client knows to the best of its abilities that a
 * PUBLISH has been successfully sent. For QoS 0 this means the message has
 * been completely written to the operating system. For QoS 1 this means we
 * have received a PUBACK from the broker. For QoS 2 this means we have
 * received a PUBCOMP from the broker. */
void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
	printf("Message with mid %d has been published.\n", mid);
    exit(0);
}

/* This function pretends to read some data from a sensor and publish it.*/
void publish_data()
{
    for (int i = 0; i < mqtt_payload_len; i++)
    {
        std::cout << std::hex << (int)payload[i] << std::dec << std::endl;
    }
    
	//const char *payload = "hey buddy";
	/* Publish the message
	 * mosq - our client instance
	 * *mid = NULL - we don't want to know what the message id for this message is
	 * topic = "example/temperature" - the topic on which this message will be published
	 * payloadlen = strlen(payload) - the length of our payload in bytes
	 * payload - the actual payload
	 * qos = 2 - publish with QoS 2 for this example
	 * retain = false - do not use the retained message feature for this message
	 */
    std::cout << " publishing message..." << "\n";
	rc = mosquitto_publish(mosq, NULL,pubTopic,mqtt_payload_len,payload, 2, false);
	if(rc != MOSQ_ERR_SUCCESS)
    {
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
	}
}

/* Required before calling other mosquitto functions */
int msq_init()
{
    return mosquitto_lib_init();
}

/* Create a new client instance.
    * id = NULL -> ask the broker to generate a client id for us
    * clean session = true -> the broker should remove old sessions when we connect
    * obj = NULL -> we aren't passing any of our private data for callbacks
    */

int create_msq_client(const char*clientId)
{
    mosq = mosquitto_new(clientId,false,NULL);
    if(mosq == NULL)
    {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }
    return 0;
}

/* Configure callbacks. This should be done before connecting ideally. */

void configure_callbacks()
{
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_publish_callback_set(mosq, on_publish);
}

int msq_connect()
{
    rc = mosquitto_username_pw_set(mosq,"123456","123456");
    if(rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        std::cout << " im here 1" << "\n";
        return 1;
    }
    rc = mosquitto_tls_set(mosq,"mqttmy.pem",NULL,NULL,NULL,NULL);
    if(rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        std::cout << " im here 2" << "\n";
        return 1;
    }

    rc = mosquitto_connect(mosq, "node02.myqtthub.com",8883, 120);
    if(rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        std::cout << " im here 3" << "\n";
        return 1;
    }

    /* Run the network loop in a background thread, this call returns quickly. */
    return 0;
}

int msq_loop()
{
   return mosquitto_loop(mosq,10000,1);
}

void msq_cleanup()
{
    mosquitto_lib_cleanup();
}


	
