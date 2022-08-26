#include <ESP8266WiFi.h>
#include <EEPROM.h>

void clign(int n) {
  for(int i=0; i<n; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n\n\nGo go go !");

  pinMode(LED_BUILTIN, OUTPUT);

  clign(8);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  //delay(500);
  ESP.deepSleep(5*1000*1000);
}
