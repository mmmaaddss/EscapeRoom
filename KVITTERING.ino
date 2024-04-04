//https://blog.arduino.cc/2023/05/20/control-a-thermal-printer-with-your-arduino/ video med bold, billeder osv

#include <SoftwareSerial.h>
#include "Adafruit_Thermal.h"
#include "logo.h"
SoftwareSerial Thermal(2, 3);  //Soft RX from printer on D2, soft TX out to printer on D3
Adafruit_Thermal printer(&Thermal); // Use the same SoftwareSerial for the Adafruit_Thermal object

#define FALSE 0
#define TRUE 1
int printOnBlack = FALSE;
int printUpSideDown = FALSE;

int ledPin = 13;
int heatTime = 255;        //80 is default from page 23 of datasheet. Controls speed of printing and darkness
int heatInterval = 255;    //2 is default from page 23 of datasheet. Controls speed of printing and darkness
char printDensity = 15;    //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
char printBreakTime = 15;  //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
String birthday = "25-05-2004";
String tidspunkt = "13:56"; 
String tlf = "93996237";
unsigned char randomNumre[32][20];

void setup() {
  pinMode(ledPin, OUTPUT);

  for (int baseArray = 0; baseArray < 32; baseArray++) {
    for (int secondaryArray = 0; secondaryArray < 20; secondaryArray++) {
      randomNumre[baseArray][secondaryArray] = char(random(1, 9));
    }
  }
    randomNumre[6][2] = tlf[0]-48;
    randomNumre[11][3] = tlf[1]-48;
    randomNumre[8][6] = tlf[2]-48;
    randomNumre[28][9] = tlf[3]-48;

    randomNumre[18][19] = tlf[4]-48;
    randomNumre[6][13] = tlf[5]-48;
    randomNumre[12][16] = tlf[6]-48;
    randomNumre[28][19] = tlf[7]-48;

  Serial.begin(9600);    //Use hardware serial
  Thermal.begin(19200);  //Setup soft serial for ThermalPrinter control
  printer.begin(); // Initialize the Adafruit_Thermal printer

  printOnBlack = FALSE;
  printUpSideDown = FALSE;

  //Modify the print speed and heat
  Thermal.write(27);
  Thermal.write(55);
  Thermal.write(7);             //Default 64 dots = 8*('7'+1)
  Thermal.write(heatTime);      //Default 80 or 800us
  Thermal.write(heatInterval);  //Default 2 or 20us

  //Modify the print density and timeout
  Thermal.write(18);
  Thermal.write(35);
  int printSetting = (printDensity << 4) | printBreakTime;
  Thermal.write(printSetting);  //Combination of printDensity and printBreakTime
  Serial.println("Ready!");
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
  printer.printBitmap(logo_width, logo_height, logo_data); // Printer logo
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
  Thermal.write(10);  //feeder lidt kvittering
  Thermal.write(10);
}

void kvittering2() { //Random tal kvittering
  Serial.println("Kvittering 2, printer nu!"); //Printer serial besked
  
  if (tlf.length() == 8) {
    Serial.println("[ThumbsUp] :)");

  }
  else {
    Serial.println("MEGA [ThumbsDown] :(");
   
  }
  for (int i = 0; i<20; i++) {
    Thermal.println();
    for (int o = 0; o<32; o++) {
      Thermal.print(randomNumre[i][o],10);
//Serial.print('.');
    }
  }
  Thermal.write(10);  //feeder lidt kvittering
  Thermal.write(10);
}
