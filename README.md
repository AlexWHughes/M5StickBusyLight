# M5StickBusyLight

This project turns your M5StickC into a BusyLight, allowing you to control its state through webhooks. You can set various states to indicate your availability or status, such as "Do Not Disturb", "Available", or "In a Meeting". The device also displays the current time and Wi-Fi details on startup.

## Features

- **Red - Do Not Disturb/In Meeting**
  - LED turns on
  - Screen displays "DnD/In Meeting"
  - Background color: Red

- **Green - Available**
  - LED turns off
  - Screen displays "Available"
  - Background color: Green

- **Blue - Available/Headphones On**
  - LED turns off
  - Screen displays "Available/Headphones On"
  - Background color: Blue
  - Smaller text size for better fit

- **Orange - Focused**
  - LED turns on
  - Screen displays "Focused"
  - Background color: Orange

- **Purple - Gin Please**
  - LED turns off
  - Screen displays "Gin Please"
  - Background color: Purple

- **RGB Chase Effect**
  - LED turns off
  - Screen cycles through RGB colors
  - Background color: Dynamic RGB

- **Black - Screen Off**
  - LED turns off
  - Screen turns off

- **Time and Wi-Fi Details**
  - Displays IP address and current time on startup
  - Automatically syncs time with an NTP server



```markdown
# M5StickC Busy Light

## Installation

### 1. Clone the repository
```bash
git clone https://github.com/AlexWHughes/M5StickBusyLight
cd M5StickC-BusyLight
```

### 2. Configure Wi-Fi Credentials
Open `src/WiFiCredentials.h` and enter your Wi-Fi SSID and password:
```cpp
#ifndef WIFI_CREDENTIALS_H
#define WIFI_CREDENTIALS_H

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

#endif // WIFI_CREDENTIALS_H
```

### 3. Upload the code
- Connect your M5StickC to your computer.
- Use PlatformIO to build and upload the code:
```bash
pio run --target upload
```

## Usage

### Start the M5StickC
The device will connect to Wi-Fi and display the IP address and current time.

### Control via Webhooks
Send HTTP GET requests to the following endpoints to change the state:
- **Red:** `http://<M5StickC_IP>/red`
- **Green:** `http://<M5StickC_IP>/green`
- **Blue:** `http://<M5StickC_IP>/blue`
- **Orange:** `http://<M5StickC_IP>/orange`
- **Purple:** `http://<M5StickC_IP>/purple`
- **RGB Chase:** `http://<M5StickC_IP>/chase`
- **Black:** `http://<M5StickC_IP>/black`

### Automatic Screen Off and On
- The screen turns off automatically at 18:00.
- The screen turns back on automatically at 09:00.

## License
This project is licensed under the GPL License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements
Thanks to the M5Stack team for creating the M5StickC. This project uses the following libraries:
- M5StickC library
- ESPAsyncWebServer library
- NTPClient library
- TimeLib library
```

