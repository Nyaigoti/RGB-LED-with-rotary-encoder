#include <EEPROM.h> //Library for EEPROM usage
#include <Wire.h> 
#include <LiquidCrystal.h>

int CLK = 32; //Rotary encoder CLK pin
int DT =  33;   //DT pin
int SW =  25;   //SW pin

int RED_PIN = 13;      //LED red pin
int GREEN_PIN = 12;    //green pin
int BLUE_PIN = 26;     //blue

int led_onoff = 1;

static int oldCLK = LOW;   //Variable to store the value of the CLK pin
static int oldDT = LOW;  //Variable to store the value of the CLK pin

liquidCrystal_I2C lcd(0x27, 20, 4);  

int RED, GREEN, BLUE;
int INTERVAL, DURATION;

int mr, mg, mb, mi, md;

char buf[20];

int mode = 0; // 0=normal, 1=setup
int menu = 0; // 0=RED, 1=GREEN, 2=BLUE, 3=INTV, 4=DUR, 5=END


void setup() {
  initsetting();
  loadsetting();
  lcd.init();   //Initialize LCD
  lcd.backlight();  //Turn on backlight

  pinMode(CLK, INPUT_PULLUP)  //Set CLK as input with internal pullup resistor
  pinMode(DT, INPUT_PULLUP)   //Set DT as input with internal pullup resistor
  pinMode(SW, INPUT_PULLUP)   //Set SW as input with internal pullup resistor

  attachInterrupt(digitalPinToInterrupt(DT), rotdial, CHANGE);
}


void loop() {
  int currb = HIGH, lastb = HIGH;

  if ((millis() / 100) % (INTERVAL + DURATION) < DURATION)
   led_onoff = 1;
  else
   led_onoff = 0;

  currb = digitalRead(SW);

  if (mode == 1 && currb == LOW && lastb == HIGH){
    lcd.clear();
    menu = menu + 1;
    delay(50);
  }

  if(mode == 0 && currb == LOW && lastb == HIGH){
    lcd.clear(); mode = 1; menu = 0; mr = RED; mg = GREEN; mb = BLUE; mi = INTERVAL; md = DURATION;
    delay(50);    
  }

  lcdshow();
  ledlight();

  lastb = currb;

  delay(10); // this speeds up the simulation
}

void ledlight() {
  if (led_onoff == 1){
    analogWrite(RED_PIN, 255 - RED);
    analogWrite(GREEN_PIN, 255 - GREEN);
    analogWrite(BLUE_PIN, 255 - BLUE);
  } else {
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 255);
  }
}

void lcdshow(){
  if (mode == 0) {
    lcd.setCursor(0,0);
    sprintf(buf, "==RGB LED LIGHTING==");
    lcd.print(buf);
    ldc,setCursor(0,1);
    sprintf(buf, "R:%3d G:%3d B:%3d   ", RED, GREEN, BLUE);
    lcd.print((buf);
    lcd.setCursor(0,2);
    sprintf(buf, " INTERVAL:%2d.%d(sec) ", INTERVAL / 10, INTERVAL % 10);
    lcd.print(buf);      
    lcd.setCursor(0, 3);             
    sprintf(buf, " DURATION:%2d.%d(sec) ", DURATION / 10, DURATION % 10);
    lcd.print(buf); 
  }
  if (mode = 1){
    switch (menu){
      case 0:
      lcd.setCursor(0,0);
      sprintf(buf, "<<<CHANGE SETTING>>>");
        lcd.print(buf);      
        lcd.setCursor(0, 2);             
        sprintf(buf, "        %3d", mr);
        lcd.print(buf);
        lcd.setCursor(0, 3);             
        sprintf(buf, "   RED INTENSITY");
        lcd.print(buf);
        break;

      case 1:
        lcd.setCursor(0, 0);             
        sprintf(buf, "<<<CHANGE SETTING>>>");
        lcd.print(buf);      
        lcd.setCursor(0, 2);             
        sprintf(buf, "        %3d", mg);
        lcd.print(buf);
        lcd.setCursor(0, 3);             
        sprintf(buf, "  GREEN INTENSITY");
        lcd.print(buf);
        break;

      case 2:
        lcd.setCursor(0, 0);             
        sprintf(buf, "<<<CHANGE SETTING>>>");
        lcd.print(buf);      
        lcd.setCursor(0, 2);             
        sprintf(buf, "        %3d", mb);
        lcd.print(buf);
        lcd.setCursor(0, 3);             
        sprintf(buf, "   BLUE INTENSITY");
        lcd.print(buf);
        break;

      case 3:
        lcd.setCursor(0, 0);             
        sprintf(buf, "<<<CHANGE SETTING>>>");
        lcd.print(buf);      
        lcd.setCursor(0, 2);             
        sprintf(buf, "       %2d.%d", mi / 10, mi % 10);
        lcd.print(buf);
        lcd.setCursor(0, 3);             
        sprintf(buf, "   BLINK INTERVAL");
        lcd.print(buf);
        break;

      case 4:
        lcd.setCursor(0, 0);             
        sprintf(buf, "<<<CHANGE SETTING>>>");
        lcd.print(buf);  
        lcd.setCursor(0, 2);             
        sprintf(buf, "       %2d.%d", md / 10, md % 10);
        lcd.print(buf);
        lcd.setCursor(0, 3);             
        sprintf(buf, "   BLINK DURATION");
        lcd.print(buf);
        break;

      case 5:
        lcd.setCursor(0, 2);             
        sprintf(buf, "STORING SETTING .");
        lcd.print(buf);  
        delay(500);

        lcd.setCursor(0, 2);             
        sprintf(buf, "STORING SETTING ..");
        lcd.print(buf);
        delay(500);

        lcd.setCursor(0, 2);             
        sprintf(buf, "STORING SETTING ...");
        lcd.print(buf);
        delay(500);

        savesetting();
        mode = 0;
        break;        
    }
  }
}

void loadsetting(){
  RED = EEPROM.read(1);
  GREEN = EEPROM.read(2);
  BLUE = EEPROM.read(3);
  INTERVAL = EEPROM.read(4);
  DURATION = EEPROM.read(5);
}

void initsetting() {
  EEPROM.update(1, 255);
  EEPROM.update(2, 255);
  EEPROM.update(3, 255);
  EEPROM.update(4, 10);
  EEPROM.update(5, 10);
}

void savesetting(){
  RED = mr; EEPROM.update(1, RED);
  GREEN =mg; EEPROM.update(2, GREEN);
  BLUE = mb; EEPROM.update(3, BLUE);
  INTERVAL = mi; EEPROM.update(4, INTERVAL);
  DURATION = md; EEPROM.update(5, DURATION);
}

void rotdial() {
  if (mode == 0) return;

  int direct = 0;                 // Direction initialized to 0
  int newCLK = digitalRead(CLK);  // Variable to store the current CLK value
  int newDT = digitalRead(DT);    // Variable to store the current DT value
  
  if (newCLK != oldCLK) {         // CLK value has changed
    if (oldCLK == LOW) {          // Changed from LOW to HIGH
      direct = -1 * (oldDT * 2 - 1); // Change direct value using DT
    }
  }
  oldCLK = newCLK;  // Update oldCLK
  oldDT = newDT;    // Update oldDT

  switch (menu) {
      case 0: mr += 5 * direct; if (mr > 255) mr = 0; if (mr < 0) mr = 255; break;
      case 1: mg += 5 * direct; if (mg > 255) mg = 0; if (mg < 0) mg = 255; break;
      case 2: mb += 5 * direct; if (mb > 255) mb = 0; if (mb < 0) mb = 255; break;
      case 3: mi += direct; if (mi > 100) mi = 0; if (mi < 0) mi = 100; break;
      case 4: md += direct; if (md > 100) md = 0; if (md < 0) md = 100; break;
  }
} 