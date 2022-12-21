    #ifndef _MQTT_CLI_H
    #define _MQTT_CLI_H

    #include <mosquitto.h>
    
    void mqtt_client_init(const char *pub_topic, uint8_t *pay_load, int packet_len);
    void on_connect(struct mosquitto *mosq, void *obj, int reason_code);
    void on_publish(struct mosquitto *mosq, void *obj, int mid);
    void publish_data();
    int msq_init();
    int create_msq_client(const char*clientId);
    void configure_callbacks();
    int msq_connect();
    int msq_loop();
    void msq_cleanup();

    #endif