#include<LiquidCrystal_I2C_Hangul.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
LiquidCrystal_I2C_Hangul lcd (0x27, 16, 2);
int sensor = A2; // Assigning analog pin A1 to variable 'sensor'
float tempc;  //variable to store temperature in degree Celsius
float tempf;  //variable to store temperature in Fahreinheit
float vout;  //temporary variable to hold sensor reading
float vout1;
unsigned long timer = 0;
int refreshDelay = 3000;
int frame = 0;
int ir_proximity_pin = 3;
int backlit = 4;
void setup()
{
  pinMode(sensor, INPUT); // Configuring pin A1 as input
  pinMode(ir_proximity_pin,INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Booting");
  pox.begin();
  lcd.begin(16, 2);
  lcd.init();                      // initialize the lcd

  timer = millis();
  if (!pox.begin()) {
    Serial.println("SPO2 SENSOR INIT FAILED");
    for (;;);
  } else {
    Serial.println("SPO2 SENSOR INIT SUCCESS");
  }
}
//bool refresh;
long int last_update_time;
long int last_update_time_2;
int hr =0 ;
int spo2_ =0;
int sample = 25;
void loop()
{    pox.update();
  if (!digitalRead(ir_proximity_pin)){
    lcd.backlight();
  run_();
  }
  else{
      lcd.noBacklight();
  lcd.clear();}
  
}

void run_(){
  pox.update();
  int temp_val = analogRead(sensor);
  temp_val = abs((temp_val * 500) / 1023);
  int hr_sum = 0;
  int  spo2_sum = 0;
  float spo2_read;
  float hr_read;
  for (int i =0 ; i < sample; i++){
  hr_read = pox.getHeartRate();
  Serial.println(hr_read);
  spo2_read = pox.getSpO2();
  hr_sum += hr_read;
  spo2_sum += spo2_read;
  }
  hr_read =  hr_sum/sample;
  spo2_read = spo2_sum/sample;
  
  if (hr_read < 110 && hr_read > 60) {
    hr = hr_read ;
  }
  if (spo2_read > 70 && spo2_read <= 100) {
    spo2_ = spo2_read;
  }

  if ( millis() - last_update_time >  1000) {
    screen_switch(hr, spo2_, temp_val);
    last_update_time = millis();

  }

  if ( millis() - last_update_time_2 >  3000) {

    last_update_time_2 = millis();
    spo2_ = 0;
    hr = 0;
  }

  frame_switch();

}
void frame_switch() {

  if (millis() - timer >= refreshDelay)
  {
    frame++;
    if (frame >= 2)
      frame = 0;
    timer = millis();
  }
}
void screen_switch(int heart_rate, int spo2_val, float temp_val) {
  switch (frame)
  {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp DegC: ");
      lcd.print(temp_val);
      //      delay(1000);
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Heart Rate: ");
      lcd.print(heart_rate);
      lcd.setCursor(0, 1);
      lcd.print("spo2:");
      lcd.print(spo2_val);
      //      delay(1000);
      break;
  }
}
