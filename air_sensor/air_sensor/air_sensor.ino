/* 
  MQ135 is a sensor that used for measure a harmful gas like benzene, CO2.
  MQ135 เป็นเซนเซอร์สำหรับการใช้วัดแก๊สที่เป็นอันตรายเช่น เบนซิน คาร์บอนไดออกไซต์

  MQ135 has a exponential calibration. because resistor that from sensor is exponential. 
  MQ135 มีลักษณะเฉพาะตัวคือ การ Calibrated แบบลักษณะ Exponential เพราะว่ามาจากการต้องปรับค่าตัวต้านทานบนตัว Sensor.
*/
#include <MQ135.h>
#define PIN_MQ135 33
#define LED_PIN 23  // This is a LED pin I have used. Pin ที่ 23 มีการใช้งาน LED
#define SAMPLE_SIZE 100
MQ135 mq135_sensor(PIN_MQ135);

// จะต้องมีการเก็บความชื้นของสภาพแวดล้อม ไม่ว่าจะเป็นอุณหภูมิและความชื้น
float temperature = 21.0;  // Assume current temperature. Recommended to measure with DHT22
float humidity = 25.0;     // Assume current humidity. Recommended to measure with DHT22
float rzero = mq135_sensor.getRZero();
float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
float resistance = mq135_sensor.getResistance();
float ppm = mq135_sensor.getPPM();
float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);



void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

// Created function that blink LED when Value of Harmful Gases exceed standard value.
void LightOutWhenCOexceed() {
  /*
  400 - 1000 ppm : normal
  1000 - 2000 ppm : medium
  morethan 5000 ppm : danger
  approximately 40000 ppm : died right there! 
  */

  // if ppm more than 5000 ppm that means, it's danger ==> Turn ON LED!
  if (ppm > 5000) {
    digitalWrite(LED_PIN, 1);
  }
  // if ppm less than 5000 ppm that means, it's medium or normal ==> blinking LED!
  else if (ppm < 5000) {
    digitalWrite(LED_PIN, 1);
    delay(500);
    digitalWrite(LED_PIN, 0);
    delay(500);
  }
}

void ShowValueMQ135() {
  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");
}

int AVGvalue(int sample_size, float ppm) {


  bool initialBufferCreated = 0;
  int samples[sample_size];
  int indexSample = 0;
  unsigned long sumSamples = 0;
  bool bufferFilled = 0;


  // initial Buffer amount 100 samples for calculate Mean
  if (initialBufferCreated == 0) {  // if Buffer doesn't created do below
    for (int i = 0; i < sample_size; i++) {
      samples[sample_size] = 0;
    }
    initialBufferCreated = 1;  // make state that show Buffer have been created.  สร้างสถานะที่บ่งบอกว่า Buffer จำนวน 100 Sample ได้ถูกสร้างแล้ว!
  }

  sumSamples -= samples[indexSample];

  samples[indexSample] = ppm;

  sumSamples += ppm;

  indexSample++;
  if (indexSample >= sample_size) {
    indexSample = 0;
    bufferFilled = 1;
  }

  int count = bufferFilled ? sample_size : indexSample;
  float average = (float)sumSamples / count;
  return average;
}

void loop() {
  ShowValueMQ135();
  LightOutWhenCOexceed();

  // float average = AVGvalue(SAMPLE_SIZE, ppm);

  // Serial.print("Average : ");
  // Serial.println(average);
  delay(100);
}
