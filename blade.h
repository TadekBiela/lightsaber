#ifndef BALDE_H
#define BALDE_H

#include "monobutton.h"
#include "pinsconfig.h"
#include "potentiometer.h"
#include "soundsplayer.h"
#include <EEPROM.h>
#include <FastLED.h>
#include <ArduinoSTL.h>

FASTLED_USING_NAMESPACE
int eepromCurrentColorIdxAddr = 0;

class Blade
{
public:
  Blade(SoundsPlayer* soundsPlayer) :
    activated(false),
    colorChangeButton(CHANGE_COLOR_PIN),
    brightness(
      BRIGHTNESS_PIN,
      6,
      1024
    ),
    currentBrightness(0),
    enabldeAndDisableDelay(3),
    soundsPlayerPtr(soundsPlayer)
  {
    colors.push_back(CRGB::Red);
    colors.push_back(CRGB::Orange);
    colors.push_back(CRGB::LawnGreen);
    colors.push_back(CRGB::Green);
    colors.push_back(CRGB::Cyan);
    colors.push_back(CRGB::Blue);
    colors.push_back(CRGB::Purple);
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
      soundsPlayerPtr->playTrack(SoundsPlayer::ON);
      delay(550);
      enableLedsWithDelay();
      delay(100);
    }
  }
  void deactivate()
  {
    if(activated)
    {
      activated = false;
      soundsPlayerPtr->playTrack(SoundsPlayer::OFF);
      disableLedsWithDelay();
      delay(300);
      soundsPlayerPtr->stop();
    }
  }

  void checkChanges()
  {
    setBrightness();
    if(colorChangeButton.clicked())
    {
      changeColor();
      soundsPlayerPtr->playRandomSwingTrack();
    }
    FastLED.show();
  }
  
private:
  static const int numOfLeds = 120;
  CRGB leds[numOfLeds];
  bool activated;
  MonoButton colorChangeButton;
  size_t currentColorIdx;
  Potentiometer brightness;
  float currentBrightness;
  int enabldeAndDisableDelay;
  SoundsPlayer* soundsPlayerPtr;
  std::vector<CRGB> colors;

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
    CRGB currentColor(colors[currentColorIdx]);
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
    float newValue = brightness.level();
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
    if(colors.size() <= currentColorIdx)
    {
      currentColorIdx = 0;
    }
  }
  
  void setCurrentColor()
  {
    CRGB currentColor(colors[currentColorIdx]);
    for( int i = 0; i < numOfLeds; i++)
    {
        leds[i] = currentColor;
    }
  }

  void saveCurrentColor()
  {
    EEPROM.put(eepromCurrentColorIdxAddr, currentColorIdx);
  }

  void loadCurrentColor()
  {
    EEPROM.get(eepromCurrentColorIdxAddr, currentColorIdx);
  }
};

#endif // BLADE_H
