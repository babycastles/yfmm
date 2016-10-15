/*
  Code for an arduino to play the snow talk.

  Based on the example for the Adafruit VS1053 Codec Breakout by Limor
  Fried/Ladyada for Adafruit Industries (original license and text not included
  because this is not a redistribution.

  Originally modified by poohlaga, rewritten by leee after rewiring the couple.
 */

// Use SPI, MP3, SD libs
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET   -1      // VS1053 reset pin (unused!)
#define SHIELD_CS       7      // VS1053 chip select pin (output)
#define SHIELD_DCS      6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS          4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ            3     // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

const int buttonA =    A0;
const int buttonB =    A1;

void setup() {
  Serial.begin(9600);  // have a serial for debugging
  Serial.println("serial initialized");
  pinMode(buttonA, INPUT);
  pinMode(buttonB, INPUT);
  Serial.println("buttons initialized");
  if (! musicPlayer.begin()) {
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));
  SD.begin(CARDCS);
  Serial.println("SD card initialized");
  musicPlayer.setVolume(2,2);
  Serial.println("volume set");
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
}

void loop() {
  if (digitalRead(buttonA)) {
    Serial.println("buttonA fired");
    musicPlayer.playFullFile("snow.mp3");
  }
}
