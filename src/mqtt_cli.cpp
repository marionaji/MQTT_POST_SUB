#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <filesystem>
#include "util.h"
#include "../include/mqtt_cli.h"

bool is_client_connected = false;
struct mosquitto *mosq;
int rc;

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
        subscribe_topic();
        // publish_data();
    }
}

/* Callback called when the broker sends a SUBACK in response to a SUBSCRIBE. */
void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	int i;
	bool have_subscription = false;

	/* In this example we only subscribe to a single topic at once, but a
	 * SUBSCRIBE can contain many topics at once, so this is one way to check
	 * them all. */
	for(i=0; i<qos_count; i++){
		printf("on_subscribe: %d:granted qos = %d\n", i, granted_qos[i]);
		if(granted_qos[i] <= 2){
			have_subscription = true;
		}
	}
	if(have_subscription == false){
		/* The broker rejected all of our subscriptions, we know we only sent
		 * the one SUBSCRIBE, so there is no point remaining connected. */
		fprintf(stderr, "Error: All subscriptions rejected.\n");
		mosquitto_disconnect(mosq);
	}
}

/* Callback called when the client receives a message. */
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	/* This blindly prints the payload, but the payload can be anything so take care. */
	printf("%s %d %s\n", msg->topic, msg->qos, (char *)msg->payload);
    std::ostringstream command;
    command << "/usr/local/bin/aws s3 cp --profile " << "ehsan.tahmasebian" << " " << std::string(((char *) msg->payload)) << " " << "/home/don/test_aws_download";
    std::cout<<"executing "<<command.str()<<std::endl;
    std::cout<<exec(command.str().c_str());

    std::cout<<"update firmware"<<std::endl;
    std::ostringstream flash_command;
    flash_command << "/usr/bin/avrdude -p atmega2560 -c wiring -P /dev/ttyACM0 -b 115200 -D -U flash:w:/home/don/test_aws_download/firmware.hex:i";
    std::cout<<"executing "<<flash_command.str()<<std::endl;
    std::cout<<exec(flash_command.str().c_str());
}


/* Callback called when the client knows to the best of its abilities that a
 * PUBLISH has been successfully sent. For QoS 0 this means the message has
 * been completely written to the operating system. For QoS 1 this means we
 * have received a PUBACK from the broker. For QoS 2 this means we have
 * received a PUBCOMP from the broker. */
void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
	printf("Message with mid %d has been published.\n", mid);
}

void subscribe_topic()
{
    /* Making subscriptions in the on_connect() callback means that if the
    * connection drops and is automatically resumed by the client, then the
    * subscriptions will be recreated when the client reconnects. */
	rc = mosquitto_subscribe(mosq, NULL, "wer", 2);
	if(rc != MOSQ_ERR_SUCCESS){
		fprintf(stderr, "Error subscribing: %s\n", mosquitto_strerror(rc));
		/* We might as well disconnect if we were unable to subscribe */
		mosquitto_disconnect(mosq);
	}
}

/* This function pretends to read some data from a sensor and publish it.*/
void publish_data()
{
	// char *payload = "HELLO!";
	char *payload = "s3://cs-main/jetson-test/95105c07-68dd-4c3f-b642-742fa9eee797/firmware.hex";
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
	rc = mosquitto_publish(mosq, NULL, "wer", strlen(payload), payload, 2, false);
	// rc = mosquitto_publish(mosq, NULL, "ehsantah/feeds/test_json", strlen(payload), payload, 2, false);
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

int create_msq_client()
{
    mosq = mosquitto_new("round_1",false,NULL);
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
    mosquitto_subscribe_callback_set(mosq, on_subscribe);
	mosquitto_message_callback_set(mosq, on_message);
}

int msq_connect()
{
    rc = mosquitto_username_pw_set(mosq,"123456","123456");
    // rc = mosquitto_username_pw_set(mosq,"ehsantah","aio_xIGd75a3Ux9YH9VgLiNVVo7Es4wz");
    if(rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        std::cout << " im here 1" << "\n";
        return 1;
    }
    rc = mosquitto_tls_opts_set(mosq,1,NULL, NULL);
    if(rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        std::cout << " im here 2" << "\n";
        return 1;
    }
    rc = mosquitto_tls_insecure_set(mosq,false);
    if(rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        std::cout << " im here 3" << "\n";
        return 1;
    }
    std::filesystem::path pwd = std::filesystem::current_path();
    std::filesystem::path cert = pwd/"myqtt.pem";
    std::cout << cert << std::endl;
    rc = mosquitto_tls_set(mosq,NULL,"/etc/ssl/certs",NULL,NULL,NULL);
    if(rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        std::cout << " im here 4" << "\n";
        return 1;
    }

    rc = mosquitto_connect(mosq, "node02.myqtthub.com",8883, 120);
    // rc = mosquitto_connect(mosq, "io.adafruit.com", 1883, 120);
    if(rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        std::cout << " im here 5" << "\n";
        return 1;
    }

    /* Run the network loop in a background thread, this call returns quickly. */
    return 0;
}

int msq_loop()
{
   
   return mosquitto_loop_start(mosq);

}

void msq_cleanup()
{
    mosquitto_disconnect(mosq);
    mosquitto_lib_cleanup();
}


	
