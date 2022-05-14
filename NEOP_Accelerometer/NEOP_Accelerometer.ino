#include <Adafruit_CircuitPlayground.h>
// http://ermahgerd.info/Adafruit_CircuitPlayground/
#include <Smoothed.h>
#include <FastLED.h>
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#chsv

//  Hoi ik ben bas

float X, Y, Z;
float sine;
float oldValX, oldValY, oldValZ;
float deltaX, deltaY, deltaZ;
static uint8_t currentHue;
int counter;

// CONTROL PARAMS
bool motionChangesHue = false;
int smoothAmt = 15;
float motionCurve = 2;
int ledBrightness = 255;
int ledBrightnessOffset = 10;
int initialHue = 0;
int endHue = 100;
int motionOffsetHue = 40;
bool shouldTimeLoop = false;
float timelineSeconds = 10;

CHSV color1( 160, 128, 255);
CHSV color2( 160, 128, 255);
CHSV color3( 160, 128, 255);
CHSV color4( 160, 128, 255);
CHSV color5( 160, 128, 255);

Smoothed <float> smoothyX, smoothyY, smoothyZ, smoothDeltaX, smoothDeltaY, smoothDeltaZ, smoothMotion;

void setup()
{
  Serial.begin(9600);
  CircuitPlayground.begin();
  startUpSmootheners();
  CircuitPlayground.setBrightness(ledBrightness);
}

void loop()
{
  prepAccels(false);
  calculateDeltaVector();
  float generalMotion = setBrightnessToMotion();
  
  float normalizedTime = normalizedTimeline(timelineSeconds);
  
  for (int i = 0; i < 10; i++)
  {
    float lerpedHue = flerp(initialHue, endHue, normalizedTime);
    currentHue = lerpedHue;
    if (motionChangesHue)
      currentHue = currentHue + (generalMotion * motionOffsetHue);      
    currentHue %= 255;
//    Serial.println(currentHue);
    setColorToPixel(i, CHSV(currentHue, 255, 255));
  }

  delay(10);
}
