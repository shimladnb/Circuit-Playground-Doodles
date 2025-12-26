void prepAccels(bool printShit) {
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

  if (printShit) {
    Serial.print("X: ");
    Serial.print(X);
    Serial.print("  Y: ");
    Serial.print(Y);
    Serial.print("  Z: ");
    Serial.println(Z);
  }
}

void startUpSmootheners() {
  smoothyX.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothyY.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothyZ.begin(SMOOTHED_AVERAGE, smoothAmt);

  smoothDeltaX.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothDeltaY.begin(SMOOTHED_AVERAGE, smoothAmt);
  smoothDeltaZ.begin(SMOOTHED_AVERAGE, smoothAmt);

  smoothMotion.begin(SMOOTHED_AVERAGE, smoothAmt);
}

void pulseBrightness(float pulseSpeed) {
  piTimer = ((millis() % 1000) / 1000.f) * 3.141;
  sine = cos(piTimer * pulseSpeed);
  sine = (sine * 0.5 + 0.5) * ledBrightness;
  //  Serial.println(sine);
  CircuitPlayground.setBrightness(constrain(sine, 0, 255));
}

int moduloLedBrightness(int currentBrightness) {
  currentBrightness = (currentBrightness + 1) % 255;
  return currentBrightness;
}

void setBrightnessWithButton(bool printShit) {
  if (CircuitPlayground.leftButton()) {
    ledBrightness = moduloLedBrightness(ledBrightness);
    CircuitPlayground.setBrightness(ledBrightness);
  }

  if (printShit)
    Serial.println(ledBrightness);
}

void calculateDeltaVector() {
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

void setColorToPixel(int pixel, const CRGB& rgb) {
  CircuitPlayground.setPixelColor(pixel, rgb.r, rgb.g, rgb.b);
}

void setColorToAllPixels(const CRGB& rgb) {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, rgb.r, rgb.g, rgb.b);
  }
}


float lerp(float v0, float v1, float t) {
  return (1 - t) * v0 + t * v1;
}

float flerp(float v0, float v1, float t) {
  return v0 + t * (v1 - v0);
}

const CRGB& flerpRgb(const CRGB& rgbA, const CRGB& rgbB, float alpha) {
  // ayyyyyyy;
}

float normalizedTimeline(int timeThreshold, int timerMillis) {
  //  Serial.print("normalized time: ");
  timeThreshold *= 1000;
  int loopTime;
  if (shouldTimeLoop) {
    loopTime = (timerMillis - (waitTime * 1000)) % timeThreshold;
  } else {
    loopTime = constrain((timerMillis - (waitTime * 1000)), 0, timeThreshold);
  }
  float loopTimeFloat = loopTime;
  float timeThresholdFloat = timeThreshold;
  float normalizedTime = loopTimeFloat / timeThresholdFloat;
  return normalizedTime;
  CircuitPlayground.setBrightness(ledBrightness);
  //  Serial.println(normalizedTime);
}

void setBrightnessToMotion() {
  float generalMotion = abs(deltaX + deltaY + deltaZ);
  smoothMotion.add(generalMotion);
  generalMotion = smoothMotion.get();
  generalMotion /= 32.0;
  generalMotion = pow(generalMotion, motionCurve);
  Serial.print("GeneralMotion: ");
  Serial.println(generalMotion);
  CircuitPlayground.setBrightness(constrain(((ledBrightness * generalMotion)) * 255, 0, 255));
}

void SittingDownAnimation(float normalizedTime) {
  if (normalizedTime < 0.95) {
    for (int i = 0; i < (normalizedTime * 10); i++) {
      setColorToPixel(i, sittingColor);
      // pulseBrightness(0.25);
    }
  } else {
    setColorToAllPixels(eagleYellow);
    // CircuitPlayground.setBrightness(255);
    // CircuitPlayground.playTone(523,1000,true); 
  }  // reset to black after chaser

  CircuitPlayground.setBrightness(ledBrightness);
  // pulseBrightness(1);
}

void MovingAnimation() {
  for (int i = 0; i < 10; i++) {
    setColorToPixel(i, movingColor);
  }
  setBrightnessToMotion();
}
