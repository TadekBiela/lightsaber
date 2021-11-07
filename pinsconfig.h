#ifndef PINSCONFIG_H
#define PINSCONFIG_H

#define ACTIVATION_BUTTON_PIN (2)
#define CHANGE_COLOR_PIN (3)
#define BRIGHTNESS_PIN (A0)
#define LED_STRIP_DATA_PIN (4)
#define SOUNDS_PLAYER_RX_PIN (10)
#define SOUNDS_PLAYER_TX_PIN (11)

static void setupConfiguratedPins()
{
  pinMode(ACTIVATION_BUTTON_PIN, INPUT);
  pinMode(CHANGE_COLOR_PIN, INPUT);
}

#endif // PINSCONFIG_H
