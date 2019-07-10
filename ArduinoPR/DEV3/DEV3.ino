#include <DHT.h>
#define DHTTYPE DHT11

int pinDht = 2;
char buf[30];

DHT dht(pinDht, DHTTYPE);

void setup() {
  Serial.begin(115200); 
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
  
  String msg1 = "TH,"; 
  msg1 += (int)fTemp; 
  msg1 += ",";
  msg1 += (int)fHumi;
  msg1 += "\n";
  msg1.toCharArray(buf, 30);
  Serial.write(buf);
}
