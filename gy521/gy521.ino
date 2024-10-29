#include "Wire.h"

const int MPU_ADDR = 0x68;

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;


char tmp_str[7];

int steps = 0;

char* convert_int16_to_str(int16_t i) {
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 7*2, true);

  accelerometer_x = Wire.read()<<8 | Wire.read();
  accelerometer_y = Wire.read()<<8 | Wire.read();
  accelerometer_z = Wire.read()<<8 | Wire.read();

  gyro_x = Wire.read()<<8 | Wire.read();
  gyro_y = Wire.read()<<8 | Wire.read();
  gyro_z = Wire.read()<<8 | Wire.read();
  
  // print out data
  Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));

  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));

  int convert_gX = 0, convert_gY = 0, convert_gZ = 0;

  convert_gX = gyro_x;
  convert_gY = gyro_y;
  convert_gZ = gyro_z;

  Serial.print(" || convert_gX = "); Serial.print(convert_gX); Serial.print(" ||");
  Serial.print(" || convert_gY = "); Serial.print(convert_gY); Serial.print(" ||");
  Serial.print(" || convert_gZ = "); Serial.print(convert_gZ); Serial.print(" ||");
  

  if (convert_gX > 1000 || convert_gY > 1000 || convert_gZ > 1000) {
    steps++;
  }

  Serial.print(" | step(s) = "); Serial.print(steps);

  Serial.println();
  
  // delay
  delay(500);
}
