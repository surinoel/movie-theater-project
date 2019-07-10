#include <LiquidCrystal.h>
                                          // A0,A1,A2
LiquidCrystal lcd(13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3);
String sCommand = "", sStart, sTmp;
char cTemp;
String Temp, Humi; 
int receiveFlag = 0;

byte user_value[8]={
    B00010,
    B00101,
    B00010,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(
  lcd.begin(16, 2);
  lcd.createChar(0, user_value);
  lcd.clear();
}

void loop() {
  int ret;
  if(receiveFlag) {
    sStart = "", sTmp = "";
    ret = sCommand.indexOf(',');
    if(ret != -1) {
      sTmp = sCommand.substring(0, ret);
      sCommand = sCommand.substring(ret+1, sCommand.length());
      if(sTmp == "TH") {
          ret = sCommand.indexOf(',');
          Temp = sCommand.substring(0, ret);                
          sCommand = sCommand.substring(ret+1, sCommand.length());
          Humi = sCommand.substring(0, sCommand.length());
          lcd.setCursor(0, 0);
          String buf = Humi + "% " + Temp;
          lcd.print(buf);  
          lcd.write(static_cast<byte>(0x00));
          lcd.print('C');
      }
      else if(sTmp == "PWM") {
        
      }
    }
    
    receiveFlag = 0; 
    sCommand = "";
  }

}

void serialEvent(void) 
{
  while(Serial.available())
  {
     cTemp = Serial.read();
     if(cTemp == '\n') {  // 개행까지 받는다
        receiveFlag = 1;
        return;
     }
     sCommand += cTemp;
  }
}
