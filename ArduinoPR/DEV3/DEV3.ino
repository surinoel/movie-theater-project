#include <DHT.h>
#define DHTTYPE DHT11

String sCommand = "";
int pinDht = 2;
char buf[30], cTemp;
int pinPos = 5, pinNeg = 4, pinEn = 3;
int pwmSpeed = 100;

DHT dht(pinDht, DHTTYPE);

void setup() {
  Serial.begin(115200); 
  pinMode(pinPos, OUTPUT); pinMode(pinNeg, OUTPUT); pinMode(pinEn, OUTPUT);
  analogWrite(pinEn, pwmSpeed);
  dht.begin();
}

void loop() {
  delay(300);
  float fTemp = dht.readTemperature();
  float fHumi = dht.readHumidity();
  
  if (isnan(fTemp) || isnan(fHumi)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  while(Serial.available())
  {
     cTemp = Serial.read();
     if(cTemp == '\n') {  // 개행까지 받는다
        sCommand += '\n'; 
        sCommand.toCharArray(buf, 30);
        Serial.write(buf);
        memset(buf, 0, sizeof(buf));
        sCommand = "";
        break;     
     }
     sCommand += cTemp;
  }

  String msg1 = "TH,"; 
  msg1 += (int)fTemp; 
  msg1 += ",";
  msg1 += (int)fHumi;
  msg1 += "\n";
  msg1.toCharArray(buf, 30);
  Serial.write(buf);
}
