#define BLYNK_TEMPLATE_ID "TMPLIf6LENKW"
#define BLYNK_DEVICE_NAME "iot"
#define BLYNK_AUTH_TOKEN "AooVSSffOUxEw02CSGPGX2CSYdYmrTOh"
#define BLYNK_PRINT Serial

//--------------------------------------
// Libraries
//--------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//--------------------------------------
// Objects
//--------------------------------------
BlynkTimer timer;

//--------------------------------------
// Variables
//--------------------------------------
int sensorData;
int output;
int onOFF;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "wifi_24g";
char pass[] = "M87aM86aJ16";
const int sensor = 34;     // GPIO 34
const int LED = 26;       // GPIO 26

//--------------------------------------
// Functions
//--------------------------------------
BLYNK_WRITE(V0) {                 // LED on off
  Serial.print("BLYNK WRITE");
  Serial.println(param.asInt());
  onOFF = param.asInt();                  // assigning server value from button to global variable (1 = on, 0 = off)
  digitalWrite(LED, param.asInt());       // Activates LED GPIO 26
}

void sendSensor() {
  sensorData = analogRead(sensor); //reading voltage from sensor´s GPIO 34

  if (isnan(sensorData)) {                  //Is Not A Number
    Serial.println("Failed to read from Hygrometer Soil Moisture sensor!");
    return;
  } else {
    Serial.print("Sensor data: ");
    Serial.println(sensorData);

    output = map(sensorData, 900, 2600, 100, 0);  //Map value : 900 will be 100% and 2600 will be 0%
    Serial.print("Output: ");
    Serial.println(output);
    Serial.println("Blynk start writting");
    Blynk.virtualWrite(V1, output);           // Upload calculatet percentage to server
    Serial.println("Blynk end of write");
  }
}

//--------------------------------------
// Core Functions
//--------------------------------------
void setup() {
  Serial.begin(115200);   // Debug console
  Serial.println("Start.");

  pinMode(LED, OUTPUT);       // assign GPIO for LED
  Blynk.begin(auth, ssid, pass);      // communication initialize

  timer.setInterval(3000L, sendSensor);   // timer setup for sensor function

  Serial.println("End.");
}

void loop() {
  Blynk.run();

  if(onOFF == 1) {
    if (output > 30) {
      Serial.print("Output value: ");
      Serial.println(output);
      digitalWrite(LED, LOW);
    } else {
      Serial.print("Output value: ");
      Serial.println(output);
      digitalWrite(LED, HIGH);
    }
  } else {
    Serial.println("Manually turned off.");
  }

  timer.run();
}

