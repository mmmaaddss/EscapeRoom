#define inpin1 1
#define inpin2 2
#define inpin3 3
#define outpin1 4
#define outpin2 5
#define outpin3 6
int wins = 0;
const int arraylen = 3;

int inpins[arraylen] = {inpin1, inpin2, inpin3};
int outpins[arraylen] = {outpin1, outpin2, outpin3};

void setup() {
  for (int i = 0; i < arraylen; i++) {
    pinMode(inpins[i], INPUT_PULLUP);
    pinMode(outpins[i], OUTPUT);
    digitalWrite(outpins[i], HIGH);
  }
  Serial.begin(9600);
}

void checkConnections() {
  wins = 0;
  for (int i = 0; i < arraylen; i++) {
    if (digitalRead(inpins[i]) == LOW) {
      digitalWrite(outpins[i], LOW);
      wins++;
    } else {
      digitalWrite(outpins[i], HIGH); 
    }
  }

  if (wins == arraylen) {
    Serial.println("WUHUUU ALL PINS ARE CONNECTED");
  }
}

void loop() {
  checkConnections();
  delay(500);// En lille forsinkelse for at undgå for mange serielle udskrifter.
}