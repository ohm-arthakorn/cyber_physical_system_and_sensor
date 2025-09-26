#define R1 29700       // Ohm
#define R2 7440        // Ohm
#define R_total 37140  // Ohm
#define ADCpin 33

// coefficient of linear calibraton สัมประสิทธิ์ของการปรับเทียบแบบเส้นตรง
float alpha = 0.6676;
float beta = 0.5269;

// coefficient of polynomial calibration สัมประสิทธิ์ของการปรับเทียบแบบพหุนาม
float alpha_poly = -0.0065;
float beta_poly = 0.7577;
float gamma_poly = 0.3978;


void setup() {
  Serial.begin(9600);
  analogSetWidth(12);  // Define width of bit each
  analogSetAttenuation(ADC_11db);
  pinMode(ADCpin, INPUT);
}

void loop() {

  // ADC value from voltage sensor 
  int analog_voltage_value = analogRead(ADCpin);
  Serial.print("voltage sensor : [");
  Serial.print(analog_voltage_value);
  Serial.println("] Volts");
  delay(500);

  // Calculate a Voltage from ADC
  float vout = (analog_voltage_value * 3.3) / (float)(1 << 9);
  Serial.print("[sensor] : [");
  Serial.print(vout);
  Serial.println("] Volts");
  delay(500);


  // Linear Calibration voltage sensor [ax+b]
  float voutcali = (alpha * vout) + beta;
  Serial.print("[linear calibrate sensor] : [");
  Serial.print(voutcali);
  Serial.println("] Volts");
  delay(500);

  // Polynomial Calibration voltage sensor [ax^2+bx+c]
  float voutcalipoly = (alpha_poly*(pow(vout,2)))+(beta_poly*vout)+gamma_poly;
  Serial.print("[polynomial calibrate sensor] : [");
  Serial.print(voutcalipoly);
  Serial.println("] Volts");
  delay(500);


    





}
