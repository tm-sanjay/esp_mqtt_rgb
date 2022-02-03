
const char *wifi_ssid = "SSID"; 
const char *wifi_password = "password"; 
const char *mqtt_server = "mqtt.example.com"; 
const char *mqtt_user = "12345678";
const char *mqtt_password = "12345678";
const int   mqtt_port = 1883;
const char *device_id = "esp8266";
const char *device_topic = "esp8266/led_control/#";

// Number of led in the strip
#define NUM_LEDS 5
#define DATA_PIN D2