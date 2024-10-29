#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int var = 0;  // ตัวแปรสำหรับทดสอบแสดงผล
void setup() {
  Serial.begin(9600);
  if (!display.begin(i2c_Address, true)) {  // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("OLED Start Working !!!");
  }
}

void loop() {
  display.clearDisplay();              // ลบภาพในหน้าจอทั้งหมด

  display.setCursor(0, 0);
  display.setTextSize(1.5);
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  display.print("  CareWear  ");

  display.setCursor(0, 20); 
  display.setTextSize(2.5);
  display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  display.print(var, DEC);

  display.setCursor(30, 20); 
  display.print(" BPM");

  display.setCursor(0, 40);
  // display.println();
  display.setTextSize(2.5);
  display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  display.print(var, DEC); //send step from gyro
  display.setCursor(30, 40); 
  display.print(" Step(s)");


  display.display();  // สั่งให้จอแสดงผล
  var++;
  delay(500);
}