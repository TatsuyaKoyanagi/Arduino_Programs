#define SPEED 80
#define TURN_SPEED 80
#define speedPinR 9   // Front Wheel PWM pin connect Model-Y M_B ENA
#define RightMotorDirPin1  22    //Front Right Motor direction pin 1 to Model-Y M_B IN1  (K1)
#define RightMotorDirPin2  24   //Front Right Motor direction pin 2 to Model-Y M_B IN2   (K1)
#define LeftMotorDirPin1  26    //Front Left Motor direction pin 1 to Model-Y M_B IN3 (K3)
#define LeftMotorDirPin2  28   //Front Left Motor direction pin 2 to Model-Y M_B IN4 (K3)
#define speedPinL 10   // Front Wheel PWM pin connect Model-Y M_B ENB
#define speedPinRB 11   // Rear Wheel PWM pin connect Left Model-Y M_A ENA
#define RightMotorDirPin1B  5    //Rear Right Motor direction pin 1 to Model-Y M_A IN1 ( K1)
#define RightMotorDirPin2B 6    //Rear Right Motor direction pin 2 to Model-Y M_A IN2 ( K1)
#define LeftMotorDirPin1B 7    //Rear Left Motor direction pin 1 to Model-Y M_A IN3  (K3)
#define LeftMotorDirPin2B 8  //Rear Left Motor direction pin 2 to Model-Y M_A IN4 (K3)
#define speedPinLB 12    // Rear Wheel PWM pin connect Model-Y M_A ENB
#define RightObstacleSensor 2  //Right obstacle sensor to D2 (front direction is from arduino point to voltage meter)
#define LeftObstacleSensor 3   //Left obstacle sensor to D3
// Micro switch pins
#define FrontSwitch1 49 // マイクロスイッチ 4
#define FrontSwitch2 50 // マイクロスイッチ 5
#define RightSwitch1 51 // マイクロスイッチ 6
#define RightSwitch2 52 // マイクロスイッチ 7
#define RearSwitch1 53  // マイクロスイッチ 1
#define RearSwitch2 46  // マイクロスイッチ 9
#define LeftSwitch1 47  // マイクロスイッチ 2
#define LeftSwitch2 48  // マイクロスイッチ 3
// Ultrasonic sensor pins
#define trigPinFront 34
#define echoPinFront 35
#define trigPinRight 36
#define echoPinRight 37
#define trigPinRear 38
#define echoPinRear 39
#define trigPinLeft 40
#define echoPinLeft 41
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#define BNO055_ADDRESS 0x28 // BNO055のI2Cアドレス（SDAを20番ピン、SCLを21番ピンに接続した場合）
Adafruit_BNO055 bno = Adafruit_BNO055(BNO055_ADDRESS, 0x28);
/* Motor control functions */
// Front-right wheel forward turn
void FR_fwd(int speed) {
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  analogWrite(speedPinR, speed);
}
// Front-right wheel backward turn
void FR_bck(int speed) {
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  analogWrite(speedPinR, speed);
}
// Front-left wheel forward turn
void FL_fwd(int speed) {
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  analogWrite(speedPinL, speed);
}
// Front-left wheel backward turn
void FL_bck(int speed) {
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  analogWrite(speedPinL, speed);
}
// Rear-right wheel forward turn
void RR_fwd(int speed) {
  digitalWrite(RightMotorDirPin1B, HIGH);
  digitalWrite(RightMotorDirPin2B, LOW);
  analogWrite(speedPinRB, speed);
}
// Rear-right wheel backward turn
void RR_bck(int speed) {
  digitalWrite(RightMotorDirPin1B, LOW);
  digitalWrite(RightMotorDirPin2B, HIGH);
  analogWrite(speedPinRB, speed);
}
// Rear-left wheel forward turn
void RL_fwd(int speed) {
  digitalWrite(LeftMotorDirPin1B, HIGH);
  digitalWrite(LeftMotorDirPin2B, LOW);
  analogWrite(speedPinLB, speed);
}
// Rear-left wheel backward turn
void RL_bck(int speed) {
  digitalWrite(LeftMotorDirPin1B, LOW);
  digitalWrite(LeftMotorDirPin2B, HIGH);
  analogWrite(speedPinLB, speed);
}
// Move forward
void forward(int speed_left, int speed_right) {
  RL_fwd(speed_left);
  RR_fwd(speed_right);
  FR_fwd(speed_right);
  FL_fwd(speed_left);
}
// Reverse
void reverse(int speed) {
  RL_bck(speed);
  RR_bck(speed);
  FR_bck(speed);
  FL_bck(speed);
}
// Right shift
void right_shift(int speed) {
  FL_fwd(speed);
  RL_bck(speed);
  RR_fwd(speed);
  FR_bck(speed);
}
// Left shift
void left_shift(int speed) {
  FL_bck(speed);
  RL_fwd(speed);
  RR_bck(speed);
  FR_fwd(speed);
}
// Left turn
void left_turn(int speed) {
  RL_bck(0);
  RR_fwd(speed);
  FR_fwd(speed);
  FL_bck(0);
}
// Right turn
void right(int speed) {
  RL_fwd(speed);
  RR_bck(0);
  FR_bck(0);
  FL_fwd(speed);
}
// Stop
void stop_Stop() {
  analogWrite(speedPinLB, 0);
  analogWrite(speedPinRB, 0);
  analogWrite(speedPinL, 0);
  analogWrite(speedPinR, 0);
}
void init_GPIO() {
  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  pinMode(RightMotorDirPin1B, OUTPUT);
  pinMode(RightMotorDirPin2B, OUTPUT);
  pinMode(LeftMotorDirPin1B, OUTPUT);
  pinMode(LeftMotorDirPin2B, OUTPUT);
  pinMode(speedPinRB, OUTPUT);
  pinMode(speedPinLB, OUTPUT);
  pinMode(RightObstacleSensor, INPUT);
  pinMode(LeftObstacleSensor, INPUT);
  // Initialize micro switch pins with internal pull-up resistors
  pinMode(FrontSwitch1, INPUT_PULLUP);
  pinMode(FrontSwitch2, INPUT_PULLUP);
  pinMode(RightSwitch1, INPUT_PULLUP);
  pinMode(RightSwitch2, INPUT_PULLUP);
  pinMode(RearSwitch1, INPUT_PULLUP);
  pinMode(RearSwitch2, INPUT_PULLUP);
  pinMode(LeftSwitch1, INPUT_PULLUP);
  pinMode(LeftSwitch2, INPUT_PULLUP);
  // Initialize ultrasonic sensor pins
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
  pinMode(trigPinRear, OUTPUT);
  pinMode(echoPinRear, INPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
}
long readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1;
  return distance;
}
void setup() {
  Serial.begin(9600);
  // BNO055センサーの初期化
  if (!bno.begin())
  {
    Serial.println("Failed to initialize BNO055 sensor!");
    while (1);
  }
  delay(1000); // 1秒待機してセンサーの初期化が安定するのを待つ
}
void loop() {
  sensors_event_t event;
  bno.getEvent(&event);
  // 姿勢データからロール角を取得
  float roll = event.orientation.x;
  // ロール角の制御例
  if (roll > 10) {
    // 左に傾いている場合、左側のモーターを強めにする
    FL_fwd(SPEED + 40); // 例えば、左前方のモーターを少し強める
    RL_fwd(SPEED + 40); // 左後方のモーターも少し強める
  } else if (roll < -10) {
    // 右に傾いている場合、右側のモーターを強めにする
    FR_fwd(SPEED + 40); // 右前方のモーターを少し強める
    RR_fwd(SPEED + 40); // 右後方のモーターも少し強める
  } else {
    // ロール角がない場合、直進する
    forward(SPEED, SPEED);
  }
  delay(50); // 動作の安定のための短い待ち時間
}