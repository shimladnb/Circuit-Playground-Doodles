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

void startUpSmootheners()
{
  smoothyX.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothyY.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothyZ.begin(SMOOTHED_AVERAGE, smoothAmt);

  smoothDeltaX.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothDeltaY.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothDeltaZ.begin(SMOOTHED_AVERAGE, smoothAmt);

  smoothMotion.begin(SMOOTHED_AVERAGE, smoothAmt);
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

float lerp(float v0, float v1, float t)
{
  return (1 - t) * v0 + t * v1;
}

float flerp(float v0, float v1, float t) {
  return v0 + t * (v1 - v0);
}

const CRGB& flerpRgb(const CRGB& rgbA, const CRGB& rgbB, float alpha)
{
  // ayyyyyyy;
}

float normalizedTimeline(int timeThreshold)
{
//  Serial.print("normalized time: ");  
  timeThreshold *= 1000;
  int loopTime;
  if(shouldTimeLoop)
  {
    loopTime = (millis() - (waitTime * 1000)) % timeThreshold;
  }
  else
  {
    loopTime = constrain((millis() - (waitTime * 1000)), 0, timeThreshold);
  }    
  float loopTimeFloat = loopTime;
  float timeThresholdFloat = timeThreshold;
  float normalizedTime = loopTimeFloat / timeThresholdFloat;
  return normalizedTime;
//  Serial.println(normalizedTime);
}

float setBrightnessToMotion()
{
  float generalMotion = abs(deltaX + deltaY + deltaZ);
  smoothMotion.add(generalMotion);
  generalMotion = smoothMotion.get();
  generalMotion /= 32.0;
  generalMotion = pow(generalMotion, motionCurve);
  //  Serial.println(generalMotion * 100);
  CircuitPlayground.setBrightness(constrain(ledBrightness * generalMotion + ledBrightnessOffset, 0, 255));
  return generalMotion;
}
