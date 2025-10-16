// 1. Temperature Sensor --> Calibrated with Boiling Point and Freeze Point.
// 2. Power Meter
// 3. Load Cell

// Import Libraly for initialize Sensor. นำเข้า Libraly เพื่อเริ่มต้นการทำงานของ Sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#include "HX711.h"

#define ONE_WIRE_BUS 18
#define CURRENT_PIN 35
#define VOLTAGE_PIN 33
#define LOAD_CELL_CLOCKPIN 22
#define LOAD_CELL_DATA 23

const float a_current = -0.0494;
const float b_current = 1.0184;
const float c_current = 0.0622;

// Declare Variable for Current Sensor. ประกาศตัวแปรสำหรับการใช้งาน Current Sensor
const float OFFSET_TEMP = 0.36667;  // <-- ตัวเลขนี้มาจากการคำนวณ Excel ที่ทำการหา Slope และ Offset ที่เกิดขึ้นโดยการ plot ที่ค่าที่อ่านได้จาก Sensor และที่มีการ Calibrated แล้ว
const float OFFSET_CURRENT = 2.17;  // <-- ตัวเลขนี้เป็น OFFSET ของ ADC ของกระแสที่เกิดขึ้น
float raw_current = analogRead(CURRENT_PIN);
float raw_without_offset = raw_current - OFFSET_CURRENT;
float voltage_from_adc = ((raw_current / 4095) * 3.3);
float voltage_without_offset = voltage_from_adc - OFFSET_CURRENT;
float real_current = voltage_without_offset / 0.185;
float sum_value = 0;

// coefficient of linear calibraton สัมประสิทธิ์ของการปรับเทียบแบบเส้นตรง
float a_voltage = 0.6676;
float b_voltage = 0.5269;


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
HX711 scale;
HX711 offset;

// declare function that usage for Measure Temperature with "Full Waterproof Temperature Sensor!" [Calibrated by finding Offset.]
// ประกาศ Function สำหรับการใช้วัดอุณหภูมิโดยใช้งาน Full Waterproof Temperature Sensor [มีการ Calibrated ด้วยการหาค่า Offset เรียบร้อยแล้ว.]

void ShowTemperatureFromSensor() {
  // 11/10/2568 --> I have been experianced with Ice Cube in Yeti Bottle for finding a Offset! for Calibration Temperature Sensor without Calibrator.
  // 11/10/2568 --> ผมได้มีการทดลองโดยผมเอาน้ำแข็งใส่ลงในแก้วเยติเพื่อหาค่า Offset ของ Sensor ของผมว่ามีค่าเท่าไหร่ สำหรับการปรับเทียบ Sensor โดยที่ไม่มีตัวปรับเทียบ
  // Offset ของผมอยู่ที่ 0.366667 C
  sensors.requestTemperatures();
  float raw_temp_c = sensors.getTempCByIndex(0);
  float real_temp_c = raw_temp_c - OFFSET_TEMP;

  Serial.print("Temperature : ");
  Serial.println(real_temp_c);
}

void ShowDCPowerMeter() {
  float raw_current;
  float raw_without_offset;
  float voltage_from_adc;
  float voltage_without_offset;
  float real_current;
  float sum_value;

  // This section is finding MEAN. ส่วนนี้สำหรับการหาค่าเฉลี่ย
  for (int i = 0; i <= 1000; i++) {
    sum_value += real_current;  // <-- Loop for Summation Value. ลูปเพื่อเก็บค่าทั้งหมดตามจำนวนตัวอย่าง
  }
  float avg_current = sum_value / 1000;  // <-- Summation / Amount Sample. หาค่าเฉลี่ย = ค่าทั้งหมด / จำนวนตัวอย่าง.

  // <-- Polynomial Calibration การปรับเทียบแบบพหุนาม
  float calibrated_current = (a_current * avg_current * avg_current) + (b_current * avg_current) + c_current;
  Serial.print("Calibrated Current (I) : ");
  Serial.print(calibrated_current);
  Serial.println(" A");
  delay(500);

  // Read ADC from Voltage Sensor อ่านค่าสัญญาณแอนาล็อกจาก Voltage Sensor
  int adc_voltage = analogRead(VOLTAGE_PIN);
  //Serial.println(adc_voltage);

  // Transform into Voltage เปลี่ยนเป็นค่าแรงดันไฟฟ้า 
  float real_voltage = ((adc_voltage * 3.3) / (float)(1 << 9));
  //Serial.println(real_voltage);

  // Linear Calibration Section of Voltage Sensor ส่วนของการปรับเทียบ Voltage Sensor 
  float calibrated_voltage = (real_voltage * a_voltage) + b_voltage;
  Serial.print("Calibrated Voltage (V) : ");
  Serial.print(calibrated_voltage);
  Serial.println(" V");
  delay(500);

  float Power = calibrated_voltage * calibrated_current;
  Serial.print("Calibrated Power (W) : ");
  Serial.print(Power);
  Serial.println(" W");
  delay(500);
}

// Function that for show value from Load Cell Sensor! 
void ShowLoadCell() {
  if (scale.is_ready()) {
    float sum_value = 0;
    float weight_with_offset = scale.get_units(1);
    float weight_without_offset = weight_with_offset - 770;

    Serial.print("Weight : ");
    Serial.print(weight_without_offset);
    Serial.println(" g.");
    // delay 500 ms for checking value.
    delay(500);
  }
}

void setup() {
  Serial.begin(9600);
  sensors.begin();

  pinMode(CURRENT_PIN, INPUT);
  pinMode(VOLTAGE_PIN, INPUT);

  // initialized Load Cell. เริ่มต้นใช้งาน Load Cell.
  scale.begin(LOAD_CELL_DATA, LOAD_CELL_CLOCKPIN);
  scale.set_scale(-118.624);
}

void loop() {
  // ShowTemperatureFromSensor();
  // ShowDCPowerMeter();
  // ShowLoadCell();
}
