#include "blade.h"
#include "lightsaber.h"
#include "pinsconfig.h"

LightSaber lightSaber;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Setup started"));

  setupConfiguratedPins();
  lightSaber.init();

  Serial.println(F("Setup ready!"));
}

void loop()
{
  if(lightSaber.isActivated())
  {
    lightSaber.activate();
  }
}
