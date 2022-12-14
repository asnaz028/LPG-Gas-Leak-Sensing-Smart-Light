#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_AUTH_TOKEN "5bmNP7OzcG-mJx_EKwTAzN-3Lm33ROVL" //Enter your blynk auth token

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Nokia C2";//Enter your WIFI name
char pass[] = "12345678";//Enter your WIFI password

// define the GPIO connected with Sensors & LEDs
#define MQ2_SENSOR    A0 //A0
#define GREEN_LED     14 //D5
#define RED_LED       13 //D7
#define WIFI_LED      16 //D0

int MQ2_SENSOR_Value = 0;
bool isconnected = false;
 

#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2

BlynkTimer timer;

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer
  getSensorData();
  isconnected = Blynk.connected();
  if (isconnected == true) {
    digitalWrite(WIFI_LED, LOW);
    sendSensorData();
    //Serial.println("Blynk Connected");
  }
  else{
    digitalWrite(WIFI_LED, HIGH);
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData()
{
  MQ2_SENSOR_Value = map(analogRead(MQ2_SENSOR), 0, 1024, 0, 100);
  //  Serial.println(analogRead(MQ2_SENSOR));
  if (MQ2_SENSOR_Value > 3 ) {
    //    Serial.println(MQ2_SENSOR_Value);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
  else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
}

void sendSensorData()
{
  Blynk.virtualWrite(VPIN_BUTTON_1, MQ2_SENSOR_Value);
  if (MQ2_SENSOR_Value > 50 )
  {
    Blynk.logEvent("gas", "Gas Detected!");
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(MQ2_SENSOR, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  //  digitalWrite(GREEN_LED, LOW);
  //  digitalWrite(RED_LED, LOW);
  //  digitalWrite(WIFI_LED, HIGH);
  //
  WiFi.begin(ssid, pass);
    timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
    Blynk.config(auth);
  delay(1000);
}

void loop()
{
    Blynk.run();
    timer.run();
}
