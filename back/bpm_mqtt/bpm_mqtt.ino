// TODO import library
//// ^ import library : oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

//// ^ import library : pulseSensor
    //$ เข้า analog pin 0
    //$ from PulseSensor Playground Getting_BPM_to_Monitor
    //$ ยังไม่ได้เอาโค้ดขึ้นจอ

#include <PulseSensorPlayground.h>




#include <WiFiS3.h>
#include <MQTTClient.h>

const char WIFI_SSID[] = "Nokia Lumia 149+";          // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "123123123";  // CHANGE TO YOUR WIFI PASSWORD

const char MQTT_BROKER_ADRRESS[] = "mqtt-dashboard.com";  // CHANGE TO MQTT BROKER'S ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "999";  // CHANGE IT AS YOU DESIRE
const char MQTT_USERNAME[] = "";                                            // CHANGE IT IF REQUIRED, empty if not required
const char MQTT_PASSWORD[] = "";                                            // CHANGE IT IF REQUIRED, empty if not required

// The MQTT topics that Arduino should publish/subscribe
const char PUBLISH_TOPIC[] = "testtopic/999157";    // CHANGE IT AS YOU DESIRE
const char SUBSCRIBE_TOPIC[] = "testtopic/999157";  // CHANGE IT AS YOU DESIRE

const int PUBLISH_INTERVAL = 60 * 1000;  // 60 seconds

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

unsigned long lastPublishTime = 0;







// TODO  Variables
//// ^ variables : pulseSensor
#define i2c_Address 0x3c

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//// ^ variables : pulseSensor
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;

PulseSensorPlayground pulseSensor;

int bpm_val = 0;  // ตัวแปรสำหรับทดสอบแสดงผล

//// ^ variables : gy521
const int MPU_ADDR = 0x68;

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;

char tmp_str[7];

int steps = 0;

char* convert_int16_to_str(int16_t i) {
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}


void pulseSetup() {
  Serial.begin(115200);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);
  pulseSensor.setThreshold(Threshold);

   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
   }
}

void ledSetup() {
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

//////////////////////////////////////////////////////////// TODO

void connectToMQTT() {
  // Connect to the MQTT broker
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);

  // Create a handler for incoming messages
  mqtt.onMessage(messageHandler);

  Serial.print("Arduino UNO R4 - Connecting to MQTT broker");

  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (!mqtt.connected()) {
    Serial.println("Arduino UNO R4 - MQTT broker Timeout!");
    return;
  }

  // Subscribe to a topic, the incoming messages are processed by messageHandler() function
  if (mqtt.subscribe(SUBSCRIBE_TOPIC))
    Serial.print("Arduino UNO R4 - Subscribed to the topic: ");
  else
    Serial.print("Arduino UNO R4 - Failed to subscribe to the topic: ");

  Serial.println(SUBSCRIBE_TOPIC);
  Serial.println("Arduino UNO R4 - MQTT broker Connected!");
}

void sendToMQTT() { //* ค่าเข้าตรงนี้
  // int val = millis();
  // //int val = analogRead(A0);
  // String val_str = String(val);

  // // Convert the string to a char array for MQTT publishing
  // char messageBuffer[10];
  // val_str.toCharArray(messageBuffer, 10);

  // // Publish the message to the MQTT topic
  // mqtt.publish(PUBLISH_TOPIC, messageBuffer); //* ส่งขึ้น mqtt ตรงนี้

  // // Print debug information to the Serial Monitor in one line
  // Serial.println("Arduino UNO R4 - sent to MQTT: topic: " + String(PUBLISH_TOPIC) + " | payload: " + String(messageBuffer));


  String jsonPayload = "{\"bpm\":" + String(bpm_val) + ",\"steps\":" + String(steps) + "}";
  mqtt.publish(PUBLISH_TOPIC, jsonPayload);
  Serial.println("Sent: " + jsonPayload);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("Arduino UNO R4 - received from MQTT: topic: " + topic + " | payload: " + payload);

  // You can process the incoming data , then control something
  /*
  process something
  */
}

//////////////////////////////////////////////////////////// TODO
//////////////////////////////////////////////////////////// TODO
//////////////////////////////////////////////////////////// TODO

void setup() {
  pulseSetup();
  ledSetup();

  Serial.begin(9600);

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Arduino UNO R4 - Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  connectToMQTT();
}

void loop() {
  mqtt.loop();

//// ^ loop : oled
  display.clearDisplay();              // ลบภาพในหน้าจอทั้งหมด

//// ^ loop : gy521
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

//// ^ loop : pulse
  pulseSensor.sawStartOfBeat();
  bpm_val = pulseSensor.getBeatsPerMinute();
  bpm_val = bpm_val / 2;

//// ^ loop : oled (continue)
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  display.print("  CareWear  ");

  display.setCursor(0, 20); 
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  // display.print(bpm_val, DEC);
  display.print(bpm_val);   //^ bmp value

  display.setCursor(30, 20); 
  display.print(" BPM");  //^ bmp text

  display.setCursor(0, 40);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);  //กำหนดข้อความสีดำ ฉากหลังสีขาว
  // display.print(bpm_val, DEC); //send step from gyro
  display.print(steps); //^ steps value

  display.setCursor(30, 40); 
  display.print(" Step(s)");  //^ steps text

  display.display();  // สั่งให้จอแสดงผล
  
  sendToMQTT();
  delay(500);

}





