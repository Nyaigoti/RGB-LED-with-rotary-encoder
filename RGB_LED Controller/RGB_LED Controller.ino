#include <EEPROM.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int CLK = 32;   //Rotary encoder CLK pin
int DT =  33;   //DT pin
int SW =  25;   //SW pin

int RED_PIN = 13;      //LED red pin
int GREEN_PIN = 12;    //green 
int BLUE_PIN = 26;     //blue

int led_onoff = 1;  // LED state (on or off)

static int oldCLK = LOW;   // Previous state of the CLK pin
static int oldDT = LOW;  // Previous state of the DT pin

LiquidCrystal_I2C lcd(0x27, 20, 4);

int RED, GREEN, BLUE;    // Variables for RGB values
int INTERVAL, DURATION;  // Variables for blink interval and duration

int mr, mg, mb, mi, md;  // Variables for modified settings

char buf[20];

int mode = 0; // 0=normal, 1=setup
int menu = 0; // 0=RED, 1=GREEN, 2=BLUE, 3=INTV, 4=DUR, 5=END


void setup() {
  initsetting();  // Initialize EEPROM with default settings
  loadsetting();
  lcd.init();   //Initialize LCD
  lcd.backlight();  //Turn on backlight

  pinMode(CLK, INPUT_PULLUP);  
  pinMode(DT, INPUT_PULLUP);   
  pinMode(SW, INPUT_PULLUP);   
  
  attachInterrupt(digitalPinToInterrupt(DT), rotdial, CHANGE);  // Attach interrupt to DT pin
}


void loop() {
  int currb = HIGH, lastb = HIGH;

  if ((millis() / 100) % (INTERVAL + DURATION) < DURATION)
   led_onoff = 1;
  else
   led_onoff = 0;

  currb = digitalRead(SW);

  //If in setup mode, pressing the button advances the menu
  if (mode == 1 && currb == LOW && lastb == HIGH){
    lcd.clear();
    menu = menu + 1;
    delay(50);
  }

  //If in normal mode, pressing it enters setup mode and copies the current settings to temporary variables.
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
  /*Controls the LED by adjusting its brightness based on the RGB values.
  When the LED is off, the colors are set to maximum */
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
  //Updates the LCD screen based on the current mode and menu selection
  if (mode == 0) {
    lcd.setCursor(0,0);
    sprintf(buf, "==RGB LED LIGHTING==");
    lcd.print(buf);
    lcd.setCursor(0,1);
    sprintf(buf, "R:%3d G:%3d B:%3d   ", RED, GREEN, BLUE);
    lcd.print(buf);
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
  //Loads the stored RGB and timing settings from EEPROM
  RED = EEPROM.read(1);
  GREEN = EEPROM.read(2);
  BLUE = EEPROM.read(3);
  INTERVAL = EEPROM.read(4);
  DURATION = EEPROM.read(5);
}

void initsetting() {
  //Initializes the EEPROM with default settings (maximum brightness, default timing)
  EEPROM.write(1, 255);
  EEPROM.commit();

  EEPROM.write(2, 255);
  EEPROM.commit();

  EEPROM.write(3, 255);
  EEPROM.commit();

  EEPROM.write(4,10);
  EEPROM.commit();

  EEPROM.write(5, 10);
  EEPROM.commit();
}

void savesetting(){
  //Saves the current settings to EEPROM
  RED = mr;
  EEPROM.write(1, RED);
  EEPROM.commit();

  GREEN =mg; 
  EEPROM.write(2, GREEN);
  EEPROM.commit();

  BLUE = mb; 
  EEPROM.write(3, BLUE);
  EEPROM.commit();

  INTERVAL = mi; 
  EEPROM.write(4, INTERVAL);
  EEPROM.commit();

  DURATION = md; 
  EEPROM.write(5, DURATION);
  EEPROM.commit();
}

void rotdial() {
  /*Handles changes in the rotary encoder's position to adjust the current setting. 
  The direction of rotation determines whether the values increase or decrease*/
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