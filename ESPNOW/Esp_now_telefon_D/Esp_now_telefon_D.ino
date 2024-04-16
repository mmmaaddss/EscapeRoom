#include <esp_now.h>
#include <WiFi.h>
#include "message_structures.h"

bool game_completed = false;

#define dialPin 16
#define pulsePin 17

#define speakerpin 26

// controller macadresse D8:BC:38:FD:42:28
uint8_t controller_macAdress[] = { 0xD8, 0xBC, 0x38, 0xFD, 0x42, 0x28 };  // mac adresse 2842FD38BCD8         A


int pulseCounter = 0;
String phone_number = ""; // stores final phone number
String numbers = ""; // stores all numbers entered since last restart
String correct_phone_number = "12345678"; // temp phone number
bool game_running = false;

// define data structure for incomming data
worker_telefon_struct telefonData;
incomming_worker_message statusData;

esp_now_peer_info_t controller;

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
  memcpy(&telefonData, incommingData, sizeof(telefonData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Phone number: ");
  Serial.println(telefonData.phonenumber);
  Serial.print("Game runnning: ");
  Serial.println(telefonData.game_running);

  game_running = telefonData.game_running;
  correct_phone_number = String(telefonData.phonenumber);

  // reset game if game_running is false
  if (!telefonData.game_running) {
    game_completed = false;
    pulseCounter = 0;
    phone_number = "";
    numbers = "";
    correct_phone_number = "12345678";
  }

  send_return_data();
}

// sent function
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



void setup() {
  Serial.begin(115200);

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

  pinMode(dialPin, INPUT_PULLUP);
  pinMode(pulsePin, INPUT_PULLUP);
  pinMode(speakerpin, OUTPUT);
  digitalWrite(speakerpin, HIGH);
}

void loop() {
  bool dialing = !digitalRead(dialPin); // check if the rotary dial is used
  if (game_running) {
    while (dialing) { // start counting pulses when rotary dialy is 
    delay(30); //debounce correction

    int pulse = digitalRead(pulsePin); // check for pulse is registered

    if (pulse) {
      delay(30); //debounce correction
      pulseCounter++; // count 1 pulse if a pulse i registeret

      while(pulse) {
        pulse = digitalRead(pulsePin);
        // wait to pulse is over
      }
    }
    dialing = !digitalRead(dialPin); // check dialing status again
    }

    if (pulseCounter > 0) { // when the rotary dial stops rotating the number should be stored 
    // 1 pulse is equivalent to the number 1 and so on... except 10 is equivalent to the number 10
    //      the modulus opperator ↓ ensures this
    numbers += String(pulseCounter%10);

    if (numbers.length() > 8) { // if theres is entered more than 8 numbers the phonenumber should be the last 8 numbers entered
      phone_number = "";
      for (int i = 8; i > 0 ; i--) { 
        phone_number += numbers[numbers.length() - i]; // select the last 8 numbers for the phonenumber
      }
    } else {
      phone_number = numbers;  
    }

    Serial.println(phone_number);
    delay(30);
    pulseCounter = 0;
    } 

    // update varible game_complete when correct phone number is entered
    if (phone_number == correct_phone_number) {
      game_completed = true;
      Serial.println("Correct phone number registeret");
      digitalWrite(speakerpin, LOW);
      delay(30);
      digitalWrite(speakerpin, HIGH);
      phone_number = "";
      numbers = "";
    }
  }
}
