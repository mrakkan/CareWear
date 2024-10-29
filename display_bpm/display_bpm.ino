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



///////////////////////////// pulseSensor
// เข้า analog pin 0
// from PulseSensor Playground Getting_BPM_to_Monitor
// ยังไม่ได้เอาโค้ดขึ้นจอ

#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the Pulse
// SensorPlayground object called "pulseSensor"


///////////////////////////////////////////

int var = 0;  // ตัวแปรสำหรับทดสอบแสดงผล
void setup() {

// pulse setup
  Serial.begin(115200);

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
   }

// led setup
  Serial.begin(9600);

  if (!display.begin(i2c_Address, true)) {  // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("OLED Start Working !!!");
  }
}

void loop() {
// led loop
    display.clearDisplay();              // ลบภาพในหน้าจอทั้งหมด

// pulse loop
  // if (pulseSensor.sawStartOfBeat()) {
  //   int var = pulseSensor.getBeatsPerMinute(); 
    // Serial.println("♥  A HeartBeat Happened ! ");
    // Serial.print("BPM: ");
    // Serial.println(myBPM);
    pulseSensor.sawStartOfBeat();
    int var = pulseSensor.getBeatsPerMinute();
    var = var / 2;

// led loop
    // display.clearDisplay();              // ลบภาพในหน้าจอทั้งหมด

    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SH110X_BLACK, SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
    display.print("  CareWear  ");

    display.setCursor(0, 20); 
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
    display.print(var, DEC);

    display.setCursor(30, 20); 
    display.print(" BPM");

    display.setCursor(0, 40);
    display.setTextSize(1.5);
    display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
    // display.print(var, DEC); //send step from gyro
    display.print(var);


    display.setCursor(30, 40); 
    display.print(" Step(s)");
  // }

    // delay(20);


  delay(500);

  display.display();  // สั่งให้จอแสดงผล
  // var++;
}


