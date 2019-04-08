#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "4df5697fb3c443a7bc8aeb65ff1c5e10";
char ssid[] = "ampere";
char pass[] = "espertap";
char server[] = "blynk.honey.co.th";  // use server honey lab can get power morethan 100000+

uint32_t pevTime = 0;
int sensorValue = 0;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, server, 8080);
}

void loop() {
  if (millis() - pevTime >= 1000) {
    pevTime = millis();
    if (digitalRead(2) == LOW) {
      Blynk.virtualWrite(V2, LOW);
    } else {
      Blynk.virtualWrite(V2, HIGH);
    }

    Serial.println("sedn V1");
    sensorValue = analogRead(A0);         // reading sensor from analog pin
    Blynk.virtualWrite(V1, sensorValue);  // sending sensor value to Blynk app
  }



  Blynk.run();
}
