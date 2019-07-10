void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(12, OUTPUT);
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
