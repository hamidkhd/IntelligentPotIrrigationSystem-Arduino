#include <LiquidCrystal.h>

#define MAX_SPEED 256

float humidity, temperature;
int wateringRate;
int PWM;

bool readSerial();
void showOnLCD();

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    PWM = 0;
    lcd.begin(20, 4);
    Serial.begin(9600);
}

void loop() {
    bool rData = readSerial();    
    showOnLCD(); 
    if (rData) {
      setWateringRate();
    }
}

bool readSerial(){
  char incomingByte; 
  bool rData = false;
  
  if(Serial.available() >= 4){// 2 byte H , 2byte ? check 
    rData = true;
    incomingByte = Serial.read();
    if (incomingByte == 'H') {
      humidity = Serial.parseFloat();
    }
    incomingByte = Serial.read();
    if (incomingByte == 'T') {
      temperature = Serial.parseFloat();
    }
  }
  return rData;
}


void setWateringRate() {
    if(humidity > 50){
        wateringRate = 0;
        PWM = 0;
    }
    else if(humidity < 20) {
        wateringRate = 20; //20cc 
        PWM = MAX_SPEED / 4;
    }
    else if (20 <= humidity && humidity <= 50) {
        if (temperature < 25){
           wateringRate = 0;
           PWM = 0;
        }
        else {
           wateringRate = 10; //10cc
           PWM = MAX_SPEED / 10;
        }
      
    }
    Serial.println(PWM);
}

void showOnLCD(){
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.print(int(temperature));
  
  lcd.setCursor(0, 1);                                                                                                                             
  lcd.print("Humidity: ");
  lcd.print(humidity);

  lcd.setCursor(0,2);
  lcd.print("Water: ");
  lcd.print(wateringRate);
  lcd.print("cc");
}
