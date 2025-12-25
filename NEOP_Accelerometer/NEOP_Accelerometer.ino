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
CRGB red(255, 0, 0);
CRGB green(0, 255, 0);
CRGB black(0, 0, 0);
CRGB resoGreen( 0, 238, 128);
CRGB hotMagenta(238, 0, 184);

// // CONTROL PARAMS
CRGB sittingColor = resoGreen;
CRGB movingColor = hotMagenta;

bool printSensors = false;

int smoothAmt = 10;
float motionCurve = 3;

int ledBrightness = 0;
int ledBrightnessOffset = 0;

int initialHue = 150;
int endHue = 42;

bool motionChangesHue = true;
int motionOffsetHue = 5;

bool shouldTimeLoop = true;
float timelineSeconds = 10;
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
  prepAccels(printSensors);
  calculateDeltaVector();

  // set brigthness of leds to motion
  // float generalMotion = setBrightnessToMotion();

  // get a normalized timeline progression
  float normalizedTime = normalizedTimeline(timelineSeconds);


  if (deltaZ < 1.f) 
  {
    sittingState = true;
    movingState = false;
  }

  else
  {
    sittingState = false;
    movingState = true;
  }
    

  if (sittingState) 
  {
    CircuitPlayground.clearPixels();
    SittingDownAnimation(normalizedTime);
  }
    
  
  if (movingState)
    MovingAnimation();
  
    
  


  //Serial.println(deltaZ);
  
}
