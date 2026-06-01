#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_NeoPixel.h>

String serialLine = "";

int alpha_val = 0;
int beta_val = 0;
int delta_val = 0;
int gamma_val = 0;
int theta_val = 0;


int brightness = 10;
int saturation = 2;
float chroma = saturation * brightness;
float mOffset = brightness - chroma;
float r1, g1, b1;


int redColor = 0;
int greenColor = 0;
int blueColor = 0;

const int ServoMin = 240;
const int ServoMax = 660;
const int ServoMid = (ServoMax + ServoMin) / 2;

int currentAngArm = ServoMin;
int nextAngArm;
int currentAngHead = ServoMin;
int nextAngHead;
int angIncriment = 10;

const int numPixels = 64;  // number of neopixels in strip, in total i have 144, half is 72
const int neoPixelPin = 6;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// stuff for reverse curse technique
unsigned long previousServoMillis = 0;
unsigned long previousPixelMillis = 0;

int servoInterval = 50;
int pixelInterval = 150;

bool servoActive = false;
bool pixelActive = false;

int pixelIndex = 0;
bool pixelDirectionForward = true;

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);
  pixels.begin();
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    serialLine = Serial.readStringUntil('\n');

    int count = sscanf(
      serialLine.c_str(),
      "%d,%d,%d,%d,%d",
      &alpha_val,
      &beta_val,
      &delta_val,
      &gamma_val,
      &theta_val);

    // once full, run this code
    if (count == 5) {
      alpha_val = map(constrain((alpha_val / 10), 50, 2600), 50, 2600, 0, 255);   // max = 26963 -> 2696
      beta_val = map(constrain((beta_val / 10),   0, 1000), 0, 1000, 0, 255); // max = 9618  -> 962
      delta_val = map(constrain((delta_val / 10), 0, 800), 0, 800, 0, 255);   // max = 7478  -> 748
      gamma_val = map(constrain((gamma_val / 10), 0, 500), 0, 500, 0, 255);   // max = 3872  -> 387
      theta_val = map(constrain((theta_val / 10), 0, 900), 0, 900, 0, 255);   // max = 8080  -> 808
      // alpha - arms
      // beta - hue
      // delta - texture
      // gamma - pattern type decision
      // theta - color speed
      if(servoActive == false){
        prepareServos(alpha_val, delta_val);
      }
      if(pixelActive == false) {
        preparePixels(gamma_val, theta_val, beta_val);
      }
    }
  }
  moveServos();
  movePixels();
}

void prepareServos(int arm, int head){
  nextAngArm  = int(map(arm, 0, 255, ServoMin / 20, ServoMax / 20)) * 20;
  nextAngHead = int(map(head, 0, 255, ServoMin / 20, ServoMid / 20)) * 20;
  servoActive = true;

}

void preparePixels(int decisionVal, int speedVal, int hueVal){
  hsvToRgb(hueVal, chroma, saturation, brightness);
  pixelInterval = int(constrain((map(speedVal, 0, 255, 5, 15)), 5, 15) * 10);
  pixelDirectionForward = map(decisionVal, 0, 255, 0, 1);
  if(pixelDirectionForward > 0.5){
    pixelDirectionForward = true;
    pixelIndex = 0;
  }
  else{
    pixelDirectionForward = false;
    pixelIndex = numPixels + 4;
  }
  pixelActive = true;
}

void moveServos(){
  if (!servoActive) return; // this will only run if the servos are set to move

  if(millis() - previousServoMillis >= servoInterval){
    previousServoMillis = millis();

    if (currentAngArm < nextAngArm) {
      currentAngArm += angIncriment;
    } else if (currentAngArm > nextAngArm) {
      currentAngArm -= angIncriment;
    }
    if (currentAngHead < nextAngHead) {
      currentAngHead += angIncriment;
    } else if (currentAngHead > nextAngHead) {
      currentAngHead -= angIncriment;
    }
    
    currentAngArm = constrain(currentAngArm, ServoMin, ServoMax);
    currentAngHead = constrain(currentAngHead, ServoMin, ServoMid);

    for (int i = 0; i < 8; i++) {
      pwm.setPWM(i, 0, currentAngArm);
    }
    for (int i = 8; i < 12; i++) {
      pwm.setPWM(i, 0, currentAngHead);
    }
    // end when both motors are done
    if (currentAngArm == nextAngArm && currentAngHead == nextAngHead) {
      servoActive = false;
    }
  }
}

void movePixels(){
  if(!pixelActive) return;

  if(millis() - previousPixelMillis >= pixelInterval){

    previousPixelMillis = millis();

    pixels.setPixelColor(pixelIndex, pixels.Color(redColor, greenColor, blueColor));
    if(pixelDirectionForward){
       pixels.setPixelColor(pixelIndex - 4, (0));
       pixelIndex ++;
       if(pixelIndex > numPixels + 4){
        pixelActive = false;
       }
    }
    else{
      pixels.setPixelColor(pixelIndex + 4, (0));
       pixelIndex --;
       if(pixelIndex < -4){
        pixelActive = false;
       }
    }
    pixels.show();
  }
}

void hsvToRgb(float h, float c, float s, float b) {
  // chroma, saturation, brightness already defined
  // make sure its constrained holy heckler
  h = map(h, 0, 255, 0, 12);
  h = (int(constrain(h, 0, 12)) * 22.5);
  float xVal = c * (1 - abs(fmod((h / 60.0), 2) - 1));

  if (h >= 0 && h < 60) {
    r1 = c;
    g1 = xVal;
    b1 = 0;
  } else if (60 <= h && h < 120) {
    r1 = xVal;
    g1 = c;
    b1 = 0;
  } else if (120 <= h && h < 180) {
    r1 = 0;
    g1 = c;
    b1 = xVal;
  } else if (180 <= h && h < 240) {
    r1 = 0;
    g1 = xVal;
    b1 = c;
  } else if (240 <= h && h < 300) {
    r1 = xVal;
    g1 = 0;
    b1 = c;
  } else {
    r1 = c;
    g1 = 0;
    b1 = xVal;
  }

  redColor = constrain(((r1 + mOffset) * 255), 0, 255);
  greenColor = constrain(((g1 + mOffset) * 255), 0, 255);
  blueColor = constrain(((b1 + mOffset) * 255), 0, 255);
}