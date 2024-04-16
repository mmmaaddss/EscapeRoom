#include <esp_now.h>
#include <WiFi.h>
#include "message_structures.h"

bool game_completed = false;
bool game_running = false;

// controller macadresse D8:BC:38:FD:42:28
uint8_t controller_macAdress[] = { 0xD8, 0xBC, 0x38, 0xFD, 0x42, 0x28 };

// define data structures
worker_elboks_struct elboksData;
incomming_worker_message statusData;

esp_now_peer_info_t controller;

// answer function
void send_return_data() {
  statusData.completed = game_completed;

  // Send message via ESP-NOW
    esp_err_t result = esp_now_send(controller.peer_addr, (uint8_t *)&statusData, sizeof(statusData));

    if (result == !ESP_OK) {
      Serial.println("Sending error");
    }
}

// recieve function
void OnDataRecv(const uint8_t * mac, const uint8_t *incommingData, int len) {
  memcpy(&elboksData, incommingData, sizeof(elboksData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Game runnning: ");
  Serial.println(elboksData.game_running);

  game_running = elboksData.game_running;

  if (!game_running) {
    game_completed = false;
  }

  send_return_data();
}

// sent function
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//------------------------^ for ESP-NOW ^---------------------------------

#define inpin1 33 // blå - skal forbindes til sort
#define inpin2 5 // blå - skal forbindes til gul
#define inpin3 19 // blå - skal forbindes til grøn
#define inpin4 26 // blå - skal forbindes til rød
#define outpin1 21 //sort
#define outpin2 27 //gul
#define outpin3 17 //grøn
#define outpin4 16 //rød

int wins = 0;
const int arraylen = 4;

int inpins[arraylen] = {inpin1, inpin2, inpin3, inpin4};
int outpins[arraylen] = {outpin1, outpin2, outpin3, outpin4};

void setup() {
  for (int i = 0; i < arraylen; i++) {
    pinMode(inpins[i], INPUT_PULLUP);
    pinMode(outpins[i], OUTPUT);
    digitalWrite(outpins[i], HIGH);
  }
  Serial.begin(115200);

  //-------------------------------------------------------------------------
  WiFi.mode(WIFI_STA); // Set ESP32 as a Wi-Fi Station

  if (esp_now_init() != ESP_OK) { // Initilize ESP-NOW
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent); 

  // Register peer
  memcpy(controller.peer_addr, controller_macAdress, 6);
  controller.channel = 0;  
  controller.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&controller) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  //------------------------^ for ESP-NOW ^---------------------------------
}

void checkConnections() {
  wins = 0;
  for (int i = 0; i < arraylen; i++) {
    digitalWrite(outpins[i], LOW);
    Serial.println("Pin #" + String(i+1) + ":" + String(!digitalRead(inpins[i])));
    if (digitalRead(inpins[i]) == LOW) {
      wins++;
    }
      digitalWrite(outpins[i], HIGH); 
  }

  if (wins == arraylen) {
    Serial.println("WUHUUU ALL PINS ARE CONNECTED");
    game_completed = true;
  }
  Serial.println();
}

void loop() {
  checkConnections();
  delay(500);// En lille forsinkelse for at undgå for mange serielle udskrifter.
}