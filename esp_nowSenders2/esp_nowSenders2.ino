


/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SoftwareSerial.h> //$

#define BUF_SIZE 250
SoftwareSerial mySerial(13, 15);

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0xCD, 0x59, 0x64};

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;  // send readings timer

typedef struct struct_message {
  char a[BUF_SIZE];
} struct_message;

struct_message myData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  mySerial.begin(19200); //$


  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  delay(1000);

  Serial.println("Hello I am Connected to ESP NOW"); //$

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
 String IncomingString="";
 boolean StringReady = false;
 
 while (mySerial.available()){
   IncomingString=mySerial.readString();
   StringReady= true;
  }
 
  if (StringReady){
    IncomingString.toCharArray(myData.a, BUF_SIZE);
     Serial.println(myData.a);
      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }
}
