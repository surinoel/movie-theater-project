int exFlag = 0;
int pinPos = 4, pinNeg = 2, pinEn = 3;
int tmpSpeed, pwmSpeed = 100;
int prevKeyValue, curKeyValue;
const int sw_row[4] = {13, 12, 11, 10};
const int sw_col[4] = {9, 8, 7, 6};
const int sw_gnd = 5;
char buf[30];

int getPWMSpeed(const int);
void incsMtSpeed(void);
void descMtSpeed(void);
void serialWriteCall(int);

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    pinMode(sw_row[i], OUTPUT);
    pinMode(sw_col[i], INPUT);
  }
  pinMode(sw_gnd, OUTPUT);
  digitalWrite(sw_gnd, LOW);

  pinMode(pinPos, OUTPUT); pinMode(pinNeg, OUTPUT); pinMode(pinEn, OUTPUT);
  digitalWrite(pinPos, HIGH);
  digitalWrite(pinNeg, LOW);
  analogWrite(pinEn, pwmSpeed);

  memset(buf, 0, sizeof(buf));
}

void loop() {
  curKeyValue = -1;
  for (int i = 0; i < 4; i++) {
    digitalWrite(sw_row[i], HIGH);
    for (int j = 0; j < 4; j++) {
      if ((i == 2 && j == 3) || (i == 3 && j == 0) || (i == 3) && (j == 2)) continue;
      if (digitalRead(sw_col[j])) {
        delay(100);
        if(prevKeyValue != i*4 + j) {
          curKeyValue = i*4 + j;  
        }
        break;
      }
    }
    digitalWrite(sw_row[i], LOW);
  }
  if(curKeyValue != -1) {
    tmpSpeed = getPWMSpeed(curKeyValue);
    if(tmpSpeed != -1) {
      if(exFlag)
      { 
        if(pwmSpeed >= 0 && pwmSpeed <= 255) {
          analogWrite(pinEn, pwmSpeed);
          serialWriteCall(pwmSpeed);  
        }
        pwmSpeed = 0;
        exFlag = 0;
      }
      else {
        pwmSpeed = pwmSpeed * 10 + tmpSpeed;    
      }    
    }
    Serial.println(tmpSpeed);  
  }
  prevKeyValue = curKeyValue;
}

void incsMtSpeed() {
  pwmSpeed = (pwmSpeed + 10 > 255) ? 0 : pwmSpeed += 10;
  analogWrite(pinEn, pwmSpeed);
  serialWriteCall(pwmSpeed);
}

void descMtSpeed() {
  pwmSpeed = (pwmSpeed - 10 <= 0) ? 0 : pwmSpeed -= 10;
  analogWrite(pinEn, pwmSpeed);
  serialWriteCall(pwmSpeed);
}

int getPWMSpeed(const int keyValue) {
  switch(keyValue) {
    case 3: incsMtSpeed(); return -1;
    case 7: descMtSpeed(); return -1;
    case 0: return 7;
    case 1: return 8;
    case 2: return 9;
    case 4: return 4;
    case 5: return 5;
    case 6: return 6;
    case 8: return 1;
    case 9: return 2;
    case 10: return 3;
    case 13: return 0;
    case 15: exFlag = 1;
  }
}

void serialWriteCall(int pwmSpeed)
{
  String msg = "PWM,"; 
  msg += pwmSpeed; 
  msg += "\n";
  msg.toCharArray(buf, 30);
  Serial.write(buf);
  memset(buf, 0, sizeof(buf));
}
