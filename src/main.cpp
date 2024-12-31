#include <Arduino.h>
#include <esp_now.h>


uint8_t broadcastAddress[] = {0x48, 0x27, 0xE2, 0x51, 0xD6, 0x56};
typedef struct struct_message {
    int id;
    char command[3];
} struct_message;

// Create a struct_message called myData
struct_message myData;

#include <WiFi.h>
#include <HTTPClient.h>


WiFiClient client;
HTTPClient http;

#include <Adafruit_NeoPixel.h>



#define RGBPin 6     //WS2812 LED Ring Pin
#define numPixels 12  //Number of LEDS in the ring, which is 12 in this case

#define pir1 2          //Motion Sensor 1
#define pir2 5          //Motion Sensor 2
#define pir3 4          //Motion Sensor 3
#define pwrButtonPin 40  //Power-on pwrButtonPin
#define OTAButtonPin 18
#define WiFiButtonPin 7
#define buzzerPin 39


#define pwrLed 3  //pwrButtonPin's built-in LED

Adafruit_NeoPixel pixels(numPixels, RGBPin, NEO_GRB + NEO_KHZ800);


unsigned long lastHeartbeatTime = 0; 
unsigned long millisValue;

int motion1 = 0, motion2 = 0, motion3 = 0, pwrButtonPinState = 0;
boolean noMotion = true, nopwrButtonPinPress = true, timeOut = false, toneFlag = false;
boolean flag = false;
boolean isOnServerCalled = false;

long elapsedTime;


const char* onServer = "ON";
const char* offServer = "OFF";
const char* heartbeat = "HB";


// Forward declarations
void pwrButtonPressed();
void OTAButtonPressed();
void WiFiButtonPressed();
void setup();
void loop();
void powerOn();
void powerOff();
void checkMotionrunLight();
void timeIndicator(long T);
void sendRequest(const char* server);
void sendHeartbeat(const char* server);
void resetVariables();

void pwrButtonPressed(){
pwrButtonPinState = true;
Serial.println("Power button has been pressed");
}

void OTAButtonPressed(){

Serial.println("OTA button has been pressed");

}



void setup() {
  //Serial.begin(115200);
  pinMode(RGBPin, OUTPUT);
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(pir3, INPUT);
  pinMode(pwrButtonPin, INPUT);
  pinMode(pwrLed, OUTPUT);
  digitalWrite(pwrLed, LOW);  //To make sure the pwrButtonPin's LED is not lit.

  tone(buzzerPin, 660, 100);

Serial.println("Beginning");

  pixels.begin();
  pixels.setBrightness(255);  //Use to adjust the brightness of the LED ring
  pixels.clear();




  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
      pixels.clear();


  sendRequest(offServer); //Making sure the switch is off, in case it was turned on from before. We are not calling offServer() to avoid the triggering the shutdown tone.
  tone(buzzerPin, 660, 100);

delay(10000);
  tone(buzzerPin, 660, 300);



  delay(60000);  //Allowing the PIR sensors to initialize

  tone(buzzerPin, 660, 300);
  delay(200);
  tone(buzzerPin, 660, 300);
  delay(200);
  tone(buzzerPin, 660, 300);

}


// Main loop
void loop() 
{
    motion1 = digitalRead(pir1);
    motion2 = digitalRead(pir2);
    motion3 = digitalRead(pir3);
    sendHeartbeat(heartbeat);

    pwrButtonPinState = digitalRead(pwrButtonPin); // Read the state of the power button

    if (pwrButtonPinState == HIGH) { // If the power button is pressed
      pwrButtonPressed(); // Call the function to handle power button press
    }

    if (pwrButtonPinState && !flag) {  //Debouncing delay
      delay(500);
        tone(buzzerPin, 660, 100);

      pwrButtonPinState = false;
      flag = true;
      timeOut = false;
    }
    if (flag && !timeOut) {
      powerOn();
    } else {
      flag = false;
    }

  


  }
  


// Function to power on the LEDs
void powerOn() {

  digitalWrite(pwrLed, HIGH);
  checkMotionrunLight();
}

// Function to power off the LEDs
void powerOff() {
  tone(buzzerPin, 1046, 1000);
  digitalWrite(pwrLed, LOW);
  resetVariables();
  sendRequest(offServer);
}

// Function to check for motion and run the light accordingly

void checkMotionrunLight() {
  if (motion1 || motion2 || motion3) {
    noMotion = false;
    millisValue = millis();
    timeIndicator(0);
    if (!isOnServerCalled) {
      sendRequest(onServer);
      isOnServerCalled = true;
    }
  }
  if (!motion1 && !motion2 && !motion3 && !noMotion) {
    noMotion = true;
    millisValue = millis();
  }
  if (noMotion) {
    elapsedTime = millis() - millisValue;
    timeIndicator(millis() - millisValue);
  }

}
// Function to indicate time passed with the LEDs
void timeIndicator(long T) {
  if (T >= 0 && T < (10000)) {
    for (int i = 0; i <= numPixels; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      pixels.show();
    }
  }
  if (T > (10000) && T < (60000)) {
    for (int i = 0; i <= numPixels; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 250));
      pixels.show();
    }
  }
  if (T > (60000) && T < (240000)) {
    for (int i = 0; i <= numPixels; i++) {
      pixels.setPixelColor(i, pixels.Color(150, 150, 0));
      pixels.show();
    }
  }
  if (T > (240000) && T < (300000)) {
    if (!toneFlag) {
      tone(buzzerPin, 622, 300);
      tone(buzzerPin, 622, 300);
      tone(buzzerPin, 622, 300);
      toneFlag = true;
    }
    for (int i = 0; i <= numPixels; i++) {
      pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      pixels.show();
    }
  }
  if (T > (300000)) {
    tone(buzzerPin, 1046, 1000);
    for (int i = 0; i <= numPixels; i++) {
      pixels.clear();
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
    }
    powerOff();
    timeOut = true;
  }
}

void sendRequest(const char* server) {
  myData.id = 1;
  strcpy(myData.command, server);
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}

void sendHeartbeat(const char* server) {
  myData.id = 1;
  strcpy(myData.command, server);
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}

// Function to reset state+flag variables
void resetVariables() {
  flag = false;
  isOnServerCalled = false;
  noMotion = true;
  timeOut = false;
  toneFlag = false;
}
