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
    soundsPlayerPtr(soundsPlayer),
    onSpeedInput(
      BLADE_ON_SPEED_PIN,
      6,
      1024
    )
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
      activated = true;
      const int onSpeedNumOfLevels { 3 };
      int onSpeedLevel = onSpeedNumOfLevels * onSpeedInput.level();
      if(onSpeedLevel == 0)
      {
        soundsPlayerPtr->playTrack(SoundsPlayer::ON2);
        delay(400);
        fastEnableLedsWithDelay();
      }
      else if(onSpeedLevel == 1)
      {
        soundsPlayerPtr->playTrack(SoundsPlayer::ON1);
        delay(400);
        enableLedsWithDelay(3);
      }
      else if(onSpeedLevel == 2)
      {
        soundsPlayerPtr->playTrack(SoundsPlayer::ON3);
        delay(400);
        enableLedsWithDelay(8);
      }
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
      delay(700);
      soundsPlayerPtr->stop();
    }
  }

  void checkChanges()
  {
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
  SoundsPlayer* soundsPlayerPtr;
  std::vector<CRGB> colors;
  Potentiometer onSpeedInput;

  void initLedStrip()
  {
    FastLED.addLeds<WS2811, LED_STRIP_DATA_PIN, GRB>(ledsTable(), numberOfLeds()).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(255);
  }

  int numberOfLeds() const
  {
    return numOfLeds;
  }

  CRGB* ledsTable()
  {
    return leds;
  }

  void fastEnableLedsWithDelay()
  {
    int middleOfLeds = numOfLeds / 2;
    CRGB currentColor(colors[currentColorIdx]);
    for( int i = 0, j = numOfLeds - 1; i < middleOfLeds; i+=4, j-=4)
    {
      leds[i] = currentColor;
      leds[i+1] = currentColor;
      leds[i+2] = currentColor;
      leds[i+3] = currentColor;
      leds[j] = currentColor;
      leds[j-1] = currentColor;
      leds[j-2] = currentColor;
      leds[j-3] = currentColor;
      FastLED.show();
    }
  }

  void enableLedsWithDelay(int enableDelay)
  {
    int middleOfLeds = numOfLeds / 2;
    CRGB currentColor(colors[currentColorIdx]);
    for( int i = 0, j = numOfLeds - 1; i < middleOfLeds; i++, j--)
    {
      leds[i] = currentColor;
      leds[j] = currentColor;
      delay(enableDelay);
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
      delay(3);
      FastLED.show();
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
