#include "blade.h"
#include "lightsaber.h"
#include "pinsconfig.h"

LightSaber lightSaber;

void setup() {
  setupConfiguratedPins();
  lightSaber.init();
  delay(2000);
  Serial.begin(9600);
}

void loop()
{
  if(lightSaber.isActivated())
  {
    lightSaber.activate();
  }
}
