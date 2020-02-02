#include <FastCRC.h>
#include <Volume3.h>

#define speakerPin 9

FastCRC8 CRC8;

bool turboMode = 1; //Turbo mode
bool seconddigit = 1; //Still unknown function
bool fastAcceleration = 1; //1 for on, 0 for off
bool KPH = 1; //1 for KPH, 0 for MPH
bool fifthdigit = 0; //Still unknown function
bool Light = 1; //1 for on, 0 for off
bool LightBlink = 0; //1 for on, 0 for off
bool ESCOn = 1; //1 for on, 0 for off

int SpeedLimit = 255; //Beetwen 0 and 255

int fourth = 0;
byte buf[6] = {0xA6, 0x12, 0x02};


void setup() {
  // initialize Serial:
  Serial.begin(9600);
  
  calcFourth();
  buf[3] = fourth;
  buf[4] = SpeedLimit;
  buf[5] = CRC8.maxim(buf, 5);

  //define LED_BUILTIN as an output pin
  pinMode(LED_BUILTIN, OUTPUT); 

  Serial.write(buf, sizeof(buf));

  // startup sound
  R2D2();
}


void loop() {
  // flashes builtin LED
  if (digitalRead(LED_BUILTIN) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH); // LED ON 
  } else {
    digitalWrite(LED_BUILTIN, LOW);// LED OFF
  }

  Serial.write(buf, sizeof(buf));
  delay(500);
}


void calcFourth() {
  if (turboMode == 1){
    fourth = fourth + 128;
  }
  if (seconddigit == 1){
    fourth = fourth + 64;
  }
  if (fastAcceleration == 1){
    fourth = fourth + 32;
  }   
  if (KPH == 1){
    fourth = fourth + 16;
  }
  if (fifthdigit == 1){
    fourth = fourth + 8;
  }
  if (Light == 1){
    fourth = fourth + 4;
  }
  if (LightBlink == 1){
    fourth = fourth + 2;
  }
  if (ESCOn == 1){
    fourth++;
  }
}


void R2D2() {
  int beeps[] = {1933, 2156, 1863, 1505, 1816, 1933, 1729, 2291};
  int buzzVols[] = {144, 180, 216, 252, 252, 252, 252, 216, 180, 144};

  int i = 9;
  while (i >= 0) {
    vol.tone(speakerPin, 1050, buzzVols[i]*4);
    delayMicroseconds(20*64);
    vol.tone(speakerPin, 1050, buzzVols[i] / 8*4);
    delayMicroseconds(40*64);
    i--;
  }

  delay(35);

  i = 0;
  while (i < 8) {
    int v = 0;
    while (v < 250) { // 12.5 mS fade up time
      vol.tone(speakerPin, beeps[i], v*4);
      v += 10;
      delayMicroseconds(2*64);
    }
    delay(20);
    v = 250;
    while (v > 0) { // 12.5 mS fade down time
      vol.tone(speakerPin, beeps[i], v*4);
      v -= 10;
      delayMicroseconds(5*64);
    }
    vol.noTone();
    delay(25);
    i++;
  }

  int f = 2466;
  while (f < 2825) {
    vol.tone(speakerPin, f, 1023);
    f += 3;
    delay(1);
  }  
  f = 2825;
  int v = 255;
  while (f > 2000) {
    vol.tone(speakerPin, f, v*4);
    f -= 6;
    v -= 1;
    delay(1);
  }
  vol.noTone();
  delay(35);

  i = 10;
  while (i > 0) {
    vol.tone(speakerPin, 1050, buzzVols[i]*4);
    delayMicroseconds(20*64);
    vol.tone(speakerPin, 1050, buzzVols[i] / 8*4);
    delayMicroseconds(40*64);
    i--;
  }
  vol.noTone();
}
