#define inpin1 11
#define inpin2 10
#define inpin3 8
#define inpin4 9
#define outpin1 2 //sort
#define outpin2 3 //gul
#define outpin3 4 //grøn
#define outpin4 5 //rød

int wins = 0;
const int arraylen = 4;

int inpins[arraylen] = {inpin1, inpin2, inpin3, inpin4};
int outpins[arraylen] = {outpin1, outpin2, outpin3, outpin4};

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
