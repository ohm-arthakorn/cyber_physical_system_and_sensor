#define LED 34
#define SENSOR 35
int sensor;
bool sensor_detect;


void setup() {
  Serial.begin(9600);
  pinMode(SENSOR, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  sensor = digitalRead(SENSOR);
  if(sensor == 1){
    Serial.println("Found the metal !");
  }
  if(sensor == 0){
    Serial.println("Doesn't the metal !");
  }
  delay(500);
}

