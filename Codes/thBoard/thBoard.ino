#include <Wire.h>

#define SensorAddr 0x40
#define motorPin1 10 //pin2 L293D
#define motorPin2 9 //pin7 L293D

void setMotorPinMode(){
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
}

void setup() {
  Wire.begin();
  setMotorPinMode();
  Serial.begin(9600);

}

void loop() {
  float humidity = getHumidityFromSensor();
  float celsiusTemperature = getTemperatureFromSensor();
  Serial.println("H"+String(humidity)+"T"+String(celsiusTemperature));

  while (Serial.available() < 1) {}
  
  int PWM = Serial.parseInt();
  analogWrite(motorPin1, PWM);
  analogWrite(motorPin2, 0);
  
  delay(4000); // 5 - 1s delay to getT,getH
}

float getHumidityFromSensor(){
  
  unsigned int data[2];  
  Wire.beginTransmission(SensorAddr);// Start I2C transmission
  Wire.write(0xF5); // Send humidity measurement command, NO HOLD master
  Wire.endTransmission(); // Stop I2C transmission
  delay(500);
   
  Wire.requestFrom(SensorAddr, 2);
   
  if(Wire.available() == 2)  {    
    data[0] = Wire.read();    
    data[1] = Wire.read();
   
    return  (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
  }
}

float getTemperatureFromSensor(){
    unsigned int data[2];  
    Wire.beginTransmission(SensorAddr);  
    Wire.write(0xF3);  // Send temperature measurement command, NO HOLD master  
    Wire.endTransmission();  // Stop I2C transmission  
    delay(500);
    Wire.requestFrom(SensorAddr, 2);// Request 2 bytes of data  

    if(Wire.available() == 2)  {// Read 2 bytes of data  temp msb, temp lsb  
      data[0] = Wire.read();    
      data[1] = Wire.read();
      float celsiusTemperature = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
      return celsiusTemperature;
  }
}
