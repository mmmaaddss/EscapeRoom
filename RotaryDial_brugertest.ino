#define dialPin 4
#define pulsePin A0

// library for groove lcd screen
#include "rgb_lcd.h"

rgb_lcd lcd;

// const values for lcd screen for user test
const int colorR = 255;
const int colorG = 200;
const int colorB = 200;

int pulseCounter = 0;
String phone_number = ""; // stores final phone number
String numbers = ""; // stores all numbers entered since last restart

bool rolling = true; // decides if rolling numbers are active mainly used for usertest


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0, 1);


  pinMode(dialPin, INPUT_PULLUP);
  pinMode(pulsePin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  bool dialing = !digitalRead(dialPin); // check if the rotary dial is used

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

    if (rolling) {
      if (numbers.length() > 8) { // if theres is entered more than 8 numbers the phonenumber should be the last 8 numbers entered
        phone_number = "";
        for (int i = 8; i > 0 ; i--) { 
          phone_number += numbers[numbers.length() - i]; // select the last 8 numbers for the phonenumber
        }
      } else {
        phone_number = numbers;  
      }
    } else {
      phone_number = numbers;
    }

    // print the phone number to the lcd screen for the user test
    lcd.clear();
    lcd.print(phone_number);
    Serial.println(phone_number);
    delay(30);
    pulseCounter = 0;
  }
}












