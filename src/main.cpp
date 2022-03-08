/**
 * @file player-url-i2s.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-player/player-url-i2s/README.md
 * 
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

// set this in AudioConfig.h or here after installing https://github.com/pschatzmann/arduino-libhelix.git
#define USE_HELIX 

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "credentials.h"



const char *urls[] = {
    "https://22653.live.streamtheworld.com/MNM_128.mp3"
};
const char *wifi = WiFiSSID;
const char *password = WiFiPasswd;

ICYStream urlStream(wifi, password);
AudioSourceURL source(urlStream, urls, "audio/mp3");
AnalogAudioStream i2s;
VolumeStream volume(i2s);
MP3DecoderHelix decoder;
AudioPlayer player(source, i2s, decoder);

// additional controls
const int volumePin = A0;
Debouncer nextButtonDebouncer(2000);
const int nextButtonPin = 13;

// Print Audio Metadata
void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}


// Arduino setup
void setup() {
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // setup output
  auto cfg = i2s.defaultConfig(TX_MODE);
  i2s.begin(cfg);

  // setup player
  player.setMetadataCallback(printMetaData);
  player.begin();
}

// Arduino loop
void loop() {
  player.copy();
}