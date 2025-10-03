#define LED 34
#define SENSOR 35
float sensor;
bool sensor_detect;


void setup() {
  Serial.begin(9600);
  pinMode(SENSOR, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  sensor = digitalRead(SENSOR);
  if(sensor==0){
  Serial.println("Found the metal!");
  }
  else{
    Serial.println("Doesn't found the metal!");
  }
}
