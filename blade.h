#include <FastLED.h>
#include <vector>

FASTLED_USING_NAMESPACE
  
class Blade
{
public:
  Blade(
    float brightness
  ) :
    enabled(false),
    currentColorIdx(0),
    currentColor(Blade::Colors[currentColorIdx]),
    currentBrightness(brightness),
    enabldeAndDisableDelay(3)
  {    
    setCurrentColor();
  }

  int numberOfLeds() const
  {
    return numOfLeds;
  }

  CRGB* leds()
  {
    return ledsTable;
  }

  void changeColor()
  {
    nextColor();
    setCurrentColor();
  }

  void setBrightness(float value)
  {
    if(value != currentBrightness)
    {
      currentBrightness = value;
      FastLED.setBrightness(255 * value);
    }
  }

  void enable()
  {
    if(not enabled)
    {
      enabled = true;
      enableLedsWithDelay();
    }
  }
  void disable()
  {
    if(enabled)
    {
      enabled = false;
      disableLedsWithDelay();
    }
  }
  
private:
  static const int numOfLeds = 120;
  CRGB ledsTable[numOfLeds];
  bool enabled;
  size_t currentColorIdx;
  CRGB currentColor;
  float currentBrightness;
  int enabldeAndDisableDelay;
  static std::vector<CRGB> Colors;

  void nextColor()
  {
    currentColorIdx++;
    if(Blade::Colors.size() <= currentColorIdx)
    {
      currentColorIdx = 0;
    }
    currentColor = Blade::Colors[currentColorIdx];
  }
  
  void setCurrentColor()
  {
    for( int i = 0; i < numOfLeds; i++)
    {
        ledsTable[i] = currentColor;
    }
  }

  void enableLedsWithDelay()
  {
    int middleOfLeds = numOfLeds / 2;
    for( int i = 0, j = numOfLeds - 1; i < middleOfLeds; i++, j--)
    {
      ledsTable[i] = currentColor;
      ledsTable[j] = currentColor;
      delay(enabldeAndDisableDelay);
      FastLED.show();
    }
  }

  void disableLedsWithDelay()
  {
    int middleOfLeds = numOfLeds / 2;
    for( int i = middleOfLeds - 1, j = middleOfLeds; 0 <= i; i--, j++)
    {
      ledsTable[i] = CRGB::Black;
      ledsTable[j] = CRGB::Black;
      delay(enabldeAndDisableDelay);
      FastLED.show();
    }
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
