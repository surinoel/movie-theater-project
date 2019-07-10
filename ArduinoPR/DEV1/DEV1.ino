int pinPos = 5, pinNeg = 4, pinEn = 3;
int pwmSpeed = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinPos, OUTPUT); pinMode(pinNeg, OUTPUT); pinMode(pinEn, OUTPUT);
  analogWrite(pinEn, pwmSpeed);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void serialEvent(void) 
{
  char data;
  data = Serial.read();
  if(data == 'o') {
    digitalWrite(12, 0);
  }
  else if(data == 'x') {
    digitalWrite(12, 1); 
  }
  Serial.write(data);
}
