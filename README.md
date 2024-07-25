# RFID_Smart_Container
This project is an RFID-based smart container using the NODEMCU ESP8266 microcontroller. The system is designed to track inventory and monitor the expiration dates of stored items, displaying information on an LCD screen and accessible via a web interface.

**Features**
1. RFID-based Inventory Tracking: Uses RFID tags to identify and track items in the container.
2. Expiration Date Monitoring: Keeps track of expiration dates and notifies users.
3. LCD Display: Displays item information and status.
4. Web Interface: Provides a web interface for remote monitoring and management.
5. Real-Time Clock (RTC): Maintains accurate time for expiration date calculations.

**Components Used**
1. ESP8266: Microcontroller with built-in Wi-Fi.
2. MFRC522: RFID reader module.
3. DS3231: Real-time clock module.
4. LiquidCrystal_I2C: I2C LCD display.
5. Buzzer: For audio notifications.
   
**Getting Started**
Hardware Setup

1.Connect the RFID Reader:

SDA to GPIO2
SCK to GPIO14
MOSI to GPIO13
MISO to GPIO12
IRQ to not connected
GND to GND
RST to GPIO0
3.3V to 3.3V

2. Connect the RTC Module:

SCL to GPIO5 (D1)
SDA to GPIO4 (D2)
VCC to 3.3V
GND to GND

3. Connect the LCD Display:

VCC to 3.3V
GND to GND
SCL to GPIO5 (D1)
SDA to GPIO4 (D2)

4. Connect the Buzzer:

+ to GPIO15
- to GND
  
**Software Setup**
1. Install Arduino IDE: Download and install the Arduino IDE from here.
   
2. Install ESP8266 Board Package:
Go to File > Preferences
In the "Additional Boards Manager URLs" field, add: http://arduino.esp8266.com/stable/package_esp8266com_index.json
Go to Tools > Board > Boards Manager
Search for esp8266 and install the latest version.

3. Install Required Libraries:
Go to Sketch > Include Library > Manage Libraries
Install the following libraries:
LiquidCrystal_I2C
ESP8266WebServer
MFRC522
RTClib
Wire
SPI

4. Upload the Code:

Open the smart_container.ino file in the Arduino IDE.
Configure your Wi-Fi credentials in the code:

const char *ssid = "Your_SSID";
const char *password = "Your_PASSWORD";

Select the appropriate board and port:
Tools > Board > NodeMCU 1.0 (ESP-12E Module)
Tools > Port > select the COM port for your ESP8266
Click the upload button.

**Usage**
1. LCD Display: The LCD will show the item name, stock status, and expiration date.
2. Web Interface: Access the web interface by connecting to the ESP8266's IP address. This will show detailed information about all tracked items.
3. Expiration Notification: The buzzer will sound when an item is nearing or past its expiration date.
