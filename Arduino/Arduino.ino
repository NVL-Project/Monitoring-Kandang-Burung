#include "ArduinoJson.h"

//dht11
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//Ultrasonic
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;
//Servo
#include<Servo.h>
Servo myservo;
//waterlevel
#define pinSensor A0
int sensorValue = 0;
float tinggiAir = 0;
float sensorVoltage = 0;
int nilaiMax = 1023;
float panjangSensor = 100;
//============================================================
float temp;
float hum;
int levelAir;
int levelPakan;
//============================================================
float value1;
float value2;
int value3;
int value4;
//============================================================
float sensorType1 = temp;
float sensorType2 = hum;
int sensorType3 = levelAir;
int sensorType4 = levelPakan;
int ntf1, ntf2, ntf3;

unsigned long interval = 1000; // the time we need to wait
unsigned long previousMillis = 0; // millis() returns an unsigned long.

#define relay1 8
#define relay2 9
#define relay3 10
#define relay4 11

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myservo.attach(5);

  delay(1000);
  Serial.println("Program Berjalan");

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

}

void loop()
{
  //dht11
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  sensorValue = analogRead(pinSensor);
  levelAir = sensorValue * panjangSensor / nilaiMax;

  //levelPakan
  long duration, distanceCm, distanceIn;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCm = duration / 29.1 / 2;
  distanceIn = duration / 74 / 2;
  levelPakan = distanceCm;

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  unsigned long currentMillis = millis(); // grab current time
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {

    aktuator();
    notif();
    Serial.print("*");
    Serial.print(temp);
    Serial.print(",");
    Serial.print(hum);
    Serial.print(",");
    Serial.print(levelAir);
    Serial.print(",");
    Serial.print(levelPakan);
    Serial.print(",");
    Serial.print(ntf1);
    Serial.print(",");
    Serial.print(ntf2);
    Serial.print(",");
    Serial.print(ntf3);
    Serial.println("#");

    previousMillis = millis();
  }

  data["temp"] = temp;
  data["humi"] = hum;
  data["lA"] = levelAir;
  data["lP"] = levelPakan;
  data["ntf1"] = ntf1;
  data["ntf2"] = ntf2;
  data["ntf3"] = ntf3;

  data.printTo(Serial1);
  jsonBuffer.clear();
}

void aktuator() {
  if (temp < 28)
  {
    digitalWrite(relay1, HIGH);
  }
  else if (temp > 28)
  {
    digitalWrite(relay1, LOW);
  }
  if (temp > 33)
  {
    digitalWrite(relay2, HIGH);
  }
  else if (temp < 33)
  {
    digitalWrite(relay2, LOW);
  }

  if (hum < 60)
  {
    digitalWrite(relay3, HIGH);
  }
  else if (hum > 60)
  {
    digitalWrite(relay3, LOW);
  }

  if (levelAir > 10)
  {
    digitalWrite(relay4, LOW);
  }
  else if (levelAir < 10)
  {
    digitalWrite(relay4, HIGH);
  }

  if (levelPakan > 5)
  {
    myservo.write(0);
  }
  else if (levelPakan < 5)
  {
    myservo.write(90);
  }
}

void notif() {
  if (digitalRead(relay1) == HIGH)
  {
    ntf1 = 1;
  }
  else
  {
    ntf1 = 0;
  }

  if (digitalRead(relay2) == HIGH)
  {
    ntf2 = 1;
  }
  else
  {
    ntf2 = 0;
  }

  if (digitalRead(relay4) == HIGH)
  {
    ntf3 = 1;
  }
  else
  {
    ntf3 = 0;
  }
}
