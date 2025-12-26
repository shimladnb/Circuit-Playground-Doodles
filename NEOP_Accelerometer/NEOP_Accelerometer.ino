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
bool printSensors = false;
int smoothAmt = 15;
float motionCurve = 3;
bool shouldTimeLoop = true;
float timelineSeconds = 11;
int waitTime = 0;
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
Timer timer;
bool didThing1, didThing2;
Smoothed<float> smoothyX, smoothyY, smoothyZ, smoothDeltaX, smoothDeltaY, smoothDeltaZ, smoothMotion;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  startUpSmootheners();
  setColorToPixel(1, CHSV(255, 255, 255));
  delay(10);
  setColorToPixel(1, CHSV(0, 0, 0));
  delay(waitTime * 1000);
  timer.start();
  didThing1 = false;
  didThing2 = false;
}

void loop() {
  prepAccels(printSensors);
  calculateDeltaVector();

  float normalizedTime = normalizedTimeline(timelineSeconds, timer.read());

  if (deltaZ < 1.f) {
    sittingState = true;
    movingState = false;
  }

  else {
    sittingState = false;
    movingState = true;
  }


  if (sittingState) {
    didThing2 = false;  // reset other thing
    if (!didThing1) {
      // ...DO THING ONE HERE ONCE
      // setColorToAllPixels(sittingColor);
      // delay(1000);
      timer.stop();
      timer.start();
      // CircuitPlayground.clearPixels();
      didThing1 = true;  // ONLY
    }
    CircuitPlayground.clearPixels();
    SittingDownAnimation(normalizedTime);
  }


  if (movingState) {
    didThing1 = false;  // reset other thing
    if (!didThing2) {
      // ...DO THING TWO HERE ONCE
      didThing2 = true;  // ONLY
    }
    MovingAnimation();
  }

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


  Serial.print("Timer: ");
  Serial.println(timer.read());
  Serial.print("DeltaZ: ");
  Serial.println(deltaZ);
}
