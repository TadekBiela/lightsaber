#ifndef LIGHTSABER_H
#define LIGHTSABER_H

#include "blade.h"
#include "monobutton.h"
#include "movedetector.h"
#include "pinsconfig.h"
#include "soundsplayer.h"

class LightSaber
{
public:
  LightSaber() :
    isActive(false),
    activationButton(ACTIVATION_BUTTON_PIN),
    blade(&soundsPlayer),
    moveDetector(&soundsPlayer)
  {}

  void init()
  {
    soundsPlayer.init();
    blade.init();
    moveDetector.init();
  }

  bool isActivated()
  {
    if(activationButton.clicked())
    {
      isActive = !isActive;
    }

    return isActive;
  }

  void activate()
  {
    blade.activate();
    activateExecutionLoop();
  }

  void deactivate()
  {
    blade.deactivate();
  }
  
private:
  bool isActive;
  MonoButton activationButton;
  SoundsPlayer soundsPlayer;
  Blade blade;
  MoveDetector moveDetector;

  void activateExecutionLoop()
  {
    while(isActivated())
    {
      blade.checkChanges();
      moveDetector.checkChanges();
      soundsPlayer.checkIfIdleLoopShouldBeStarted();
      delay(100);
    }
    
    deactivate();
  }
};

#endif // LIGHTSABER_H
