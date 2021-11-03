#ifndef BALDE_H
#define BALDE_H

#include "brightness.h"
#include "button.h"
#include "pinsconfig.h"
#include <EEPROM.h>
#include <FastLED.h>
#include <vector>

FASTLED_USING_NAMESPACE
int eepromCurrentColorIdxAddr = 0;

class Blade
{
public:
  Blade() :
    activated(false),
    colorChangeButton(CHANGE_COLOR_PIN),
    brightnessInput(
      BRIGHTNESS_PIN,
      6,
      1024
    ),
    currentBrightness(0),
    enabldeAndDisableDelay(3)
  {
  }

  void init()
  {
    loadCurrentColor();
    initLedStrip();
  }

  void activate()
  {
    if(not activated)
    {
      setBrightness();
      activated = true;
      enableLedsWithDelay();
    }
  }
  void deactivate()
  {
    if(activated)
    {
      activated = false;
      disableLedsWithDelay();
    }
  }

  void checkChanges()
  {
    setBrightness();
    if(colorChangeButton.clicked())
    {
      changeColor();
    }
    FastLED.show();
  }
  
private:
  static const int numOfLeds = 120;
  CRGB leds[numOfLeds];
  bool activated;
  Button colorChangeButton;
  size_t currentColorIdx;
  Brightness brightnessInput;
  float currentBrightness;
  int enabldeAndDisableDelay;
  static std::vector<CRGB> Colors;

  void initLedStrip()
  {
    FastLED.addLeds<WS2811, LED_STRIP_DATA_PIN, GRB>(ledsTable(), numberOfLeds()).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(0);
  }

  int numberOfLeds() const
  {
    return numOfLeds;
  }

  CRGB* ledsTable()
  {
    return leds;
  }
  void enableLedsWithDelay()
  {
    int middleOfLeds = numOfLeds / 2;
    CRGB currentColor(Blade::Colors[currentColorIdx]);
    for( int i = 0, j = numOfLeds - 1; i < middleOfLeds; i++, j--)
    {
      leds[i] = currentColor;
      leds[j] = currentColor;
      delay(enabldeAndDisableDelay);
      FastLED.show();
    }
  }

  void disableLedsWithDelay()
  {
    int middleOfLeds = numOfLeds / 2;
    for( int i = middleOfLeds - 1, j = middleOfLeds; 0 <= i; i--, j++)
    {
      leds[i] = CRGB::Black;
      leds[j] = CRGB::Black;
      delay(enabldeAndDisableDelay);
      FastLED.show();
    }
  }

  void setBrightness()
  {
    float newValue = brightnessInput.level();
    if(currentBrightness != newValue)
    {
      currentBrightness = newValue;
      FastLED.setBrightness(255 * newValue);
    }
  }

  void changeColor()
  {
    nextColor();
    setCurrentColor();
    saveCurrentColor();
  }

  void nextColor()
  {
    currentColorIdx++;
    if(Blade::Colors.size() <= currentColorIdx)
    {
      currentColorIdx = 0;
    }
  }
  
  void setCurrentColor()
  {
    CRGB currentColor(Blade::Colors[currentColorIdx]);
    for( int i = 0; i < numOfLeds; i++)
    {
        leds[i] = currentColor;
    }
  }

  void saveCurrentColor()
  {
    EEPROM.begin(4);
    delay(20);
    EEPROM.put(eepromCurrentColorIdxAddr, currentColorIdx);
    EEPROM.end();
  }

  void loadCurrentColor()
  {
    EEPROM.begin(4);
    delay(20);
    EEPROM.get(eepromCurrentColorIdxAddr, currentColorIdx);
  }
};

std::vector<CRGB> Blade::Colors = {
  CRGB::Red,
  CRGB::Orange,
  CRGB::LawnGreen,
  CRGB::Green,
  CRGB::LightSeaGreen,
  CRGB::Cyan,
  CRGB::Blue,
  CRGB::Purple
};

#endif // BLADE_H
