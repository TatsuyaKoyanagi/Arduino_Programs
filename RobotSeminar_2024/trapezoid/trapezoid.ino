void setup() {
  // put your setup code here, to run once:
  pinMode(5, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int input = 0;
  while (digitalRead(5) == LOW) {
    if (input < 255) {
      input += 1;
    }
    Serial.println(input);
    delay(10);
  }
}
