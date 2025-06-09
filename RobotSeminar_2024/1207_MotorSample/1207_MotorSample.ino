#define MotorPower_R 4
#define MotorPower_L 9

#define MotorF_R 5
#define MotorB_R 6

#define MotorF_L 7
#define MotorB_L 8

#define Joy_x A3
#define Joy_y A4

#define FR 13
#define FL 12
#define RR 10
#define RL 11

#define button 3

float a1 = 0.5;
float a2 = 0.25;
float b0 = 1.0;
int x = 0;

float y1 = 0.0;
float y2 = 0.0;

int tmp = 0;
int axis_x = 0;
int axis_y = 0;

int input_speed_FR = 100;
int input_speed_RR = 100;
int input_speed_FL = 100;
int input_speed_RL = 100;

void setup() {
  // put your setup code here, to run once:
  pinMode(MotorPower_R, OUTPUT);
  pinMode(MotorPower_L, OUTPUT);
  pinMode(MotorF_R, OUTPUT);
  pinMode(MotorF_L, OUTPUT);
  pinMode(MotorB_R, OUTPUT);
  pinMode(MotorB_L, OUTPUT);
  pinMode(FR, INPUT_PULLUP);
  pinMode(FL, INPUT_PULLUP);
  pinMode(RR, INPUT_PULLUP);
  pinMode(RL, INPUT_PULLUP);
  analogWrite(MotorPower_R, 0);
  analogWrite(MotorPower_L, 0);
  stop();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //なめらかな加減速
  button_trapezoid_v2();
  //急加減速
  //moving();
}

void moving() {
  if (digitalRead(FR) == LOW) {
    FR_t(1, 255);
  } else {
    FR_t(0, 0);
  }
  if (digitalRead(FL) == LOW) {
    FL_t(1, 255);
  } else {
    FL_t(0, 0);
  }
  if (digitalRead(RR) == LOW) {
    RR_t(1, 255);
  } else {
    RR_t(0, 0);
  }
  if (digitalRead(RL) == LOW) {
    RL_t(1, 255);
  } else {
    RL_t(0, 0);
  }
}

void button_trapezoid_v2() {
  Serial.print("max:");
  Serial.println(300);
  Serial.print("min:");
  Serial.println(0);
  if (digitalRead(FR) == LOW) {
    if (input_speed_FR < 130) {
      input_speed_FR += 10;
    } else if (input_speed_FR < 255) {
      input_speed_FR += 50;
      if (input_speed_FR >= 255) {
        input_speed_FR = 255;
      }
    }
    FR_t(1, input_speed_FR);
  } else {
    if (input_speed_FR > 200) {
      input_speed_FR -= 50;
      FR_t(1, input_speed_FR);
    } else if (input_speed_FR > 150) {
      input_speed_FR -= 40;
      FR_t(1, input_speed_FR);
    } else if (input_speed_FR > 100) {
      input_speed_FR -= 10;
      if (input_speed_FR < 100) {
        input_speed_FR == 100;
      }
      FR_t(1, input_speed_FR);
    } else {
      FR_t(0, input_speed_FR);
    }
  }

  if (digitalRead(FL) == LOW) {
    if (input_speed_FL < 130) {
      input_speed_FL += 10;
    } else if (input_speed_FL < 255) {
      input_speed_FL += 50;
      if (input_speed_FL >= 255) {
        input_speed_FL = 255;
      }
    }
    FL_t(1, input_speed_FL);
  } else {
    if (input_speed_FL > 200) {
      input_speed_FL -= 50;
      FL_t(1, input_speed_FL);
    } else if (input_speed_FL > 150) {
      input_speed_FL -= 40;
      FL_t(1, input_speed_FL);
    } else if (input_speed_FL > 100) {
      input_speed_FL -= 10;
      if (input_speed_FL < 100) {
        input_speed_FL == 100;
      }
      FL_t(1, input_speed_FL);
    } else {
      FL_t(0, input_speed_FL);
    }
  }

  if (digitalRead(RR) == LOW) {
    if (input_speed_RR < 130) {
      input_speed_RR += 10;
    } else if (input_speed_RR < 255) {
      input_speed_RR += 50;
      if (input_speed_RR >= 255) {
        input_speed_RR = 255;
      }
    }
    RR_t(1, input_speed_RR);
  } else {
    if (input_speed_RR > 200) {
      input_speed_RR -= 50;
      RR_t(1, input_speed_RR);
    } else if (input_speed_RR > 150) {
      input_speed_RR -= 40;
      RR_t(1, input_speed_RR);
    } else if (input_speed_RR > 100) {
      input_speed_RR -= 10;
      if (input_speed_RR < 100) {
        input_speed_RR == 100;
      }
      RR_t(1, input_speed_RR);
    } else {
      RR_t(0, input_speed_RR);
    }
  }

  if (digitalRead(RL) == LOW) {
    if (input_speed_RL < 130) {
      input_speed_RL += 10;
    } else if (input_speed_RL < 255) {
      input_speed_RL += 50;
      if (input_speed_RL >= 255) {
        input_speed_RL = 255;
      }
    }
    RL_t(1, input_speed_RL);
  } else {
    if (input_speed_RL > 200) {
      input_speed_RL -= 50;
      RL_t(1, input_speed_RL);
    } else if (input_speed_RL > 150) {
      input_speed_RL -= 40;
      RL_t(1, input_speed_RL);
    } else if (input_speed_RL > 100) {
      input_speed_RL -= 10;
      if (input_speed_RL < 100) {
        input_speed_RL == 100;
      }
      RL_t(1, input_speed_RL);
    } else {
      RL_t(0, input_speed_RL);
    }
  }
  delay(10);
}


void stop() {
  analogWrite(MotorPower_R, 0);
  analogWrite(MotorPower_L, 0);
  digitalWrite(MotorF_R, LOW);
  digitalWrite(MotorF_L, LOW);
  digitalWrite(MotorB_R, LOW);
  digitalWrite(MotorB_L, LOW);
}

void FR_t(int state, int input_speed) {
  Serial.print("FR:");
  Serial.println(input_speed);
  analogWrite(MotorPower_R, input_speed);
  if (state == 1) {
    digitalWrite(MotorF_R, HIGH);
  } else {
    digitalWrite(MotorF_R, LOW);
  }
}

void FL_t(int state, int input_speed) {
  Serial.print("FL:");
  Serial.println(input_speed);
  analogWrite(MotorPower_L, input_speed);
  if (state == 1) {
    digitalWrite(MotorF_L, HIGH);
  } else {
    digitalWrite(MotorF_L, LOW);
  }
}

void RR_t(int state, int input_speed) {
  Serial.print("RR:");
  Serial.println(input_speed);
  analogWrite(MotorPower_R, input_speed);
  if (state == 1) {
    digitalWrite(MotorB_R, HIGH);
  } else {
    digitalWrite(MotorB_R, LOW);
  }
}

void RL_t(int state, int input_speed) {
  Serial.print("RL:");
  Serial.println(input_speed);
  analogWrite(MotorPower_L, input_speed);
  if (state == 1) {
    digitalWrite(MotorB_L, HIGH);
  } else {
    digitalWrite(MotorB_L, LOW);
  }
}