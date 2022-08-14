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

// COLORS
  CRGB eagleYellow( 239, 167, 0 );
  CRGB eagleTeal( 0, 75, 214);
  CRGB iceIceBaby( 193, 221, 217);

// CONTROL PARAMS
CRGB initialRGB = eagleYellow;
CRGB endRGB = eagleYellow;

int smoothAmt = 15;
float motionCurve = 2;

int ledBrightness = 255;
int ledBrightnessOffset = 255;

int initialHue = 255;
int endHue = 235;

bool motionChangesHue = false;
int motionOffsetHue = 5;

bool shouldTimeLoop = false;
float timelineSeconds = 10;
int waitTime = 0;

Smoothed <float> smoothyX, smoothyY, smoothyZ, smoothDeltaX, smoothDeltaY, smoothDeltaZ, smoothMotion;

void setup()
{
  Serial.begin(9600);
  CircuitPlayground.begin();
  startUpSmootheners();
  CircuitPlayground.setBrightness(ledBrightness);
  setColorToPixel(1, CHSV(255, 255, 255));
  delay(10);
  setColorToPixel(1, CHSV(0, 0, 0));
  delay(waitTime*1000);
}

void loop()
{
  prepAccels(false);
  calculateDeltaVector();
  float generalMotion = setBrightnessToMotion();
  
  float normalizedTime = normalizedTimeline(timelineSeconds);
  
  for (int i = 0; i < 10; i++)
  {
    float lerpRed = flerp(initialRGB.r, endRGB.r, normalizedTime);
    float lerpGreen = flerp(initialRGB.g, endRGB.g, normalizedTime);
    float lerpBlue = flerp(initialRGB.b, endRGB.b, normalizedTime);

    setColorToPixel(i, CRGB(lerpRed, lerpGreen, lerpBlue));


    //    currentHue = lerpedHue;
//    if (motionChangesHue)
//      currentHue = currentHue + (generalMotion * motionOffsetHue);      
//    currentHue %= 255;
//    Serial.println(currentHue);
//    setColorToPixel(i, CHSV(currentHue, 255, 255));
//    setColorToPixel(i, CRGB(lerpRed, lerpBlue, 0));
//    setColorToPixel(i, CRGB(0, 74, 213));

  }

  delay(10);
}
