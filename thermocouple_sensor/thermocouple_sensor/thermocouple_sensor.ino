#include "max6675.h"

#define SO 19   // Slave Out (Datapin) Pin สำหรับการส่งข้อมูลออก
#define CLK 14  // Clock Pin สำหรับการส่งสัญญาณนาฬิกา
#define CS 15   // Chip Select Pin สำหรับการเลือก Chip

MAX6675 thermoCouple(CLK, CS, SO);

void setup() {
  Serial.begin(9600);
}


void loop() {
  // declare variables that collect value from sensor ประกาศตัวแปรสำหรับการเก็บค่าที่วัดได้จาก sensor
  float Celsius = thermoCouple.readCelsius();
  float Fahrenheit = thermoCouple.readFahrenheit();
  float Kelvin = Celsius + 273.15;


  // Show Celsius temperature แสดงค่าอุณหภูมิในหน่วย องศา
  Serial.print("Temperature [Celsius] : ");
  Serial.println(Celsius);
  Serial.println("--------------------------------------------------");
  delay(100);

  // Show Fahrenheit temperature แสดงค่าอุณหภูมิในหน่วย องศาฟาเรนไฮต์
  Serial.print("Temperature [Fahrenheit] : ");
  Serial.println(Fahrenheit);
  Serial.println("--------------------------------------------------");
  delay(100);

  // Show Kelvin temperature แสดงค่าอุณหภูมิในหน่วย เคลวิน 
  Serial.print("Temperature [Kelvin] : ");
  Serial.println(Kelvin);
  Serial.println("--------------------------------------------------");
  delay(100);
  delay(700);
}

