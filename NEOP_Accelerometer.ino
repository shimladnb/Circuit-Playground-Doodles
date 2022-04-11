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

// CONTROL PARAMS
int smoothAmt = 15;
int ledBrightness = 255;
int ledBrightnessOffset = 10;
int initialHue = 0;
int endHue = 255;
int motionOffsetHue = 40;
float timelineSeconds = 4;


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
  //  setBrightnessWithButton(false);
  calculateDeltaVector();
  
  float normalizedTime = normalizedTimeline(timelineSeconds);
  float lerpedHue = flerp(initialHue, endHue, normalizedTime); 

  float generalMotion = abs(deltaX + deltaY + deltaZ);
  smoothMotion.add(generalMotion);
  generalMotion = smoothMotion.get();
  generalMotion /= 32.0;
  generalMotion = pow(generalMotion, 2);
  //  Serial.println(generalMotion * 100);
  CircuitPlayground.setBrightness(constrain(ledBrightness * generalMotion + ledBrightnessOffset, 0, 255));

  for (int i = 0; i < 10; i++)
  {    
    currentHue = lerpedHue;
    currentHue = currentHue + (generalMotion * motionOffsetHue);
    currentHue %= 255;
    Serial.println(currentHue);
    setColorToPixel(i, CHSV(currentHue, 255, 255));
  }

  delay(10);
}
