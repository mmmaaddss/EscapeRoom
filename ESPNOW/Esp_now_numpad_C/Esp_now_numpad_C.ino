#include "SoftwareSerial.h"
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

String receivedSequence = ""; // Initialize the received input sequence


//------------------------------------------------------------------------------

#include <esp_now.h>
#include <WiFi.h>
#include "message_structures.h"

bool game_completed = false;
bool game_running = false;

// controller macadresse D8:BC:38:FD:42:28
uint8_t controller_macAdress[] = { 0xD8, 0xBC, 0x38, 0xFD, 0x42, 0x28 };

// define data structures
worker_numpad_struct numpadData;
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
  memcpy(&numpadData, incommingData, sizeof(numpadData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Game runnning: ");
  Serial.println(numpadData.game_running);

  if (!numpadData.game_running) {
    game_completed = false;
  }

  Serial.print("game completed: ");
  Serial.println(game_completed);
  Serial.print("game running: ");
  Serial.println(game_running);

  if (numpadData.game_running) {
    if (!game_running) {
      // reset and start
      receivedSequence = "";
      lcd.begin(16,2);
      lcd.print("Kortets ende:"); // Display initial message on the LCD
      lcd.setCursor(0, 2); // Set the cursor position to the second line of the LCD
    }
  }
  game_running = numpadData.game_running;

  send_return_data();
}

// sent function
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//------------------------^ for ESP-NOW ^---------------------------------

const int buzzerPin = 26;

SoftwareSerial mySerial(17,16);

String inputSequence = "9976"; // Define the expected input sequence
bool inputCompleted = false; // Flag to indicate if input sequence is completed

void setup() {
    mySerial.begin(9600); // Initialize SoftwareSerial for communication with the numpad
    Serial.begin(9600); // Initialize serial communication for debugging

    //lcd.begin(16, 2); // Initialize the LCD display
    lcd.print("Kortets ende"); // Display initial message on the LCD
    lcd.setCursor(0, 2); // Set the cursor position to the second line of the LCD

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

void loop() {
  if (game_running && !game_completed) {

    printData(); // Call the function to handle incoming data from the numpad
    if (inputCompleted) { // Check if the input sequence is completed
        if (receivedSequence.equals(inputSequence)) { // Check if the received sequence matches the expected sequence
            game_completed = true;
            lcd.clear(); // Clear the LCD display
            lcd.print("Matched!"); // Display "Matched!" on the LCD
            //game_running = false;
        } else {
            lcd.clear(); // Clear the LCD display
            lcd.print("Not matched!"); // Display "Not matched!" on the LCD
            delay(2000);
            lcd.clear(); // Clear the LCD display
            lcd.print("Kortets ende"); // Display initial message on the LCD
            lcd.setCursor(0, 2); // Set the cursor position to the second line of the LCD
            receivedSequence = "";
        }
        inputCompleted = false; // Reset the flag for input completion
    }
  }
}

void reset() {
  receivedSequence = "";
  lcd.clear(); // Clear the LCD display
  lcd.print("Kortets ende:"); // Display initial message on the LCD
  lcd.setCursor(0, 2); // Set the cursor position to the second line of the LCD
  playTone1();
}

void printData() {
    static int index = 0; // Initialize the index for tracking received inputs
    if (mySerial.available()) { // Check if there is data available from the numpad
        uint8_t data = mySerial.read(); // Read the incoming data
        String ciffer; // Declare a string to store the received digit
        switch(data) { // Handle the received digit
            case 0xE1: ciffer = "1"; playTone(); break; // If '1' is received, set 'ciffer' to "1" and play tone
            case 0xE2: ciffer = "2"; playTone(); break; // If '2' is received, set 'ciffer' to "2" and play tone
            case 0xE3: ciffer = "3"; playTone(); break; // If '3' is received, set 'ciffer' to "3" and play tone
            case 0xE4: ciffer = "4"; playTone(); break; // If '4' is received, set 'ciffer' to "4" and play tone
            case 0xE5: ciffer = "5"; playTone(); break; // If '5' is received, set 'ciffer' to "5" and play tone
            case 0xE6: ciffer = "6"; playTone(); break; // If '6' is received, set 'ciffer' to "6" and play tone
            case 0xE7: ciffer = "7"; playTone(); break; // If '7' is received, set 'ciffer' to "7" and play tone
            case 0xE8: ciffer = "8"; playTone(); break; // If '8' is received, set 'ciffer' to "8" and play tone
            case 0xE9: ciffer = "9"; playTone(); break; // If '9' is received, set 'ciffer' to "9" and play tone
            case 0xEA: reset(); playTone2(); index--; break; // If 'reset' is received, call 'reset()' function and play tone
            case 0xEB: ciffer = "0"; playTone(); break; // If '0' is received, set 'ciffer' to "0" and play tone
            case 0xEC: playTone1(); inputCompleted = true; index--; break; // If '#' is received, call 'playTone1()' function and set 'inputCompleted' flag
            default: break; // Handle other cases (unused in this code)
        }
        lcd.print(ciffer); // Display the received digit on the LCD
        receivedSequence += ciffer; // Append the received digit to the received sequence
        index++; // Increment the index
        if (receivedSequence.length() > 4) { // Check if the received sequence exceeds 8 digits
            reset();
        }
    }
}

void playTone() {
    tone(buzzerPin, 6000); // Play a tone at 6000 Hz
    delay(50); // Wait for 50 milliseconds
    noTone(buzzerPin); // Turn off the tone
}

void playTone2() {
    tone(buzzerPin, 600); // Play a tone at 600 Hz
    delay(500); // Wait for 500 milliseconds
    noTone(buzzerPin); // Turn off the tone
}

void playTone1() {
    tone(buzzerPin, 1000); // Play a tone at 1000 Hz
    delay(800); // Wait for 800 milliseconds
    noTone(buzzerPin); // Turn off the tone
}
