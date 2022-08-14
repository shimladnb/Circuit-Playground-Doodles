#include <Adafruit_CircuitPlayground.h>
// http://ermahgerd.info/Adafruit_CircuitPlayground/
#include <Smoothed.h>
#include <FastLED.h>
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#chsv






void setup()
{
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(255);
}

void loop()
{

  
  for (int i = 0; i < 10; i++)
  {
      CircuitPlayground.setBrightness(255);
    setColorToPixel(i, CHSV(30, 255, 255));
  }

  delay(10);
}

void setColorToPixel(int pixel, const CRGB& rgb)
{
  CircuitPlayground.setPixelColor(pixel, rgb.r, rgb.b, rgb.g);
}
