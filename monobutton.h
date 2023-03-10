#ifndef MONO_BUTTON_H
#define MONO_BUTTON_H

class MonoButton
{
public:
  MonoButton(int inputPin) :
    buttonPin(inputPin),
    pressCounter(0),
    clickedFlag(false)
  {}
  
  void readInput()
  {
    if(digitalRead(buttonPin) == 1)
    {
      pressCounter++;
    }
    else
    {
      if(clickedCounterLimit <= pressCounter)
      {
        clickedFlag = true;
      }
      else
      {
        clickedFlag = false;
      }
      
      pressCounter = 0;
    }
  }

  bool clicked() const
  {
    return clickedFlag;
  }

private:
  int buttonPin;
  int pressCounter;
  bool clickedFlag;
  
  const int clickedCounterLimit = 2;
};

#endif // MONO_BUTTON_H
