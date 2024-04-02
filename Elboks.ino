#define inpin1 3
#define inpin2 5
#define inpin3 6
#define outpin1 9
#define outpin2 10
#define outpin3 11
int wins = 0;
const int arraylen = 3;

int inpins[arraylen] = {inpin1, inpin2, inpin3};
int outpins[arraylen] = {outpin1, outpin2, outpin3};

void setup() {
  for (int i = 0; i < arraylen; i++) {
    pinMode(inpins[i], INPUT_PULLUP);
    pinMode(outpins[i], OUTPUT);
    digitalWrite(outpins[i], LOW);
  }
  Serial.begin(9600);
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
  }
  Serial.println();
}

void loop() {
  checkConnections();
  delay(500);// En lille forsinkelse for at undgå for mange serielle udskrifter.
}
