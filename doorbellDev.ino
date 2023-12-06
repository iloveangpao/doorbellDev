#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <Arduino.h>
#include <esp32-hal-timer.h>
#include "WifiConnection.h"
#include "MyServo.h"
#include "DigitalSensor.h"
#include "PinDefinitions.h"
#include "Buzzer.h"
#include "config.h"
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

DigitalSensor pir(PIR_PIN, 1);
DigitalSensor button(BUTTON_PIN, 1);
DigitalSensor reed(REED_PIN, 0);
Buzzer buzzer(BUZZER_PIN);
MyServo servoBlue(BLUESERVO_PIN);
MyServo servoBlack(BLACKSERVO_PIN);
const unsigned char defaultWrite[NUMACTUATOR] = {0, 0, 0};
enum Actuator {DELIVERY = 0, VISITOR = 1, BUZZER = 2};
volatile short status[NUMACTUATOR] = {0,0,0};
volatile bool alarmActive = false;
volatile bool motionDetected = false;
volatile unsigned char reedTick = 0;

WiFiConnection connection(wifi_ssid, wifi_password);

//---- HiveMQ Cloud Broker settings are defined in config.h

WiFiClientSecure espClient;  
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (500)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(length);

  if (strcmp(topic, "doorbell/instruction") == 0 && length >= 2) {
    Serial.println("rec mqtt");
    Serial.println(payload[0]);
    Actuator actuator = static_cast<Actuator>(payload[0]); // Assuming Actuator is an enum
    unsigned char value = payload[1];
    Serial.println(payload[1]);
    switch (actuator) {
      case DELIVERY:
        doorAction(DELIVERY, value);
        break;
      case VISITOR:
        doorAction(VISITOR, value);
        break;
      case BUZZER:
        if(value == 0) {
          buzzer.offBuzzer();
          status[BUZZER] = 0;
          Serial.println('buzzerOff');
        }
        break;
      default:
        Serial.println("unknown actuator");
        defaultActuator();
        break;
    }
    
  }

  delay(1000);
  doorAction(VISITOR, 0);
  doorAction(DELIVERY, 0);
}


void reconnect() {
  // Loop until we’re reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection… ");
    String clientId = "ESP32Client";
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected!");
      // Once connected, publish an announcement…
      client.publish("testTopic", "Hello World!");
      // … and resubscribe
      client.subscribe("doorbell/instruction");
    } else {
      Serial.print("failed, rc = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void doorAction(Actuator actuator, bool lock){
  if (actuator == VISITOR){
    if (lock)
    servoBlue.setAngle(0);
    else 
    servoBlue.setAngle(180);
  }
  if(actuator == DELIVERY){
    if (lock)
    servoBlack.setAngle(0);
    else 
    servoBlack.setAngle(180);
  }
  status[actuator] = !lock;
  Serial.print("lock");
  Serial.println(lock);
  Serial.print("status:");
  Serial.println(status[VISITOR]);
}

void defaultActuator(){
  servoBlack.setAngle(defaultWrite[DELIVERY]);
  servoBlue.setAngle(defaultWrite[VISITOR]);
  buzzer.offBuzzer();
  status[VISITOR] = 0;
  status[DELIVERY] = 0;
  status[BUZZER] = 0;
}

void ARDUINO_ISR_ATTR reedISR(){
  if((status[VISITOR])) {
    // buzzer.onBuzzer();
    // status[BUZZER] = 1;
    alarmActive = true; // Set the flag to activate the timer-based alarm
  }
}


void setup() {
  Serial.begin(9600);
  Serial.println("start");
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable   detector
  reed.initInterrupt(reedISR, false);
  Serial.println("setISR");

  connection.connect();

  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(pir.read()) {
    if(!motionDetected) client.publish("doorbell/reading", "someone outside");
  }
  motionDetected = pir.read();
  if(button.read()) {
    while(button.read());
    client.publish("doorbell/reading", "visitor");
    buzzer.setTune();
  }
  buzzer.play();

  
  if(alarmActive){
    Serial.println("active");
    Serial.println(reedTick);
    if(!(reed.read())) reedTick ++;
    else reedTick = 0;
    if(reedTick > 2) {
      client.publish("doorbell/reading", "intruder");
      buzzer.onBuzzer();
      status[BUZZER] = 1;
      reedTick = 0;
      alarmActive = 0;
  }
  }
  Serial.print("visitor:");
  Serial.println(status[VISITOR]);
  Serial.print("pir");
  Serial.println(pir.read());
  Serial.print("reeed");
  Serial.println(reed.read());
  delay(50);
  
}

