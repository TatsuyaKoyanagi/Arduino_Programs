#define Joy_x A3
#define Joy_y A4
#define Joy_SW 12

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  double x = analogRead(Joy_x) / 2 - 256;
  double y = analogRead(Joy_y) / 2 - 256;
  double z = x / y;
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.println(z);
}
