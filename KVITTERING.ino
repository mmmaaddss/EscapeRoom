#include <SoftwareSerial.h>
SoftwareSerial Thermal(2, 3);  //Soft RX from printer on D2, soft TX out to printer on D3

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
String backuptlf = "12345678";



void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);    //Use hardware serial
  Thermal.begin(19200);  //Setup soft serial for ThermalPrinter control

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
}

void loop() {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '1') {
      kvittering1();
    }
    else if (receivedChar == '2') {
      kvittering2();
    }
  }
}

void kvittering1() { //Kidnapnings kvittering
  Serial.println("Kvittering 1, printer nu!"); //Printer serial besked

  Thermal.println("            Rema420"); //Kvittering
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
  int randomNumre[32][20];
  Serial.println("Kvittering 2, printer nu!"); //Printer serial besked
  for (int baseArray = 0; baseArray < 32; baseArray++) {
    for (int secondaryArray = 0; secondaryArray < 20; secondaryArray++) {
      randomNumre[baseArray][secondaryArray] = random(1, 9);
    }
  }
  if (tlf.length() == 8) {
    Serial.println("[ThumbsUp] :)");
    randomNumre[6][2] = tlf[0];
    randomNumre[11][3] = tlf[1];
    randomNumre[8][6] = tlf[2];
    randomNumre[28][9] = tlf[3];

    randomNumre[18][19] = tlf[4];
    randomNumre[6][13] = tlf[5];
    randomNumre[12][16] = tlf[6];
    randomNumre[28][19] = tlf[7];
  }
  else {
    Serial.println("MEGA [ThumbsDown] :(");
    randomNumre[6][2] = tlf[0];
    randomNumre[11][3] = tlf[1];
    randomNumre[8][6] = tlf[2];
    randomNumre[28][9] = tlf[3];

    randomNumre[18][19] = tlf[4];
    randomNumre[6][13] = tlf[5];
    randomNumre[12][16] = tlf[6];
    randomNumre[28][19] = tlf[7];
  }
  for (int i = 0; i < 20; i++) {
    //her skal der therman printes 32 tegn for hver linje i arrary, (x aksen)
  }
  Thermal.write(10);  //feeder lidt kvittering
  Thermal.write(10);
}