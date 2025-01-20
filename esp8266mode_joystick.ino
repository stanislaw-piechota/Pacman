const int X_pin = A0;
const int Y_pin = A1;
char choice = 'L';

void setup() {
  Serial.begin(9600);
}

void loop() {
  int x = map(analogRead(X_pin), 0, 1023, -10, 10);
  int y = map(analogRead(Y_pin), 0, 1023, 10, -10);
  
  if ((x <= -8) && (y <= 5) && (y >= -5) && (choice != 'L')){
    choice = 'L';
    Serial.println("L");
  }
  else if ((x >= 8) && (y <= 5) && (y >= -5) && (choice != 'R')) {
    choice = 'R';
    Serial.println("R");
  }
  else if ((y <= -8) && (x <= 5) && (x >= -5) && (choice != 'D')){
    choice = 'D';
    Serial.println("D");
  }
  else if ((y >= 8) && (x <= 5) && (x >= -5) && (choice != 'U')){
    choice = 'U';
    Serial.println("U");
  }
  delay(50);
}