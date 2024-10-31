// for web
#include <ESP8266WiFi.h>  
#include <PubSubClient.h>
const char* ssid = "Nokia Lumia 149+"; // ชื่อ Wi-Fi ของคุณ
const char* password = "123123123"; // รหัสผ่าน Wi-Fi ของคุณ
const char* mqttServer = "yourbroker.com"; // ที่อยู่ของ HiveMQ broker
WiFiClient espClient;
PubSubClient client(espClient);

// oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// pulseSensor
#include <PulseSensorPlayground.h>
const int PulseWire = 0;       // PulseSensor ANALOG PIN 0
const int LED = LED_BUILTIN;   // The on-board Arduino LED PIN 13.
int Threshold = 550;
PulseSensorPlayground pulseSensor;
int bpm_val = 0;  // ตัวแปรสำหรับทดสอบแสดงผล

// gy521
const int MPU_ADDR = 0x68;
int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;

char tmp_str[7];
int steps = 0;

char* convert_int16_to_str(int16_t i) {
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

//set up wifi
void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.printf("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (Wifi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

//call back : function executed 
void callback(String topic, byte message, unsigned int lenght) {
  String messageTemp;

  for(int i=0; i<lenght; i++){
    Serial.print((char)messgae[1]);
    messgaeTemp += (char)messge[i];
  }
  Seraial.println()
}
//^ /////////////////////////////////////////
void setup() {

//HEART
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
//
  Serial.begin(115200);
//
  setupWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback); // ตั้ง callback function



// pulse setup
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

//Connect to web
void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(" connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ArduinoClient")) { // ใช้ชื่อ client ของคุณ
      Serial.println("connected");
      client.subscribe("your/incoming/topic"); // เปลี่ยนเป็น topic ที่คุณต้องการรับข้อมูล
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

// Callback function สำหรับรับข้อมูล
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
}

void loop() {
  WiFiClient client = server.available();


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


0000000000000000000000000000000