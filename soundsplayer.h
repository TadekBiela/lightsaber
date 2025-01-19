#ifndef SOUNDSPLAYER_H
#define SOUNDSPLAYER_H

#include "pinsconfig.h"
#include <SoftwareSerial.h>

#define DFPLAYER_UART_FRAME_SIZE 0x08

class SoundsPlayer
{
public:
  enum TRACK
  {
    NO_TRACK = 0,
    ON1,
    ON2,
    ON3,
    OFF,
    SWING1,
    SWING2,
    SWING3,
    SWING4,
    SWING5,
    SWING6,
    SWING7,
    SWING8,
    STRIKE1,
    STRIKE2,
    STRIKE3,
    IDLE,
    NUM_OF_TRACKS = IDLE
  };

  SoundsPlayer() :
    softwareSerial(SOUNDS_PLAYER_RX_PIN, SOUNDS_PLAYER_TX_PIN)
  {
    memset(commandBuffer, 0x00, DFPLAYER_UART_FRAME_SIZE);
  }

  void init()
  {
    softwareSerial.begin(9600);
    delay(1000);
    setVolume(14);
  }

  void playTrack(SoundsPlayer::TRACK track)
  {
    if(canPlayNextTrack)
    {
      sendCommand(PLAY_TRACK, track);
      loopIdlePlayDelayCounter = 0;
      isLoopIdlePlay = false;
      canPlayNextTrack = false;
    }
  }

  void playRandomSwingTrack()
  {
    Serial.println("SWING1");
    playRandomTrackFromRangeWithIdleAtEnd(SoundsPlayer::SWING1, SoundsPlayer::SWING8);
  }

  void playRandomStrikeTrack()
  {
    playRandomTrackFromRangeWithIdleAtEnd(SoundsPlayer::STRIKE1, SoundsPlayer::STRIKE3);
  }

  void loopTrack(SoundsPlayer::TRACK track)
  {
    sendCommand(LOOP_TRACK, track);
    loopIdlePlayDelayCounter = 0;
    isLoopIdlePlay = true;
    canPlayNextTrack = true;
  }

  void setVolume(byte volume)
  {
    sendCommand(SET_VOLUME, volume);
  }

  void stop()
  {
    sendCommand(STOP, 0);
    loopIdlePlayDelayCounter = 0;
  }

  void checkIfIdleLoopShouldBeStarted()
  {
    if(isLoopIdlePlay)
    {
      return;
    }

    if(startPlayNextTrackLimit <= loopIdlePlayDelayCounter)
    {
      canPlayNextTrack = true;
    }
    if(loopIdlePlayStartLimit <= loopIdlePlayDelayCounter)
    {
      loopTrack(SoundsPlayer::IDLE);
    }
    else
    {
      loopIdlePlayDelayCounter++;
    }
  }

private:
  SoftwareSerial softwareSerial;
  uint8_t commandBuffer[DFPLAYER_UART_FRAME_SIZE];
  bool isLoopIdlePlay { false };
  int loopIdlePlayDelayCounter { 0 };
  const int loopIdlePlayStartLimit { 6000 };
  bool canPlayNextTrack { true };
  const int startPlayNextTrackLimit { 10 };

  enum COMMANDS
  {
    PLAY_TRACK = 0x03,
    LOOP_TRACK = 0x08,
    SET_VOLUME = 0x06,
    STOP = 0x0E
  };

  void sendCommand(byte command, byte argument)
  {
    commandBuffer[0] = (byte)0x7E;
    commandBuffer[1] = (byte)0xFF;
    commandBuffer[2] = (byte)0x06;
    commandBuffer[3] = command;
    commandBuffer[4] = (byte)0x00;
    commandBuffer[5] = (byte)0x00;
    commandBuffer[6] = argument;
    commandBuffer[7] = (byte)0xEF;

    softwareSerial.write(commandBuffer, DFPLAYER_UART_FRAME_SIZE);
  }

  void playRandomTrackFromRangeWithIdleAtEnd(SoundsPlayer::TRACK begin, SoundsPlayer::TRACK end)
  {
    int randomSwingTrack = random(begin, end + 1);
    playTrack(randomSwingTrack);
  }
};

#endif // SOUNDSPLAYER_H
