#include "SoftwareSerial.h"
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int buzzerPin = 26;

SoftwareSerial mySerial(17,16);

String inputSequence = "23344567"; // Define the expected input sequence
String receivedSequence = ""; // Initialize the received input sequence
bool inputCompleted = false; // Flag to indicate if input sequence is completed

void setup() {
    mySerial.begin(9600); // Initialize SoftwareSerial for communication with the numpad
    Serial.begin(9600); // Initialize serial communication for debugging

    lcd.begin(16, 2); // Initialize the LCD display
    lcd.print("hello, world!"); // Display initial message on the LCD
    lcd.setCursor(0, 2); // Set the cursor position to the second line of the LCD
}

void loop() {
    printData(); // Call the function to handle incoming data from the numpad
    if (inputCompleted) { // Check if the input sequence is completed
        if (receivedSequence.equals(inputSequence)) { // Check if the received sequence matches the expected sequence
            lcd.clear(); // Clear the LCD display
            lcd.print("Matched!"); // Display "Matched!" on the LCD
        } else {
            lcd.clear(); // Clear the LCD display
            lcd.print("Not matched!"); // Display "Not matched!" on the LCD
        }
        inputCompleted = false; // Reset the flag for input completion
    }
}

void reset() {
    lcd.clear(); // Clear the LCD display
    lcd.print("hello, world!"); // Display initial message on the LCD
    lcd.setCursor(0, 2); // Set the cursor position to the second line of the LCD
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
        if (receivedSequence.length() > 8) { // Check if the received sequence exceeds 8 digits
            receivedSequence = ""; // Clear the received sequence
            lcd.setCursor(0, 2); // Set the cursor position to the second line of the LCD
            lcd.print("                "); // Clear the second line of the LCD
            lcd.setCursor(0, 2); // Set the cursor position to the second line of the LCD
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
}num
