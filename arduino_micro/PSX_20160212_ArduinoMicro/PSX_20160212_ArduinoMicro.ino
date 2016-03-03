//GARAPIKO-PU
//20160214 @ MATHRAX
//MATHRAX

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN_HEADLED 5
Adafruit_NeoPixel headLED = Adafruit_NeoPixel(144, PIN_HEADLED, NEO_GRB + NEO_KHZ800);

#define PIN_EYELED 4
Adafruit_NeoPixel eyeLED = Adafruit_NeoPixel(144, PIN_EYELED, NEO_GRB + NEO_KHZ800);


#include <PS2X_lib.h> //for v1.6
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); // RX, TX

PS2X ps2x; // create PS2 Controller Class

#define PS2_DAT        10  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        9  //16
#define PS2_CLK        12  //17

int error = 0;
byte vibrate = 0;
int tapeLedLock = 0;
int headR, headG, headB;
int eyeR, eyeG, eyeB;
//==========================================================================================================
//Main
//==========================================================================================================

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  delay(1000);




  //CHANGES for v1.6 HERE!!! **************(VS-C3コントローラー)*************

  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0) {
    Serial.println("0 Found Controller, configured successful");
  }

  else if (error == 1)
    Serial.println("1 No controller found, check wiring, see readme.txt to enable debug. ");

  else if (error == 2)
    Serial.println("2 Controller found but not accepting commands. see readme.txt to enable debug. ");

  else if (error == 3)
    Serial.println("3 Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);
  Serial.println(" PS-C2 Controller Found OK! "); //PS-2Cコントローラー


  headLedOff();
  headLED.begin();
  headLED.show();

  eyeLedOff();
  eyeLED.begin();
  eyeLED.show();

  for (uint16_t i = 0; i < headLED.numPixels(); i++) {
    headLED.setPixelColor(i, headLED.Color(headR, headG, headB));
  }
  headLED.show();

  for (uint16_t i = 0; i < eyeLED.numPixels(); i++) {
    eyeLED.setPixelColor(i, eyeLED.Color(eyeR, eyeG, eyeB));
  }
  eyeLED.show();
}


void loop() {
  //DualShock Controller(PS-C3コントローラーの場合)

  ps2x.read_gamepad(false, vibrate); //ゲームパッド・ボタンの読み込み

  if (ps2x.Button(PSB_START)) {
    //スタートボタン
    Serial.println("Start");
  }
  if (ps2x.Button(PSB_SELECT)) {
    //セレクトボタン
    Serial.println("Select");
  }

  if (ps2x.Button(PSB_PAD_UP)) { //上ボタン
    headLedOff();
    eyeLedOff();

    Serial.println("U");
    mySerial.write(255);
    mySerial.write('U');
  }
  if (ps2x.Button(PSB_PAD_RIGHT)) { //右ボタン
    headLedOff();
    eyeLedOff();

    Serial.println("R");
    mySerial.write(255);
    mySerial.write('R');
  }
  if (ps2x.Button(PSB_PAD_LEFT)) { //左ボタン
    headLedOff();
    eyeLedOff();

    Serial.println("L");
    mySerial.write(255);
    mySerial.write('L');
  }
  if (ps2x.Button(PSB_PAD_DOWN)) { //下ボタン
    headLedOff();
    eyeLedOff();

    Serial.println("D");
    mySerial.write(255);
    mySerial.write('D');
  }

  if (ps2x.Button(PSB_GREEN)) { //三角ボタン
    headLedOff();
    eyeLedOff();

    Serial.println("G");
    mySerial.write(255);
    mySerial.write('G');
  }
  if (ps2x.Button(PSB_RED)) { //丸ボタン
    headLedOff();
    eyeLedOff();

    Serial.println("O");
    mySerial.write(255);
    mySerial.write('O');
  }
  if (ps2x.Button(PSB_PINK)) { //四角ボタン
    headLedOff();
    eyeLedOff();

    Serial.println("P");
    mySerial.write(255);
    mySerial.write('P');
  }
  if (ps2x.Button(PSB_BLUE)) { //エックスボタン
    headLedOff();
    eyeLedOff();


    Serial.println("X");
    mySerial.write(255);
    mySerial.write('X');
  }


  if (ps2x.Button(PSB_L1)) {

    Serial.println("L1");
    mySerial.write(255);
    mySerial.write('a');

    headLedOn_Red();
  }

  if (ps2x.Button(PSB_R1)) {
    Serial.println("R1");
    mySerial.write(255);
    mySerial.write('b');

    headLedOn();
  }

  if (ps2x.Button(PSB_L2)) {
    Serial.println("L2");
    mySerial.write(255);
    mySerial.write('c');

    eyeLedOn_Red();
  }
  if (ps2x.Button(PSB_R2)) {
    Serial.println("R2");
    mySerial.write(255);
    mySerial.write('d');

    eyeLedOn();
  }




  //STICK-L LEFT
  if (ps2x.Analog(PSS_LX) <= 10) {
    headLedOff();
    eyeLedOff();

    Serial.println("STK-L L");
    mySerial.write(255);
    mySerial.write('g');
  }
  //STICK-L RIGHT
  if (ps2x.Analog(PSS_LX) >= 200) {
    headLedOff();
    eyeLedOff();

    Serial.println("STK-L R");
    mySerial.write(255);
    mySerial.write('h');
  }
  //STICK-L UP
  if (ps2x.Analog(PSS_LY) <= 10) {
    headLedOff();
    eyeLedOff();

    Serial.println("STK-L U");
    mySerial.write(255);
    mySerial.write('i');
  }
  //STICK-L DOWN
  if (ps2x.Analog(PSS_LY) >= 200) {
    headLedOff();
    eyeLedOff();

    Serial.println("STK-L D");
    mySerial.write(255);
    mySerial.write('j');
  }

  //STICK-R LEFT
  if (ps2x.Analog(PSS_RX) <= 10) {
    headLedOff();
    eyeLedOff();

    Serial.println("STK-R L");
    mySerial.write(255);
    mySerial.write('k');
  }
  //STICK-R RIGHT
  if (ps2x.Analog(PSS_RX) >= 200) {
    headLedOff();
    eyeLedOff();

    Serial.println("STK-R R");
    mySerial.write(255);
    mySerial.write('l');
  }
  //STICK-R UP
  if (ps2x.Analog(PSS_RY) <= 10) {
    headLedOff();
    eyeLedOff();

    Serial.println("STK-R U");
    mySerial.write(255);
    mySerial.write('m');
  }
  //STICK-R DOWN
  if (ps2x.Analog(PSS_RY) >= 200) {
    headLedOff();
    eyeLedOff();

    Serial.println("STK-R D");
    mySerial.write(255);
    mySerial.write('n');
  }
  //  {
  //    Serial.print(ps2x.Analog(PSS_LX), DEC);  0~255 通常127
  //    Serial.print(",");
  //    Serial.print(ps2x.Analog(PSS_RY), DEC);
  //    Serial.print(",");
  //    Serial.println(ps2x.Analog(PSS_RX), DEC);
  //  }

  for (uint16_t i = 0; i < headLED.numPixels(); i++) {
    headLED.setPixelColor(i, headLED.Color(headR, headG, headB));
  }
  headLED.show();

  for (uint16_t i = 0; i < eyeLED.numPixels(); i++) {
    eyeLED.setPixelColor(i, eyeLED.Color(eyeR, eyeG, eyeB));
  }
  eyeLED.show();
  delay(20);
}


// LED
void headLedOn() {
  //X button
  Serial.println("X");
  mySerial.write(255);
  mySerial.write('X');

  headR = 250;
  headG = 250;
  headB = 50;
}
void headLedOn_Red() {
  //X button
  Serial.println("X");
  mySerial.write(255);
  mySerial.write('X');

  headR = 255;
  headG = 0;
  headB = 0;
}
void headLedOff() {
  headR = 0;
  headG = 0;
  headB = 0;
}

void eyeLedOn() {
  eyeR = 250;
  eyeG = 250;
  eyeB = 50;
}
void eyeLedOn_Red() {
  eyeR = 255;
  eyeG = 0;
  eyeB = 0;
}
void eyeLedOff() {
  eyeR = 0;
  eyeG = 0;
  eyeB = 0;
}

