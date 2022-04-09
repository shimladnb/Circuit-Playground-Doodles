#include <Adafruit_CircuitPlayground.h>
// http://ermahgerd.info/Adafruit_CircuitPlayground/
#include <Smoothed.h>

float X, Y, Z;
int counter;
float sine;
int smoothAmt = 20;
int ledBrightness = 25;

Smoothed <float> smoothyX;
Smoothed <float> smoothyY;
Smoothed <float> smoothyZ;

void setup()
{
  Serial.begin(9600);
  CircuitPlayground.begin();

  smoothyX.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothyY.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothyZ.begin(SMOOTHED_AVERAGE, smoothAmt);
  CircuitPlayground.setBrightness(ledBrightness);
}

void loop()
{
  prepAccels(false);
  pulseBrightness(0.1);
  setBrightnessWithButton();

  for (int i = 0; i < 10; i++)
  {
    CircuitPlayground.setPixelColor(i, X, 0, Z);
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

void setBrightnessWithButton()
{
  if (CircuitPlayground.leftButton())
  {
    ledBrightness = moduloLedBrightness(ledBrightness);
    CircuitPlayground.setBrightness(ledBrightness);
  }

  Serial.println(ledBrightness);
}

float calculateDelta(float inputFloat)
{

}


//  calculating delta
//   oldTime = currentTime;
//   currentTime = millis();
//   deltaTime = currentTime - oldTime;
//
