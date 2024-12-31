#include <esp_now.h>
#include <WiFi.h>

int RELAY = 39;
typedef struct struct_message {
    int id;
    char command[3];
} struct_message;

struct_message myData;
unsigned long lastHeartbeatTime = 0; 


void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  if (strcmp(myData.command, "ON") == 0) {
    digitalWrite(RELAY, HIGH);
  } else if (strcmp(myData.command, "OFF") == 0) {
    digitalWrite(RELAY, LOW);
  }
  else if (strcmp(myData.command, "HB") == 0) {
    lastHeartbeatTime = millis(); 


  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW); 

  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);


}


void loop() {

}