#include <esp_now.h>
#include <WiFi.h>
#include "message_structures.h"

#define NUMWORKERS 4

// for loop function
int workercount = 0;

// varibles for reset function
bool reseting = false;
int reset_worker_count = 0;

// variabler til data som skal ud til de forskellige enhederne
char phonenumber[9] = "11111111";  // 9 because of space for the null character 93996237
bool game_running = true;
char name[100] = "Mads Baunbæk";
char postnummer[5] = "1234";
bool start_printer = false;



// MAC adresses                                                                                                     ↓ workers id
uint8_t printer_macAdress[] = { 0xD8, 0xBC, 0x38, 0xFD, 0x06, 0x98 };     // mac adresse D8:BC:38:FD:06:98      B   0
uint8_t numpad_macAdress[] = { 0xE4, 0x65, 0xB8, 0x75, 0xC2, 0xF4 };      // mac adresse E4:65:B8:75:C2:F4      C   1
uint8_t telefon_macAdress[] = { 0xA8, 0x42, 0xE3, 0xAE, 0xAB, 0xE0 };     // mac adresse A8:42:E3:AE:AB:E0      D   2
uint8_t elboks_macAdress[] = { 0xD8, 0xBC, 0x38, 0xFD, 0x37, 0xC4 };      // mac adresse D8:BC:38:FD:37:C4      E   3
uint8_t controller_macAdress[] = { 0xD8, 0xBC, 0x38, 0xFD, 0x42, 0x28 };  // mac adresse D8:BC:38:FD:42:28      A

// array for names in same order as mac adress listed:
String worker_names[] = {"printer", "numpad", "telefon", "elboks"};
bool worker_completion[] = {false, false, false, false};

// array for peer info for esp now protocall
esp_now_peer_info_t workers[NUMWORKERS] = {};

// definer data varibler for hver enhed
worker_printer_struct printerData;
worker_numpad_struct numpadData;
worker_telefon_struct telefonData;
worker_elboks_struct elboksData;

// variabel for data der kommer ind fra enhederne
incomming_worker_message statusData;

// recieve function
void OnDataRecv(const uint8_t * mac, const uint8_t *incommingData, int len) {
  memcpy(&statusData, incommingData, sizeof(statusData));
  
  /*Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Game completed: ");
  Serial.println(statusData.completed);*/

  if (statusData.completed) {
    worker_completion[workercount] = true;

    switch(workercount) {
      case 0:
        
        break;
      case 1:
        
        break;
      case 2:
        
        break;
      case 3:
        start_printer = true;
        break;
    }
  }

  workercount++;
}

// sent function
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  if (status != ESP_NOW_SEND_SUCCESS) {
    workercount++;
  }

  if (reseting) {
    if (status != ESP_NOW_SEND_SUCCESS) {
      Serial.print("RESET FAILED! for: ");
      Serial.println(worker_names[reset_worker_count]);
    } else {
      Serial.print("RESET COMPLETE! for: ");
      Serial.println(worker_names[reset_worker_count]);
    }
    reset_worker_count++;
    delay(1000);

    if (reset_worker_count >= 4) {
      reset_worker_count = 0;
      reseting = false;
    }

  } else {
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Active" : "Not Active");
  }
}

void call_worker(int workernumber) {
  esp_err_t result;
  switch(workernumber) {
    case 0:
      result = esp_now_send(workers[workernumber].peer_addr, (uint8_t *)&printerData, sizeof(printerData));
      break;
    case 1:
      result = esp_now_send(workers[workernumber].peer_addr, (uint8_t *)&numpadData, sizeof(numpadData));
      break;
    case 2:
      result = esp_now_send(workers[workernumber].peer_addr, (uint8_t *)&telefonData, sizeof(telefonData));
      break;
    case 3:
      result = esp_now_send(workers[workernumber].peer_addr, (uint8_t *)&elboksData, sizeof(elboksData));
      break;
  }
  
  Serial.print("Worker ");
  Serial.print(worker_names[workernumber]);
  Serial.print(":\t");

  if (worker_completion[workernumber]) {
    Serial.print("COMPLETE");
  } else {
    Serial.print("NOT COMPLETE");
  }

  Serial.print("\t");
}

void reset_workers() {
  reseting = true;

  printerData.game_running = false;
  numpadData.game_running = false;
  telefonData.game_running = false;
  elboksData.game_running = false;
  
  for (int i = 0; i < NUMWORKERS; i++) {
    call_worker(i);
    worker_completion[i] = false;
  }

  printerData.game_running = game_running;
  numpadData.game_running = game_running;
  telefonData.game_running = game_running;
  elboksData.game_running = game_running;
}


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);  // Set ESP32 as a Wi-Fi Station

  if (esp_now_init() != ESP_OK) {  // Initilize ESP-NOW
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // adding worker printer
  memcpy(workers[0].peer_addr, printer_macAdress, 6);
  workers[0].channel = 0;
  workers[0].encrypt = false;

  if (esp_now_add_peer(&workers[0]) != ESP_OK) {
    Serial.print("Failed to add worker: ");
    Serial.println(worker_names[0]);
    return;
  }

  // adding worker numpad
  memcpy(workers[1].peer_addr, numpad_macAdress, 6);
  workers[1].channel = 0;
  workers[1].encrypt = false;

  if (esp_now_add_peer(&workers[1]) != ESP_OK) {
    Serial.print("Failed to add worker: ");
    Serial.println(worker_names[1]);
    return;
  }
  // adding worker telefon
  memcpy(workers[2].peer_addr, telefon_macAdress, 6);
  workers[2].channel = 0;
  workers[2].encrypt = false;

  if (esp_now_add_peer(&workers[2]) != ESP_OK) {
    Serial.print("Failed to add worker: ");
    Serial.println(worker_names[2]);
    return;
  }
  // adding worker elboks
  memcpy(workers[3].peer_addr, elboks_macAdress, 6);
  workers[3].channel = 0;
  workers[3].encrypt = false;

  if (esp_now_add_peer(&workers[3]) != ESP_OK) {
    Serial.print("Failed to add worker: ");
    Serial.println(worker_names[3]);
    return;
  }

  // register callback functions
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  reset_workers();

  strcpy(telefonData.phonenumber, phonenumber);
  strcpy(printerData.name, name);
  strcpy(printerData.phonenumber, phonenumber);
  strcpy(printerData.postnummer, postnummer);
  
}



void loop() {
  telefonData.game_running = game_running;
  elboksData.game_running = game_running;
  printerData.game_running = game_running;

  printerData.print_numbers = start_printer;
  
  
  if (workercount >= 4) {
    workercount = 0;
    Serial.println();
  }
  call_worker(workercount);

  delay(500);

}




