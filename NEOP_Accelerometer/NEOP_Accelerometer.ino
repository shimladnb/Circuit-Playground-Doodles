// install board:
// https://learn.adafruit.com/adafruit-circuit-playground-express/set-up-arduino-ide



#include <Adafruit_CircuitPlayground.h>
// http://ermahgerd.info/Adafruit_CircuitPlayground/
#include <Smoothed.h>
#include <FastLED.h>
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#chsv

float X, Y, Z;
float sine;
float oldValX, oldValY, oldValZ;
float deltaX, deltaY, deltaZ;
static uint8_t currentHue;
int counter;
bool sittingState;
bool movingState;

// COLORS
CRGB eagleYellow(239, 167, 0);
CRGB eagleTeal(0, 75, 214);
CRGB iceIceBaby(193, 221, 217);
CRGB red(255, 37, 37);
CRGB green(0, 233, 46);
CRGB black(0, 0, 0);

// // CONTROL PARAMS
CRGB initialRGB = red;
CRGB endRGB = green;

int smoothAmt = 10;
float motionCurve = 2;

int ledBrightness = 255;
int ledBrightnessOffset = 255;

int initialHue = 150;
int endHue = 42;

bool motionChangesHue = true;
int motionOffsetHue = 5;

bool shouldTimeLoop = true;
float timelineSeconds = 1;
int waitTime = 0;




Smoothed<float> smoothyX, smoothyY, smoothyZ, smoothDeltaX, smoothDeltaY, smoothDeltaZ, smoothMotion;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  startUpSmootheners();
  CircuitPlayground.setBrightness(ledBrightness);
  setColorToPixel(1, CHSV(255, 255, 255));
  delay(10);
  setColorToPixel(1, CHSV(0, 0, 0));
  delay(waitTime * 1000);
}

void loop() {
  prepAccels(false);
  calculateDeltaVector();

  // set brigthness of leds to motion
  // float generalMotion = setBrightnessToMotion();

  // get a normalized timeline progression
  float normalizedTime = normalizedTimeline(timelineSeconds);


  if (deltaZ < 2.f) 
  {
    sittingState = true;
  }
  else
  {
    sittingState = false;
  }

  if (sittingState) 
  {
    SittingDownAnimation(normalizedTime);
  } 
  else 
  {
    for (int i = 0; i < 10; i++) 
    {
      setColorToPixel(i, red);
    }
  }


  Serial.println(deltaZ);
}
