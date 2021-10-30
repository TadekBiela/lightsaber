class Button
{
public:
  Button(int inputPin) :
    buttonPin(inputPin),
    isWasPressed(false)
  {}
  bool clicked()
  {
    if(analogRead(buttonPin) && isWasPressed == false)
    {
      isWasPressed = true;
      return true;
    }
    else if(analogRead(buttonPin) == 0)
    {
      isWasPressed = false;
    }
    return false;
  }
  
private:
  int buttonPin;
  bool isWasPressed;
};
