# Wire Prepping Machine

## Overview
The **Wire Prepping Machine** automates the processing of 5.5mm diameter, 4-core wire by performing cutting, sheathing removal, core stripping, and tinning. The system is controlled via a local web portal and integrates sensors and actuators for seamless operation. The project focuses on automation, control system design, and sensor-actuator integration.

## Features
- **Automated Wire Processing:** Cutting, sheathing removal, core stripping, and tinning.
- **Local Web Portal Control:** Start/stop operations and monitor system status.
- **Error Handling:** Real-time error detection and display on an OLED/7-segment screen and the web portal.
- **Sensor Integration:** IR sensor for wire length detection and a limit switch for runout detection.

## System Components
### **Hardware**
- **Controller:** NodeMCU V3 ESP8266
- **Actuators:**
  - DC Motors (for wire feeding)
  - Servo Motors (for cutting, sheathing removal, and stripping/tinning)
- **Sensors:**
  - IR Sensor (for wire length measurement)
  - Limit Switch (for wire runout detection)
- **Display:** OLED or 7-segment display
- **Error Indication:** LED indicators and dry contact points

### **Software**
- Developed using **Arduino IDE**.
- Features include:
  - **Motor and Servo Control:** Sequential operations for wire processing.
  - **Web Interface:** Allows users to start/stop operations and configure wire lengths.
  - **Error Monitoring:** Detects and reports operational failures.

## Installation and Setup
### **Hardware Setup**
1. Connect the **NodeMCU ESP8266** to the actuators and sensors.
2. Wire the **DC motors** for wire feeding and **servo motors** for cutting and stripping.
3. Attach the **IR sensor** and **limit switch** for wire detection.
4. Connect the **OLED/7-segment display** for status updates.
5. Ensure error indication LEDs and dry contact points are functional.

### **Software Installation**
1. Install the **Arduino IDE**.
2. Add the required libraries:
   ```cpp
   #include <Servo.h>
   #include <ezButton.h>
   // #include <ESP8266WiFi.h>
   // #include <ESP8266WebServer.h>
   ```
3. Upload the provided code to **NodeMCU ESP8266**.
4. Configure WiFi credentials in the code:
   ```cpp
   const char* ssid = "Your_SSID";
   const char* password = "Your_PASSWORD";
   ```
5. Deploy and test the machine.

## Operational Flow
1. **Wire Detection:** Limit switch detects wire presence.
2. **Wire Feeding:** DC motor feeds wire; IR sensor measures length.
3. **Wire Cutting:** Servo motor activates the cutting mechanism.
4. **Sheathing Removal:** External sheathing is removed at both ends.
5. **Core Stripping & Tinning:** Wire cores are stripped and tinned with solder.
6. **Error Handling:** Alerts are displayed in case of failures.

## Limitations
- WiFi and display functions were not fully tested in **Proteus**.
- Wire length precision depends on motor calibration.

## Future Improvements
- Implement an advanced **error logging system**.
- Optimize **motor control algorithms** for better precision.
- Expand to handle **multiple wire diameters** and configurations.

## Contributing
Contributions are welcome! Feel free to submit **issues** and **pull requests** to improve the project.

## License
This project is licensed under the **MIT License**.

---
### **Author:** Abdullah Zunorain
For inquiries, contact: abdullahzunorain2@gmail.com

