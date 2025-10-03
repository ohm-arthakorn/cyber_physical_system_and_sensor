#include <math.h>

#define CURRENT_SENSOR_PIN 35
#define SAMPLE_SIZE 100
// coefficient for Linear Calibration!
const float alpha = 0.7687;
const float beta = 0.6905;

// coefficient for Polynomial Calibration! 
const float alpha_poly = -0.0611;
const float beta_poly = 1.1348;
const float gamma_poly = 0.3578;
float sum_sample;

void setup() {
  Serial.begin(9600);
  pinMode(CURRENT_SENSOR_PIN, INPUT);
}

void loop() {
  float ADC = analogRead(CURRENT_SENSOR_PIN);
  Serial.print("ADC : ");
  Serial.println(ADC);

  // change ADC to voltages compare with 3.3V 
  float voltage = (ADC/4095)*3.3;
  float set_zero_voltage = voltage - 2.16;
  float current_sensor = set_zero_voltage/0.185;
  float linear_calibration = (alpha*current_sensor)+beta;
  float polynomial_calibration = (alpha_poly*current_sensor*current_sensor)+(beta_poly*current_sensor)+gamma_poly;

  Serial.print("Voltage sensor : ");
  Serial.println(voltage);
  Serial.print("Current sensor : ");
  Serial.println(current_sensor);
  Serial.print("Calibrated Linear Current Sensor : ");
  Serial.println(linear_calibration);
  Serial.print("Calibrated Polynomial Current Sensor : ");
  Serial.println(polynomial_calibration);
  Serial.println("---------------------------------------------");
  delay(700);
}
