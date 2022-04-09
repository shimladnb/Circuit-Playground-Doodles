#include <Adafruit_CircuitPlayground.h>
// http://ermahgerd.info/Adafruit_CircuitPlayground/
#include <Smoothed.h>
#include <FastLED.h>
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#chsv

float X, Y, Z;
int counter;
float sine;
int smoothAmt = 20;
int ledBrightness = 255;
int ledBrightnessOffset = 10;
float oldValX, oldValY, oldValZ;
float deltaX, deltaY, deltaZ;
int initialHue = 100;
int motionOffsetHue = 100;

Smoothed <float> smoothyX, smoothyY, smoothyZ, smoothDeltaX, smoothDeltaY, smoothDeltaZ, smoothMotion;

void setup()
{
  Serial.begin(9600);
  CircuitPlayground.begin();

  smoothyX.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothyY.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothyZ.begin(SMOOTHED_AVERAGE, smoothAmt);

  smoothDeltaX.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothDeltaY.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothDeltaZ.begin(SMOOTHED_AVERAGE, smoothAmt);

  smoothMotion.begin(SMOOTHED_AVERAGE, smoothAmt);

  CircuitPlayground.setBrightness(ledBrightness);
}

void loop()
{
  prepAccels(false);
  setBrightnessWithButton(false);
  calculateDeltaVector();

  float generalMotion = abs(deltaX + deltaY + deltaZ);
  smoothMotion.add(generalMotion);
  generalMotion = smoothMotion.get();
  generalMotion /= 32.0;
  generalMotion = pow(generalMotion, 2);
  Serial.println(generalMotion * 100);
  CircuitPlayground.setBrightness(constrain(ledBrightness * generalMotion + ledBrightnessOffset, 0, 255));

  for (int i = 0; i < 10; i++)
  {
    static uint8_t hue;
    hue = hue + 1;
    setColorToPixel(i, CHSV(initialHue + (generalMotion * motionOffsetHue), 255, 255));
  }

  delay(10);
}

void prepAccels(bool printShit)
{
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();
  X = map(X, -10, 10, 10, 255);
  Y = map(Y, -10, 10, 10, 255);
  Z = map(Z, -10, 10, 10, 255);
  smoothyX.add(X);
  smoothyY.add(Y);
  smoothyZ.add(Z);
  X = smoothyX.get();
  Y = smoothyY.get();
  Z = smoothyZ.get();

  if (printShit)
  {
    Serial.print("X: ");
    Serial.print(X);
    Serial.print("  Y: ");
    Serial.print(Y);
    Serial.print("  Z: ");
    Serial.println(Z);
  }
}

void pulseBrightness(float pulseSpeed)
{
  counter++;
  sine = sin(counter * pulseSpeed);
  sine = (sine * 0.5 + 0.5) * ledBrightness;
  //  Serial.println(sine);
  CircuitPlayground.setBrightness(sine);
}

int moduloLedBrightness(int currentBrightness)
{
  currentBrightness = (currentBrightness + 1) % 255;
  return currentBrightness;
}

void setBrightnessWithButton(bool printShit)
{
  if (CircuitPlayground.leftButton())
  {
    ledBrightness = moduloLedBrightness(ledBrightness);
    CircuitPlayground.setBrightness(ledBrightness);
  }

  if (printShit)
    Serial.println(ledBrightness);
}

void calculateDeltaVector()
{
  float currentValX = X;
  deltaX = currentValX - oldValX;
  oldValX = currentValX;
  smoothDeltaX.add(abs(deltaX));
  deltaX = smoothDeltaX.get();

  float currentValY = Y;
  deltaY = currentValY - oldValY;
  oldValY = currentValY;
  smoothDeltaY.add(abs(deltaY));
  deltaY = smoothDeltaY.get();

  float currentValZ = Z;
  deltaZ = currentValZ - oldValZ;
  oldValZ = currentValZ;
  smoothDeltaZ.add(abs(deltaZ));
  deltaZ = smoothDeltaZ.get();
}

void setColorToPixel(int pixel, const CRGB& rgb)
{
  CircuitPlayground.setPixelColor(pixel, rgb.r, rgb.b, rgb.g);
}
