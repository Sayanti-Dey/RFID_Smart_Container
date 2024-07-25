/ MAX30100 ESP8266 WebServer
#include "LiquidCrystal_I2C.h"
#include <ESP8266WebServer.h>
#include <MFRC522.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>

RTC_DS3231 rtc;

char t[32];

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
constexpr uint8_t RST_PIN = 0; // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 2;  // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

/Put your SSID & Password/
const char *ssid = "Venom's Hell";    // Enter SSID here
const char *password = "Jeorge1990!"; // Enter Password here

char *tagUIDS[] = {"43 51 7D 92", "43 18 4A A4"};

int noOfCards = 2;
int buzz=15;

DateTime item1expDate(2023, 3, 12, 0, 0, 0);    // change y,m,d
DateTime item2expDate(2024, 3, 14, 0, 0, 0);

String item1="Milk";
String item1exp;
String item1msg = "";
String item1stk = "In Stock";

String item2="Juice";
String item2exp;
String item2msg = "";
String item2stk = "In Stock";

ESP8266WebServer server(80);

void setup() {
  
  Serial.begin(115200);
  pinMode(buzz,OUTPUT);
  digitalWrite(buzz,LOW);
  Wire.begin();
  SPI.begin(); // Init SPI bus
  rtc.begin();
  rtc.adjust(DateTime(F(_DATE),F(TIME_)));
  mfrc522.PCD_Init(); // Init MFRC522
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  lcd.clear(); // Clears LCD
  delay(2000);
  lcd.clear(); // Clears LCD
  Serial.println("Connecting to ");
  Serial.println(ssid);

  // connect to your local wi-fi network
  WiFi.begin(ssid, password);

  // check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  lcd.setCursor(0, 0);
  lcd.print("Got IP: ");

  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  
  delay(5000);

  item1exp+= item1expDate.day();
  item1exp+="/";
  item1exp+= item1expDate.month();
  item1exp+="/";
  item1exp+= item1expDate.year();
  
  item2exp+= item2expDate.day();
  item2exp+="/";
  item2exp+= item2expDate.month();
  item2exp+="/";
  item2exp+= item2expDate.year();

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(buzz,HIGH);
  delay(200);
  digitalWrite(buzz,LOW);
  delay(200);
  digitalWrite(buzz,HIGH);
  delay(200);
  digitalWrite(buzz,LOW);
  delay(200);
  digitalWrite(buzz,HIGH);
  delay(200);
  digitalWrite(buzz,LOW);
  delay(200);
}
void loop() {
    DateTime now1 = rtc.now();
    DateTime now2 = DateTime(now1.year(),now1.month(),now1.day(),0,0,0);

    
  if (now2 > item1expDate && item1stk == "In Stock") {
    item1msg = "Expired";
  } else if (now2 <= item1expDate && item1stk == "In Stock") {
    item1msg = "Fresh";
  }
  if (now2 > item2expDate && item2stk == "In Stock") {
    item2msg = "Expired";
  } else if (now2 <= item2expDate && item2stk == "In Stock") {
    item2msg = "Fresh";
  }

  if(item1stk=="Empty")
  {
    item1msg="N/A";
    item1exp="N/A";
  }
  
  if(item2stk=="Empty")
  {
    item2msg="N/A";
    item2exp="N/A";
  }

  if(item1stk=="In Stock")
  {
    item1exp = item1expDate.day();
  item1exp+="/";
  item1exp+= item1expDate.month();
  item1exp+="/";
  item1exp+= item1expDate.year();
  }

  if(item2stk=="In Stock")
  {
    item2exp = item2expDate.day();
  item2exp+="/";
  item2exp+= item2expDate.month();
  item2exp+="/";
  item2exp+= item2expDate.year();
  }

  server.handleClient();
  delay(500); // Update at every 100mSeconds

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Reading from the card
  String tag = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tag.toUpperCase();
  digitalWrite(buzz,HIGH);
  delay(200);
  digitalWrite(buzz,LOW);
  delay(200);
  //Serial.println(item1msg);
  //Serial.println(item2msg);
  // Checking the card
  //Serial.println(tag);
    if (tag.substring(1) == tagUIDS[0]) // change here the UID of the card/cards
                                        // that you want to give access
    {
      if (item1stk == "In Stock") {
        item1stk = "Empty";
      } 
      else if (item1stk == "Empty") {
        item1stk = "In Stock";
      } 
    }
    else if (tag.substring(1) == tagUIDS[1]) // change here the UID of the card/cards
                                        // that you want to give access
    {
       if (item2stk == "In Stock") {
        item2stk = "Empty";
      
      } else if (item2stk == "Empty") {
        item2stk = "In Stock";
      } 
    }
}

void handle_OnConnect() {
  server.send(200, "text/html",
              SendHTML(item1, item2, item1exp, item1stk, item1msg, item2exp,
                       item2stk, item2msg));
}

void handle_NotFound() { server.send(404, "text/plain", "Not found"); }

String SendHTML(String item1, String item2, String item1exp, String item1stk,
                String item1msg, String item2exp, String item2stk,
                String item2msg) {
  String ptr = "<!DOCTYPE html>";
  ptr += "<html>";
  ptr += "<head>";
  ptr += "<title>Smart Refrigerator</title>";
  ptr +=
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<link rel='stylesheet' "
         "href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.7.2/css/"
         "all.min.css'>";
  ptr += "<link rel='stylesheet' type='text/css' href='styles.css'>";
  ptr += "<style>";
  ptr += "body { background-color: #fff; font-family: sans-serif; color: "
         "#333333;font: 2rem; box-sizing: border-box;}";
  ptr += "#page { margin: 20px; background-color: #fff;}";
  ptr += ".container { height: inherit; padding-bottom: 20px;}";
  ptr += ".header { padding: 20px;}";
  ptr += ".header h1 { padding-bottom: 0.3em; color: #008080; font-size: 45px; "
         "font-weight: bold; font-family: Garmond, 'sans-serif'; text-align: "
         "center;}";
  ptr += "h2 { padding-bottom: 0.2em; border-bottom: 1px solid #eee; margin: "
         "2px;text-align: left;}";
  ptr += ".header h3 { font-weight: bold; font-family: Arial, 'sans-serif'; "
         "font-size: 17px; color: #b6b6b6; text-align: center;}";
  ptr += ".box-full { padding: 20px; border 1px solid #ddd; border-radius: 1em "
         "1em 1em 1em; box-shadow: 1px 7px 7px 1px rgba(0,0,0,0.4); "
         "background: #fff; margin: 20px; width: 300px;}";
  ptr += "@media (max-width: 494px) { #page { width: inherit; margin: 5px "
         "auto; }  #content { padding: 1px;} .box-full { margin: 8px 8px 12px "
         "8px; padding: 10px; width: inherit;; float: none; } }";
  ptr += "@media (min-width: 494px) and (max-width: 980px) { #page { width: "
         "465px;margin 0 auto; } .box-full { width: 380px; } }";
  ptr += "@media (min-width: 980px) { #page { width: 930px; margin: auto; } }";
  ptr += ".sensor { margin: 12px 0px; font-size: 2rem;}";
  ptr += ".sensor-labels { font-size: 1rem; vertical-align: middle;}";
  ptr += ".units { font-size: 1.2rem;}";
  ptr +=
      "hr { height: 1px; color: #eee; background-color: #eee; border: none;}";
  ptr += "</style>";

  // Ajax Code Start
  ptr += "<script>\n";
  ptr += "setInterval(loadDoc,1000);\n";
  ptr += "function loadDoc() {\n";
  ptr += "var xhttp = new XMLHttpRequest();\n";
  ptr += "xhttp.onreadystatechange = function() {\n";
  ptr += "if (this.readyState == 4 && this.status == 200) {\n";
  ptr += "document.body.innerHTML =this.responseText}\n";
  ptr += "};\n";
  ptr += "xhttp.open(\"GET\", \"/\", true);\n";
  ptr += "xhttp.send();\n";
  ptr += "}\n";
  ptr += "</script>\n";
  // Ajax Code END

  ptr += "</head>";
  ptr += "<body>";
  ptr += "<div id='page'>";
  ptr += "<div class='header'>";
  ptr += "<h1>Smart Refrigerator</h1>";
  ptr += "</div>";
  ptr += "<div id='content' align='center'>";
  ptr += "<div class='box-full' align='left'>";
  ptr += "<h2>Sensor Readings</h2>";
  ptr += "<div class='sensors-container'>";

  // For Item 1
  ptr += "<p class='sensor'>";
  ptr += "<span class='sensor-labels'> Item 1: </span>";
  ptr += item1;
  ptr+="<br />";
  ptr += "<span class='sensor-labels'> Stock: </span>";
  ptr += item1stk;
  ptr+="<br />";
  ptr += "<span class='sensor-labels'> Condition: </span>";
  ptr += item1msg;
  ptr+="<br />";
  ptr += "<span class='sensor-labels'> Exp: </span>";
  ptr += item1exp;
  ptr += "</p>";

  // For Item 2
  ptr += "<p class='sensor'>";
  ptr += "<span class='sensor-labels'> Item 2: </span>";
  ptr += item2;
  ptr+="<br />";
  ptr += "<span class='sensor-labels'> Stock: </span>";
  ptr += item2stk;
  ptr+="<br />";
  ptr += "<span class='sensor-labels'> Condition: </span>";
  ptr += item2msg;
  ptr+="<br />";
  ptr += "<span class='sensor-labels'> Exp: </span>";
  ptr += item2exp;
  ptr += "</p>";

  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</body>";
  ptr += "</html>";
  return ptr;
}