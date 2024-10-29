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
  display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีขาว ฉากหลังสีดำ
  display.setCursor(0, 0);             // กำหนดตำแหน่ง x,y ที่จะแสดงผล
  display.setTextSize(1);              // กำหนดขนาดตัวอักษร
  display.println();
  display.print("WIDTH:");             // แสดงผลข้อความ
  display.print(SCREEN_WIDTH);
  display.print(" | ");
  display.print("HEIGHT:");
  display.println(SCREEN_HEIGHT);
  display.println();


  display.setTextSize(2);
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  display.println("Ampify");


  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.println();
  display.println("Academy");


  display.setTextColor(SH110X_WHITE);
  display.setCursor(110, 25);
  display.setTextSize(1);
  display.println(var, DEC);


  display.display();  // สั่งให้จอแสดงผล
  var++;
  delay(500);
}