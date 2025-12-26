// install board:
// https://learn.adafruit.com/adafruit-circuit-playground-express/set-up-arduino-ide

#include <Adafruit_CircuitPlayground.h>
// http://ermahgerd.info/Adafruit_CircuitPlayground/
#include <Smoothed.h>
#include <FastLED.h>
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#chsv
#include "Timer.h"
// https://github.com/sstaub/Timer

////////////  COLORS  ///////////////
CRGB eagleYellow(239, 167, 0);
CRGB eagleTeal(0, 75, 214);
CRGB iceIceBaby(193, 221, 217);
CRGB red(255, 0, 0);
CRGB green(0, 255, 0);
CRGB black(0, 0, 0);
CRGB resoGreen(0, 238, 128);
CRGB hotMagenta(238, 0, 184);

////////////  CONTROL PARAMS  ///////////////
CRGB sittingColor = resoGreen;
CRGB movingColor = hotMagenta;
CRGB rewardColor = eagleYellow;
bool printSensors = true;
int smoothAmt = 10;
float motionCurve = 4;
float movementThresh = 1.5;
bool shouldTimeLoop = true;
float timelineSeconds = 6;
int ledBrightness = 30;


////////////  SYSVALS  ///////////////
float X, Y, Z;
float sine;
float oldValX, oldValY, oldValZ;
float deltaX, deltaY, deltaZ;
static uint8_t currentHue;
int counter;
float piTimer;
bool sittingState;
bool movingState;
Timer sittingTimer;
Timer movingTimer;
bool didThing1, didThing2;
Smoothed<float> smoothyX, smoothyY, smoothyZ, smoothDeltaX, smoothDeltaY, smoothDeltaZ, smoothMotion;
float elapsedMovingTime;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  startUpSmootheners();
  setColorToPixel(1, CHSV(255, 255, 255));
  delay(10);
  setColorToPixel(1, CHSV(0, 0, 0));
  sittingTimer.start();
  didThing1 = false;
  didThing2 = false;
}

void loop() {
  prepAccels(printSensors);
  calculateDeltaVector();
  float normalizedTime = normalizedTimeline(timelineSeconds, sittingTimer.read());
  elapsedMovingTime = (movingTimer.read() / 1000.f) / 60.f;

  if (deltaZ < movementThresh) {
    sittingState = true;
    movingState = false;
  }
  else {
    sittingState = false;
    movingState = true;
  }


  ////////////  SITTING STATE  ///////////////
  if (sittingState) {
    didThing2 = false;  // reset other thing
    if (!didThing1) {
      // ...DO THING ONE HERE ONCE
      movingTimer.pause();
      sittingTimer.stop();
      sittingTimer.start();
      didThing1 = true;
    }
    CircuitPlayground.clearPixels();
    SittingDownAnimation(normalizedTime);
  }

  ////////////  MOVING STATE  ///////////////
  if (movingState) {
    didThing1 = false; 
    if (!didThing2) {
      // ...DO THING TWO HERE ONCE
      if (movingTimer.state() == STOPPED)
        movingTimer.start();
      if (movingTimer.state() == PAUSED)
        movingTimer.resume();
      didThing2 = true;
    }
    MovingAnimation();
  }

  ////////////  BUTTONS  ///////////////
  if (CircuitPlayground.rightButton()) {
    if (ledBrightness > 10) {
      ledBrightness = ledBrightness - 1;
    }
  }

  if (CircuitPlayground.leftButton()) {
    if (ledBrightness < 240) {
      ledBrightness = ledBrightness + 1;
    }
  }

  if (printSensors) {
    Serial.print("sittingTimer: ");
    Serial.println(sittingTimer.read());
    Serial.print("movingTimer: ");
    Serial.println(elapsedMovingTime);
    Serial.print("DeltaZ: ");
    Serial.println(deltaZ);
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
  }

  delay(10);
}
