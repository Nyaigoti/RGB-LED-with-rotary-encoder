#include <EEPROM.h> // Library for EEPROM usage
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int CLK = 32; // Rotary encoder CLK pin
int DT = 33;  // DT pin
int SW = 25;  // SW pin

int RED_PIN = 13;   // LED red pin
int GREEN_PIN = 12; // green pin
int BLUE_PIN = 26;  // blue

int led_onoff = 1;

static int oldCLK = LOW; // Variable to store the value of the CLK pin
static int oldDT = LOW;  // Variable to store the value of the CLK pin

LiquidCrystal_I2C lcd(0x27, 20, 4); // Correct constructor for the I2C LCD

int RED, GREEN, BLUE;
int INTERVAL, DURATION;

int mr, mg, mb, mi, md;

char buf[20];

int mode = 0; // 0=normal, 1=setup
int menu = 0; // 0=RED, 1=GREEN, 2=BLUE, 3=INTV, 4=DUR, 5=END

void setup() {
  initsetting();
  loadsetting();
  lcd.begin(20, 4); // Initialize LCD with 20 columns and 4 rows
  lcd.backlight();  // Turn on backlight

  pinMode(CLK, INPUT_PULLUP); // Set CLK as input with internal pullup resistor
  pinMode(DT, INPUT_PULLUP);  // Set DT as input with internal pullup resistor
  pinMode(SW, INPUT_PULLUP);  // Set SW as input with internal pullup resistor

  attachInterrupt(digitalPinToInterrupt(DT), rotdial, CHANGE);
}

void loop() {
  int currb = HIGH, lastb = HIGH;

  if ((millis() / 100) % (INTERVAL + DURATION) < DURATION)
    led_onoff = 1;
  else
    led_onoff = 0;

  currb = digitalRead(SW);

  if (mode == 1 && currb == LOW && lastb == HIGH) {
    lcd.clear();
    menu = menu + 1;
    delay(50);
  }

  if (mode == 0 && currb == LOW && lastb == HIGH) {
    lcd.clear();
    mode = 1;
    menu = 0;
    mr = RED;
    mg = GREEN;
    mb = BLUE;
    mi = INTERVAL;
    md = DURATION;
    delay(50);
  }

  lcdshow();
  ledlight();

  lastb = currb;

  delay(10); // this speeds up the simulation
}

void ledlight() {
  if (led_onoff == 1) {
    analogWrite(RED_PIN, 255 - RED);
    analogWrite(GREEN_PIN, 255 - GREEN);
    analogWrite(BLUE_PIN, 255 - BLUE);
  } else {
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 255);
  }
}

void lcdshow() {
  if (mode == 0) {
    lcd.setCursor(0, 0);
    sprintf(buf, "==RGB LED LIGHTING==");
    lcd.print(buf);
    lcd.setCursor(0, 1);
    sprintf(buf, "R:%3d G:%3d B:%3d   ", RED, GREEN, BLUE);
    lcd.print(buf);
    lcd.setCursor(0, 2);
    sprintf(buf, " INTERVAL:%2d.%d(sec) ", INTERVAL / 10, INTERVAL % 10);
    lcd.print(buf);
    lcd.setCursor(0, 3);
    sprintf(buf, " DURATION:%2d.%d(sec) ", DURATION / 10, DURATION % 10);
    lcd.print(buf);
  }
  if (mode == 1) {
    switch (menu) {
    case 0:
      lcd.setCursor(0, 0);
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
      // Missing closing braces added here
      break;
    }
  }
}

// Dummy functions to prevent compilation errors
void initsetting() {
  // Initialize your settings here
}

void loadsetting() {
  // Load your settings here
}

// Dummy function for rotary dial interrupt
void rotdial() {
  // Handle rotary dial interrupts here
}
