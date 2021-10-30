#include "button.h"
#include "brightness.h"
#include "blade.h"

const int button_pin = 16;
Button button(button_pin);

const int change_blade_color_pin = 5;
Button changeColorButton(change_blade_color_pin);

const int brightness_pin = 0;
Brightness brightness(brightness_pin, 6, 1024);

bool bladeActivated = false;

const int led_strip_data_pin = 4;
Blade blade(
  1
);

void setup() {
  delay(2000);
  pinMode(button_pin, INPUT);
  pinMode(change_blade_color_pin, INPUT);
  FastLED.addLeds<WS2811,led_strip_data_pin,GRB>(blade.leds(), blade.numberOfLeds()).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(0);
  Serial.begin(9600);
}

void loop()
{
  if(button.clicked())
  {
    Serial.print("bladeActivated: ");
    Serial.print(bladeActivated);
    Serial.print("\n");
    bladeActivated = !bladeActivated;
  }

  if(bladeActivated)
  {
    blade.setBrightness(brightness.level());
    blade.enable();
    if(changeColorButton.clicked())
    {
      blade.changeColor();
    }
  }
  else
  {
    blade.disable();
  }
  FastLED.show();
}
