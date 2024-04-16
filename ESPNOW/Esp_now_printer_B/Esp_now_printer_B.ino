#include "Adafruit_Thermal.h"
//#include "adalogo.h"
//#include "adaqrcode.h"
// Here's the new syntax when using SoftwareSerial (e.g. Arduino Uno) ----
// If using hardware serial instead, comment out or remove these lines:

#include "SoftwareSerial.h"
#define TX_PIN 16 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 17 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal Thermal(&mySerial);     // Pass addr to printer constructor

bool print_kvittering1 = false;
bool print_kvittering2 = false;


int ledPin = 13;
int heatTime = 255;        //80 is default from page 23 of datasheet. Controls speed of printing and darkness
int heatInterval = 255;    //2 is default from page 23 of datasheet. Controls speed of printing and darkness
char printDensity = 15;    //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
char printBreakTime = 15;  //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
String birthday = "25-05-2004";
String tidspunkt = "13:56"; 
String tlf = "11111111";
String postnummer = "1234";
String navn = "John doe";

bool receipt_printed = false;

const unsigned int rows = 14;
const unsigned int columns = 16;
int randomNumre[rows][columns];

int offsetx_tlf=4;
int offsety_tlf=3;

int offsetx_spaces = offsetx_tlf;
int offsety_spaces = rows - offsety_tlf;

//------------------------------------------------------------------------------

#include <esp_now.h>
#include <WiFi.h>
#include "message_structures.h"

bool game_completed = false;
bool game_running = false;

// controller macadresse D8:BC:38:FD:42:28
uint8_t controller_macAdress[] = { 0xD8, 0xBC, 0x38, 0xFD, 0x42, 0x28 };

// define data structures
worker_printer_struct printerData;
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
  memcpy(&printerData, incommingData, sizeof(printerData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Game runnning: ");
  Serial.println(printerData.game_running);
  Serial.print("phonenmumber: ");
  Serial.println(printerData.phonenumber);
  Serial.print("name: ");
  Serial.println(printerData.name);
  Serial.print("postnummer: ");
  Serial.println(printerData.postnummer);
  Serial.print("print_numbers: ");
  Serial.println(printerData.print_numbers);

  if (!printerData.game_running) {
    game_completed = false;
  }

  Serial.print("game completed: ");
  Serial.println(game_completed);
  Serial.print("game running: ");
  Serial.println(game_running);

  if (printerData.game_running) {
    if (!game_running) {
      print_kvittering1 = true;
      //delay(10000);
      //game_running = printerData.game_running;
      receipt_printed = false;
    }
  }
  game_running = printerData.game_running;

  tlf = printerData.phonenumber;
  for (int i = 0; i < tlf.length(); i++) {
    randomNumre[offsety_tlf][offsetx_tlf+i] = tlf[i]-48;
  }
  navn = String(printerData.name);
  postnummer = printerData.postnummer;
  if (!receipt_printed) {
    if (printerData.print_numbers) {
      receipt_printed = printerData.print_numbers;
      print_kvittering2 = true;
    }
  }



  

  send_return_data();
}

// sent function
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//------------------------^ for ESP-NOW ^---------------------------------

void setup() {
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

  // NOTE: SOME PRINTERS NEED 9600 BAUD instead of 19200, check test page.
  mySerial.begin(19200);  // Initialize SoftwareSerial
  //Serial1.begin(19200); // Use this instead if using hardware serial
  Thermal.begin();        // Init printer (same regardless of serial type)
  Serial.begin(9600);

  for (int baseArray = 0; baseArray < rows; baseArray++) {
     Serial.print("Row ");
     //Serial.print(baseArray+1);
           Serial.print(": ");


    for (int secondaryArray = 0; secondaryArray < columns; secondaryArray++) {
      //Serial.print(secondaryArray+1);
      Serial.print(" ");
      randomNumre[baseArray][secondaryArray] = int(random(0, 10));
    }
    Serial.println();
  }


  for (int i = 0; i < tlf.length(); i++) {
    randomNumre[offsety_tlf][offsetx_tlf+i] = tlf[i]-48;
  }
    
  Serial.println("Ready!");

//dvehdbe

  Thermal.sleep();      // Tell printer to sleep
  delay(3000L);         // Sleep for 3 seconds
  Thermal.wake();       // MUST wake() before printing again, even if reset
  Thermal.setDefault(); // Restore printer to defaults
}

void loop() {
  /*
  while (!Serial.available());

    char receivedChar = Serial.read();
    Serial.print("Got: "); Serial.println(receivedChar);
    if (receivedChar == '1') {
      kvittering1();
    }
    else if (receivedChar == '2') {
      kvittering2();
    }*/

  if (print_kvittering1) {
    kvittering1();
    print_kvittering1 = false;
  }
  if (print_kvittering2) {
    kvittering2();
    print_kvittering2 = false;
  }
}

void kvittering1() { //Kidnapnings kvittering
  Thermal.setSize('S');        // Set type size, accepts 'S', 'M', 'L'

  Serial.println("Kvittering 1, printer nu!"); //Printer serial besked
  //printer.printBitmap(logo_width, logo_height, logo_data); // Printer logo
  Thermal.println("");
  Thermal.println("");
  Thermal.println("             Aarhus");
  Thermal.println("  Dollerupvej 2    "+postnummer+" Aarhus");
  Thermal.println("      Aben Man-Sondag 8-20");
  Thermal.println("");
  Thermal.println("");
  Thermal.println("2x Gaffatape              149,90");
  Thermal.println("20pk Strips                20,00");
  Thermal.println("Kloroform                  79,95");
  Thermal.println("Karklude                   19,95");
  Thermal.println("");
  Thermal.println("Total                     269,80");
  Thermal.println("");
  Thermal.println("Dankort                   769,80");
  Thermal.println("XXXXXXXXXXXX9976");
  Thermal.println("Byttepenge                500,00");
  Thermal.println("");
  Thermal.println("");
  Thermal.println("Heraf 25% moms             53,96");
  Thermal.println("Moms total                 53,96");
  Thermal.println("");
  Thermal.println("Du blev betjent af:");
  Thermal.println(navn);
  Thermal.println("");
  Thermal.println("       "+birthday+" "+tidspunkt);
  Thermal.println("");
  Thermal.println("");
}

void kvittering2() { //Random tal kvittering
  Thermal.setSize('L');        // Set type size, accepts 'S', 'M', 'L'
  Serial.println("Kvittering 2, printer nu!"); //Printer serial besked
  
  
  if (tlf.length() == 8) {
    Serial.println("[ThumbsUp] :)");

  }
  else {
    Serial.println("MEGA [ThumbsDown] :(");
   
  }
    Thermal.println();

  for (int i = 0; i<rows; i++) {
    for (int o = 0; o<columns; o++) {
      if (i == 9 && o >= offsetx_tlf && o <= offsetx_spaces + 7) {
        Thermal.print(" ");
      } else {
        Thermal.print(randomNumre[i][o],10);
      }
//Serial.print('.');
    }
    Thermal.println();
    
  }

  Thermal.println("");
  Thermal.println("");

  game_completed = true;
}