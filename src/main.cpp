#include <M5StickC.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "WiFiCredentials.h" // Include the Wi-Fi credentials header

// NTP server and timezone
const char* ntpServer = "au.pool.ntp.org";
const long gmtOffset_sec = 36000; // GMT+10 for Melbourne/Australia
const int daylightOffset_sec = 3600; // Daylight saving time offset

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// NTP client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, 60000);

bool screenOff = false;
unsigned long lastDisplayUpdate = 0;
bool showClock = true;

void displayTimeAndWiFi() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Connected to WiFi");
    M5.Lcd.println("IP Address:");
    M5.Lcd.println(WiFi.localIP().toString());

    String currentTime = String(hour()) + ":" + String(minute()) + ":" + String(second());
    M5.Lcd.println("Current Time:");
    M5.Lcd.println(currentTime);
}

void displayStatus(const char* status1, const char* status2 = "", int textSize = 2) {
    M5.Lcd.setCursor(0, 10); // Adjust cursor position to the top
    M5.Lcd.setTextColor(BLACK); // Set text color to black for better contrast
    M5.Lcd.setTextSize(textSize); // Set text size
    M5.Lcd.printf("%s\n%s", status1, status2);
}

void setup() {
    // Initialize M5StickC
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.setTextFont(2);

    // Initialize the LED pin
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH); // Turn off LED

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Start NTP client
    timeClient.begin();
    while (!timeClient.update()) {
        timeClient.forceUpdate();
    }

    // Get the current time
    unsigned long epochTime = timeClient.getEpochTime();
    setTime(epochTime);

    // Print IP Address and time
    displayTimeAndWiFi();

    // Define webhooks
    server.on("/red", HTTP_GET, [](AsyncWebServerRequest *request){
        M5.Axp.SetLDO2(true); // Turn the screen back on
        screenOff = false;
        M5.Lcd.fillScreen(RED);
        displayStatus("DnD/In", "Meeting");
        digitalWrite(10, LOW); // Turn on the LED
        showClock = false;
        request->send(200, "text/plain", "Red light on");
    });

    server.on("/green", HTTP_GET, [](AsyncWebServerRequest *request){
        M5.Axp.SetLDO2(true); // Turn the screen back on
        screenOff = false;
        M5.Lcd.fillScreen(GREEN);
        displayStatus("Available");
        digitalWrite(10, HIGH); // Turn off the LED
        showClock = false;
        request->send(200, "text/plain", "Green light on");
    });

    server.on("/blue", HTTP_GET, [](AsyncWebServerRequest *request){
        M5.Axp.SetLDO2(true); // Turn the screen back on
        screenOff = false;
        M5.Lcd.fillScreen(BLUE);
        displayStatus("Available/", "Headphones On", 1); // Smaller text size
        digitalWrite(10, HIGH); // Turn off the LED
        showClock = false;
        request->send(200, "text/plain", "Blue light on");
    });

    server.on("/orange", HTTP_GET, [](AsyncWebServerRequest *request){
        M5.Axp.SetLDO2(true); // Turn the screen back on
        screenOff = false;
        M5.Lcd.fillScreen(TFT_ORANGE);
        displayStatus("Focused");
        digitalWrite(10, LOW); // Turn on the LED
        showClock = false;
        request->send(200, "text/plain", "Orange light on");
    });

    server.on("/purple", HTTP_GET, [](AsyncWebServerRequest *request){
        M5.Axp.SetLDO2(true); // Turn the screen back on
        screenOff = false;
        M5.Lcd.fillScreen(TFT_PURPLE);
        displayStatus("Gin", "Please");
        digitalWrite(10, HIGH); // Turn off the LED
        showClock = false;
        request->send(200, "text/plain", "Purple light on");
    });

    server.on("/chase", HTTP_GET, [](AsyncWebServerRequest *request){
        M5.Axp.SetLDO2(true); // Turn the screen back on
        screenOff = false;
        showClock = false;
        for (int i = 0; i < 255; i++) {
            M5.Lcd.fillScreen(M5.Lcd.color565(i, 0, 255 - i));
            delay(5);
        }
        for (int i = 0; i < 255; i++) {
            M5.Lcd.fillScreen(M5.Lcd.color565(255 - i, i, 0));
            delay(5);
        }
        for (int i = 0; i < 255; i++) {
            M5.Lcd.fillScreen(M5.Lcd.color565(0, 255 - i, i));
            delay(5);
        }
        request->send(200, "text/plain", "RGB chase effect");
    });

    server.on("/black", HTTP_GET, [](AsyncWebServerRequest *request){
        M5.Axp.SetLDO2(false); // Turn off screen
        digitalWrite(10, HIGH); // Turn off the LED
        screenOff = true;
        showClock = false;
        request->send(200, "text/plain", "Screen off");
    });

    server.begin();
}

void loop() {
    // Update the NTP client
    timeClient.update();

    // Get the current time
    unsigned long epochTime = timeClient.getEpochTime();
    setTime(epochTime);

    // Update display every second if showClock is true
    if (showClock && (millis() - lastDisplayUpdate >= 1000)) {
        displayTimeAndWiFi();
        lastDisplayUpdate = millis();
    }

    // Check if it's 18:00
    if (hour() == 18 && minute() == 0 && !screenOff) {
        M5.Axp.SetLDO2(false); // Turn off screen
        screenOff = true;
    }

    // Reset screenOff flag at 9:00
    if (hour() == 9 && minute() == 0) {
        screenOff = false;
    }

    delay(100); // Short delay to allow other tasks to run
}
