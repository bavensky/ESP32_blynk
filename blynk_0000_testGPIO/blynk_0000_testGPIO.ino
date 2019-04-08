#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

char auth[] = "4df5697fb3c443a7bc8aeb65ff1c5e10";
char ssid[] = "ampere";
char pass[] = "espertap";
char server[] = "blynk.honey.co.th";  // use server honey lab can get power morethan 100000+

uint32_t pevTime = 0;
int sensorValue = 0;

WidgetLED led1(V2);

void setup() {
  Serial.begin(115200);
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
  }

  if (digitalRead(2) == LOW) {
    led1.on();
  } else {
    led1.off();
  }

  Blynk.run();
}
