#include <Motoron.h> //download from https://github.com/pololu/motoron-arduino
#include <motoron_protocol.h>

#include <Arduino.h>
#include <WiFi.h>
#include <Motoron.h>
#include <FastLED.h>
#include "pin_def.h"
#define ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include "nanocraw_ctrl/CrawlerCmd.h"

#define LEFT_CRAWLER  2
#define RIGHT_CRAWLER 1

void messageCb( const nanocraw_ctrl::CrawlerCmd& cmd_msg );
void error_loop();
void wifi_setup();

//Wi-Fi configration
const char SSID[] = "SSID";
const char PASS[] = "password";
IPAddress  HOST_IP( 192,168,8,211 ); //IP address of host (where roscore is running)
const uint16_t serverPort = 11411;
WiFiClient client;

//ros node handle
ros::NodeHandle nh;
ros::Subscriber<nanocraw_ctrl::CrawlerCmd> sub("nanocraw_cmd", &messageCb);

nanocraw_ctrl::CrawlerCmd cmd_val;
long recieve_time = 0;

//Motoron M2T256
MotoronI2C mc;

//LED
CRGB leds[1];

class WiFiHardware {
  public:
    WiFiHardware() {};
    void init() {
      client.connect(HOST_IP, serverPort);
    };
    int read() {
      return client.read();
    };
    void write(uint8_t* data, int length) {
      for(int i=0; i<length; i++)
        client.write(data[i]);
    };
    unsigned long time() {
      return millis();
    };
};

void setup() {
  //serial
  Serial.begin(115200);

  //LED setup
  FastLED.addLeds<SK6812, PIN_LED, RGB>(leds, 1);

  //motor driver setting
  Wire.begin();
  mc.reinitialize();    // Bytes: 0x96 0x74
  mc.disableCrc();      // Bytes: 0x8B 0x04 0x7B 0x43
  mc.clearResetFlag();  // Bytes: 0xA9 0x00 0x04
  digitalWrite(PIN_RST,HIGH);
  mc.setSpeed(LEFT_CRAWLER,  0);    // Speed: -800~800
  mc.setSpeed(RIGHT_CRAWLER, 0);
  // // Configure motor 1
  // mc.setMaxAcceleration(1, 200);
  // mc.setMaxDeceleration(1, 300);

  // // Configure motor 2
  // mc.setMaxAcceleration(2, 200);
  // mc.setMaxDeceleration(2, 300);

  //wi-fi
  wifi_setup();
  
  //ros init
  // Set the connection to rosserial socket server
  nh.getHardware()->setConnection(HOST_IP, serverPort);
  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  if(!nh.connected()){
    digitalWrite( PIN_LED, HIGH );
    //disable motor driver
    mc.setSpeed(LEFT_CRAWLER,  0);
    mc.setSpeed(RIGHT_CRAWLER, 0);
  }else{
    //lifetime of received data
    // if (millis()-recieve_time > 100){
    //   cmd_val.left  = 0;
    //   cmd_val.right = 0;
    // }
    if (digitalRead(PIN_EROR)==LOW){ 
      //enable motor driver
      mc.setSpeed(LEFT_CRAWLER,  constrain(cmd_val.left,  -800, 800));
      mc.setSpeed(RIGHT_CRAWLER, constrain(cmd_val.right, -800, 800));
    }
    else{ //motor driver reports an error
      //disable motor driver
      Serial.println("motor driver reports an error!");
      mc.setSpeed(LEFT_CRAWLER,  0);
      mc.setSpeed(RIGHT_CRAWLER, 0);
      leds[0] = 0x008000; 
      FastLED.show();
    }
  }
  nh.spinOnce();
  delay(1);
}

void messageCb( const nanocraw_ctrl::CrawlerCmd& cmd_msg ){
  Serial.printf("left : %d, right : %d\n", cmd_msg.left, cmd_msg.right);
  cmd_val.left  = cmd_msg.left;
  cmd_val.right = cmd_msg.right;
  recieve_time = millis();
}

void error_loop(){
  while (1){
        digitalWrite( PIN_LED, !digitalRead( PIN_LED ) );
        delay( 500 );
    }
}

void wifi_setup(){
  WiFi.begin(SSID,PASS);
      Serial.print("WiFi connecting");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("....");
      leds[0] = 0xFF0000; 
      FastLED.show();
      delay( 500 );
      leds[0] = 0x00F000;
      FastLED.show();
      delay( 500 );      
    }
    Serial.printf("connected! spent time %0.2f[s]\n", millis()*0.001);
    Serial.print(WiFi.localIP());
    Serial.printf(" to access client\n");
    delay(1000);
    leds[0] = 0xFF0000; 
    FastLED.show();
}

