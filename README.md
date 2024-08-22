
# RGB LED Controller with Rotary Encoder and LCD Display

This project allows you to control an RGB LED's color intensity and blinking pattern using a rotary encoder and an LCD display. The settings are stored in EEPROM, ensuring they persist even after the system is powered off.


## Features

* **Color Control:** Adjust the intensity of the red, green, and blue channels of the RGB LED.
* **Blinking Pattern Control:** Set the interval and duration for the LED's blinking pattern.
* **Interface:** Navigate through settings using a rotary encoder, with visual feedback provided by an LCD display.
* **Settings:** Save your custom settings to EEPROM so they are preserved across power cycles.
## Features

* **Color Control:** Adjust the intensity of the red, green, and blue channels of the RGB LED.
* **Blinking Pattern Control:** Set the interval and duration for the LED's blinking pattern.
* **Interface:** Navigate through settings using a rotary encoder, with visual feedback provided by an LCD display.
* **Settings:** Save your custom settings to EEPROM so they are preserved across power cycles.
## Components
* Microcontroller (e.g., ESP32, Arduino)
* RGB LED
* Rotary Encoder with push-button
* LCD Display with I2C interface (20x4 characters)
* Resistors (as required)
* Wires and Breadboard
## Pin Connections
* Rotary Encoder CLK =>	32
* Rotary Encoder DT =>	33
* Rotary Encoder SW =>	25
* Red LED =>	13
* Green LED =>	12
* Blue  =>	26
* LCD SDA =>	Connect to I2C SDA
* LCD SCL =>	Connect to I2C SCL
## How It Works
### Normal Mode
* **LED Control:** The LED's color and blinking pattern are controlled based on the settings stored in EEPROM.
* **Display:** The LCD shows the current RGB values, blink interval, and duration.

### Setup Mode
* **Entering Setup Mode:** Press the rotary encoder button to enter setup mode. The screen will display options to change the RED, GREEN, and BLUE values, as well as the blink interval and duration.
* **Adjusting Values:** Rotate the encoder to adjust the selected value. Press the button to move to the next setting.
* **Saving Settings:** After adjusting all values, the settings are saved to EEPROM automatically.
## EEPROM Storage
* **RED, GREEN, BLUE:** Intensity values for each color channel.
* **INTERVAL:** Time (in tenths of a second) between blinks.
* **DURATION:** Time (in tenths of a second) the LED stays on during a blink
## Code Overview
* **setup():** Initializes the system, sets up pin modes, and loads settings from EEPROM.
* **loop():** Handles the main program logic, including LED control, reading user input, and updating the display.
* **ledlight():** Controls the RGB LED based on the current settings.
* **lcdshow():** Updates the LCD display with the current settings or setup options.
* **rotdial():** Handles rotary encoder input for adjusting settings.
* **initsetting(), loadsetting(), savesetting():** Manage reading and writing settings to EEPROM.
## Usage Instructions
1. **Connect the Components:** Assemble the circuit according to the pin connections described above.
2. **Upload the Code:** Flash the code to your microcontroller.
3. **Power On:** Once powered, the LCD will display the current RGB values and blinking settings.
4. **Adjust Settings:** Press the rotary encoder button to enter setup mode and adjust the settings to your preference.
5. **Save and Use:** The system will automatically save your settings, and the LED will behave according to your custom configuration.
## Dependencies
* **Arduino IDE:** Used for compiling and uploading the code.
* **EEPROM.h:** For reading/writing persistent settings.
* **Wire.h:** For I2C communication with the LCD.
* **LiquidCrystal_I2C.h:** For controlling the I2C LCD.