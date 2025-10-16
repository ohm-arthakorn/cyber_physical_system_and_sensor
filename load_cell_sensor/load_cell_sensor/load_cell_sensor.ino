#include "HX711.h"

HX711 scale;

//  adjust pins if needed
uint8_t dataPin = 23;
uint8_t clockPin = 22;
float a = 103.51;
float c = -6191;


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("HX711_LIB_VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.println();

  scale.begin(dataPin, clockPin);

  //  TODO find a nice solution for this calibration..
  //load cell factor 20 KG
  scale.set_scale(-118.624);

  //  load cell factor 5 KG
  // scale.set_scale(107.605881);  //  TODO you need to calibrate this yourself.
  // scale.set_offset(-85735);
  //  reset the scale to zero = 0
  // scale.tare(20);
}


void loop() {

  // create condition for checking my load cell is ready ? show value below : my load cell'll broken down.
  if (scale.is_ready()) {
    float sum_value = 0;
    float weight_with_offset = scale.get_units(1);
    float weight_without_offset = weight_with_offset - 770;

    float weight_with_calibrated = (a*weight_with_offset)+c;


    Serial.print("Mean Weight : ");
    Serial.print(weight_without_offset);
    Serial.println(" g.");
    // delay 500 ms for checking value.
    delay(500);
  }
}




//  -- END OF FILE --
//  reset the scale to zero = 0
// scale.tare(20);
