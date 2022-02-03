#include <FastLED.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <config.h>
#include <inttypes.h>

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<1024> doc;

char device_command[10];
bool STATE = false;

String hsv = "";
String rgb = "";

// Define the array of leds
CRGB leds[NUM_LEDS];

// wifi setup
void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void strreplace(char s[], char chr, char repl_chr) 
{
  int i=0;
  while(s[i]!='\0') {
    if(s[i]==chr)
    {
      s[i]=repl_chr;
    }  
    i++; 
  }
}

void setRGB(u_int8_t bright, uint8_t r, uint8_t g, uint8_t b) {
  Serial.printf("brightness: %d \n" ,bright);
  Serial.printf("colorR: %d \n" ,r);
  Serial.printf("colorG: %d \n" ,g);
  Serial.printf("colorB: %d \n" ,b);
  FastLED.setBrightness(bright);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(r, g, b);
    FastLED.show();
  }
}

void setHSV(uint8_t h, uint8_t s, uint8_t v) {
  Serial.printf("hue: %d \n" ,h);
  Serial.printf("saturation: %d \n" ,s);
  Serial.printf("value: %d \n" ,v);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(h, s, v);
    FastLED.show();
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  strreplace(topic,'/',' ');
  sscanf(topic,"%*s %*s %s", device_command); //
  Serial.println(topic);
  Serial.printf("device_command: %s \n" ,device_command);

  DeserializationError error = deserializeJson(doc, payload);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  /*
  Example message for rgb
  {
    "state" : true,
    "rgb":"180,255,0,0"
  }
  Example message for hsv
  {
    "state" : true,
    "hsv":"80,255,150"
  }
  */

  if (strcmp(device_command, "set") == 0) {
    
    STATE = doc["state"].as<int>();
    rgb = doc["rgb"].as<String>();
    hsv = doc["hsv"].as<String>();

    if (STATE == true) {
      //! for some reason, the legth of string is 4 after json, even if it is empty
      if(rgb.length() > 4) {
        int bright, r, g, b;
        sscanf(rgb.c_str(), "%d,%d,%d,%d", &bright, &r, &g, &b);
        setRGB(bright, r, g, b);
      }
      else if(hsv.length() > 4) {
        int h, s, v;
        sscanf(hsv.c_str(), "%d,%d,%d", &h, &s, &v);
        setHSV(h, s, v);
      }
    }
  }
}

//mqtt reconnect
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(device_id, mqtt_user, mqtt_password))
    { 
    Serial.println("connected");
    client.subscribe(device_topic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.print("WA2812 LED");

  setup_wifi(); //comment after connected to WiFi

  client.setServer(mqtt_server, mqtt_port); // change port number as mentioned in your cloudmqtt console
  client.setCallback(callback);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
//  FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);
  
}

void loop() { 
  yield();
  if (!client.connected())
  {
    reconnect();
  }

  delay(200);
  
  client.loop();
  // for (int i = 0; i < NUM_LEDS; i++) {
  //   leds[i] = CHSV(hue+(i*10), 255, 255);
  //   FastLED.show();
  // }
  // EVERY_N_MILLISECONDS(5) {
  //   hue++;
  // }
  FastLED.show();
}
