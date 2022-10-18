#include "config.h"

void setup() {
  Serial.begin(9600);

  // Start config server
  configStart();
}

void loop() {
  if (configTick()) {
    Serial.println(getConfigStatus());
    if (getConfigStatus() == CONFIG_SUBMIT) {
      Serial.println(config.SSID);
      Serial.println(config.pass);
    }
  }
}
