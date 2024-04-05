#include "Adafruit_Thermal.h"
#include "adalogo.h"
#include "adaqrcode.h"
// Here's the new syntax when using SoftwareSerial (e.g. Arduino Uno) ----
// If using hardware serial instead, comment out or remove these lines:

#include "SoftwareSerial.h"
#define TX_PIN 3 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 2 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal Thermal(&mySerial);     // Pass addr to printer constructor

int ledPin = 13;
int heatTime = 255;        //80 is default from page 23 of datasheet. Controls speed of printing and darkness
int heatInterval = 255;    //2 is default from page 23 of datasheet. Controls speed of printing and darkness
char printDensity = 15;    //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
char printBreakTime = 15;  //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
String birthday = "25-05-2004";
String tidspunkt = "13:56"; 
String tlf = "11111111";

const unsigned int rows = 14;
const unsigned int columns = 16;
unsigned char randomNumre[rows][columns];

void setup() {
  // NOTE: SOME PRINTERS NEED 9600 BAUD instead of 19200, check test page.
  mySerial.begin(19200);  // Initialize SoftwareSerial
  //Serial1.begin(19200); // Use this instead if using hardware serial
  Thermal.begin();        // Init printer (same regardless of serial type)
  Serial.begin(9600);

  for (int baseArray = 0; baseArray < rows; baseArray++) {
     Serial.print("Row ");
     Serial.print(baseArray+1);
           Serial.print(": ");


    for (int secondaryArray = 0; secondaryArray < columns; secondaryArray++) {
      Serial.print(secondaryArray+1);
      Serial.print(" ");
      randomNumre[baseArray][secondaryArray] = char(random(0, 10));
    }
    Serial.println();
  }

int offsetx=4;
int offsety=3;
for (int i = 0; i < tlf.length(); i++) {
    randomNumre[offsety][offsetx+i] = tlf[i]-48;
}
    
  Serial.println("Ready!");

//dvehdbe

  Thermal.sleep();      // Tell printer to sleep
  delay(3000L);         // Sleep for 3 seconds
  Thermal.wake();       // MUST wake() before printing again, even if reset
  Thermal.setDefault(); // Restore printer to defaults
}

void loop() {
  while (!Serial.available());

    char receivedChar = Serial.read();
    Serial.print("Got: "); Serial.println(receivedChar);
    if (receivedChar == '1') {
      kvittering1();
    }
    else if (receivedChar == '2') {
      kvittering2();
    }
}

void kvittering1() { //Kidnapnings kvittering
  Serial.println("Kvittering 1, printer nu!"); //Printer serial besked
  //printer.printBitmap(logo_width, logo_height, logo_data); // Printer logo
  Thermal.println("");
  Thermal.println("");
  Thermal.println("             Aarhus");
  Thermal.println("  Dollerupvej 2    8000 Aarhus");
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
      Thermal.print(randomNumre[i][o],10);
      
//Serial.print('.');
    }
    Thermal.println();
    
  }

  Thermal.println("");
  Thermal.println("");
}
