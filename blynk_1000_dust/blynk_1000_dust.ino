#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <stdio.h>

#define BLYNK_PRINT Serial

char auth[] = "4df5697fb3c443a7bc8aeb65ff1c5e10";
char ssid[] = "ampere";
char pass[] = "espertap";
char server[] = "blynk.honey.co.th";  // use server honey lab can get power morethan 100000+

uint32_t pevTime = 0;
int sensorValue = 0;

WidgetLED led1(V2);


#define RXD2 18
#define TXD2 19

#define NUMBER_OF_BYTE 23
char line1[16], line2[16];
unsigned char buffer [NUMBER_OF_BYTE];
int PM25 = 0, PM10 = 0;
int PM1, PM1a, PM25a, PM10a;

bool checkValue(unsigned char *buf, int length)
{
  bool flag = 0;
  int sum = 0;

  for (int i = 0; i < (length - 2); i++)
  {
    sum += buf[i];
  }
  sum = sum + 0x42;

  if (sum == ((buf[length - 2] << 8) + buf[length - 1]))
  {
    sum = 0;
    flag = 1;
  }
  return flag;
}

void readPM3003() {
  char fel = 0x42;
  if (Serial1.find(&fel, 1)) {
    Serial1.readBytes(buffer, NUMBER_OF_BYTE);
  }

  if (buffer[0] == 0x4d)
  {
    if (checkValue(buffer, NUMBER_OF_BYTE))
    {
      PM25 = ((buffer[5] << 8) + buffer[6]);
      PM10 = ((buffer[7] << 8) + buffer[8]);

      // rest of values (if you want to use it)
      PM1 = ((buffer[3] << 8) + buffer[4]);
      PM1a = ((buffer[9] << 8) + buffer[10]);
      PM25a = ((buffer[11] << 8) + buffer[12]);
      PM10a = ((buffer[13] << 8) + buffer[14]);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Blynk.begin(auth, ssid, pass, server, 8080);
}

uint32_t ct = 0;

void loop() {
  if (millis() - pevTime >= 1000) {
    pevTime = millis();
    ct++;
    Serial.print("sender... ");
    Serial.println(ct);
    sensorValue = analogRead(A0);         // reading sensor from analog pin
    Blynk.virtualWrite(V0, sensorValue);  // sending sensor value to Blynk app
    Blynk.virtualWrite(V1, ct);

    readPM3003();

    Serial.print("pm1 atmosphere: "); Serial.println(PM1a);
    Serial.print("pm2.5 atmosphere: "); Serial.println(PM25a);
    Serial.print("pm10 atmosphere: "); Serial.println(PM10a);
    Serial.println();
    Serial.print("pm1: "); Serial.println(PM1);
    Serial.print("pm2.5: "); Serial.println(PM25);
    Serial.print("pm10: "); Serial.println(PM10);
    Serial.println();
    Serial.println();
  }

  if (digitalRead(2) == LOW) {
    led1.on();
  } else {
    led1.off();
  }

  Blynk.run();
}
