//^ /////////////////////////// oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//^ /////////////////////////// pulseSensor
//* เข้า analog pin 0
//* from PulseSensor Playground Getting_BPM_to_Monitor
//* ยังไม่ได้เอาโค้ดขึ้นจอ

#include <PulseSensorPlayground.h>

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;

PulseSensorPlayground pulseSensor;

int bpm_val = 0;  // ตัวแปรสำหรับทดสอบแสดงผล

//^ /////////////////////////// gy521
const int MPU_ADDR = 0x68;

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;

char tmp_str[7];

int steps = 0;

char* convert_int16_to_str(int16_t i) {
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

//^ /////////////////////////////////////////
//^ /////////////////////////////////////////

void setup() {

// pulse setup
  Serial.begin(115200);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);
  pulseSensor.setThreshold(Threshold);

   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
   }

// led setup
  // Serial.begin(9600);
  if (!display.begin(i2c_Address, true)) {  // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("OLED Start Working !!!");
  }

  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
// led loop
  display.clearDisplay();              // ลบภาพในหน้าจอทั้งหมด

// gy521
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

  if (gyro_x > 1000 || gyro_y > 1000 || gyro_z > 1000) {
    steps++;
  }

// pulse loop
  pulseSensor.sawStartOfBeat();
  int bpm_val = pulseSensor.getBeatsPerMinute();
  bpm_val = bpm_val / 2;

// led loop (continue)
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  display.print("  CareWear  ");

  display.setCursor(0, 20); 
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  display.print(bpm_val, DEC);

  display.setCursor(30, 20); 
  display.print(" BPM");

  display.setCursor(0, 40);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  // display.print(bpm_val, DEC); //send step from gyro
  display.print(steps);


  display.setCursor(30, 40); 
  display.print(" Step(s)");


  delay(500);

  display.display();  // สั่งให้จอแสดงผล
}


