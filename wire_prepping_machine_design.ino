#include <Servo.h>
#include <ezButton.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>

// Define pins for peripherals
#define LIMIT_SWITCH_PIN 5   // Limit switch to detect wire runout
#define IR_SENSOR_PIN 9      // IR sensor for rotation count
#define DC_MOTOR_IN1 7       // DC motor IN1
#define DC_MOTOR_IN2 6       // DC motor IN2
#define DC_MOTOR_ENA 4       // DC motor enable (PWM control)
#define SERVO_SHEATH 3       // Servo for sheathing removal
#define SERVO_SOLDER 2       // Servo for soldering
#define SERVO_CUTTER 8       // Servo for wire cutting
#define ERROR_LED 0          // LED for error indication
//#define ERROR_CONTACT_PIN 10 // Optional dry contact for external error reporting (commented as requested)

// WiFi credentials (commented as requested)
//const char* ssid = "Your_SSID";
//const char* password = "Your_PASSWORD";

// Servo objects
Servo servoSheath;
Servo servoSolder;
Servo servoCutter;

// Button object for the limit switch
ezButton limitSwitch(LIMIT_SWITCH_PIN);

// Variables for system state
volatile int rotationCount = 0;  // Count of rotations detected by the IR sensor
bool wirePresent = false;        // Wire presence status
bool errorFlag = false;          // System error state
bool taskRunning = false;        // Task in progress

// Configurable parameters
int wireLength = 10;          // Wire length in cm (can be adjusted via web input)
int sheathLength = 2;         // Sheathing removal length in cm
int rotationsPerCm = 2;       // IR sensor rotations per cm of wire

// ESP8266 Web Server
// ESP8266WebServer server(80);

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  Serial.println("Wire Preparation Machine Initializing...");

  // Limit switch debounce setup
  limitSwitch.setDebounceTime(50);

  // Initialize IR sensor pin
  pinMode(IR_SENSOR_PIN, INPUT);

  // Initialize DC motor pins
  pinMode(DC_MOTOR_IN1, OUTPUT);
  pinMode(DC_MOTOR_IN2, OUTPUT);
  pinMode(DC_MOTOR_ENA, OUTPUT);

  // Initialize error LED
  pinMode(ERROR_LED, OUTPUT);
  //pinMode(ERROR_CONTACT_PIN, OUTPUT); // Optional dry contact for error (commented as requested)

  // Attach servo motors
  servoSheath.attach(SERVO_SHEATH);
  servoSolder.attach(SERVO_SOLDER);
  servoCutter.attach(SERVO_CUTTER);

  // Reset components
  stopDCMotor();
  servoSheath.write(0);
  servoSolder.write(0);
  servoCutter.write(0);

  // Setup WiFi (commented as requested)
  //WiFi.begin(ssid, password);
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
  //  Serial.print(".");
  //}
  //Serial.println("\nWiFi connected");
  //Serial.print("IP Address: ");
  //Serial.println(WiFi.localIP());

  // // Configure Web Server routes
  // server.on("/", handleRoot);
  // server.on("/set", handleSetParameters);
  // server.begin();
  // Serial.println("Web server started.");

  Serial.println("Setup Complete. Ready for Operation.");
}

void loop() {
  // Update limit switch state
  limitSwitch.loop();

  // Check for wire presence
  wirePresent = limitSwitch.getState() == LOW;

  // Handle web server requests
  // server.handleClient();

  // Display status on Serial Monitor
  displayStatus();

  // Handle errors if wire is not detected
  if (!wirePresent) {
    handleError("Wire not detected. System halted.");
    return;
  } else {
    clearError();
  }

  // If no errors and not already running, start task
  if (!taskRunning) {
    taskRunning = true;

    // Execute the steps in sequence
    if (checkWirePresence()) feedWire(wireLength);
    if (checkWirePresence()) cutWire();
    if (checkWirePresence()) removeSheath(sheathLength);
    if (checkWirePresence()) stripAndTinCores();

    taskRunning = false; // Mark task complete
  }

  // Pause briefly before next iteration
  delay(1000);
}

// Check wire presence during operation
bool checkWirePresence() {
  limitSwitch.loop();
  wirePresent = limitSwitch.getState() == LOW;
  if (!wirePresent) {
    handleError("Wire lost during operation. Halting.");
    return false;
  }
  return true;
}

// Feed wire by rotating the motor
void feedWire(int length) {
  Serial.print("Feeding wire: ");
  Serial.print(length);
  Serial.println(" cm...");

  rotationCount = 0; // Reset rotation count
  analogWrite(DC_MOTOR_ENA, 200); // Set motor speed
  digitalWrite(DC_MOTOR_IN1, HIGH);
  digitalWrite(DC_MOTOR_IN2, LOW);

  while (rotationCount < length * rotationsPerCm) {
    if (digitalRead(IR_SENSOR_PIN) == HIGH) {
      rotationCount++;
      delay(50); // Debounce delay
    }
  }

  stopDCMotor();
  Serial.println("Wire feeding complete.");
}

// Cut wire using servo
void cutWire() {
  Serial.println("Cutting the wire...");
  servoCutter.write(90); // Move cutter servo to cutting position
  delay(1000);           // Simulate cutting time
  servoCutter.write(0);  // Return servo to initial position
  Serial.println("Wire cutting complete.");
}

// Remove sheath using servo
void removeSheath(int length) {
  Serial.print("Removing sheath: ");
  Serial.print(length);
  Serial.println(" cm...");
  servoSheath.write(90); // Move servo to sheathing removal position
  delay(length * 500);   // Simulate removal time
  servoSheath.write(0);  // Return to initial position
  Serial.println("Sheath removal complete.");
}

// Strip and tin cores using soldering servo
void stripAndTinCores() {
  Serial.println("Stripping and tinning cores...");
  servoSolder.write(90); // Move soldering servo to position
  delay(2000);           // Simulate tinning process
  servoSolder.write(0);  // Return to initial position
  Serial.println("Stripping and tinning complete.");
}

// Stop the DC motor
void stopDCMotor() {
  digitalWrite(DC_MOTOR_IN1, LOW);
  digitalWrite(DC_MOTOR_IN2, LOW);
  analogWrite(DC_MOTOR_ENA, 0);
}

// Display current status on Serial Monitor
void displayStatus() {
  Serial.println("=======================");
  Serial.print("Wire Presence: ");
  Serial.println(wirePresent ? "Detected" : "Not Detected");
  Serial.print("Rotation Count: ");
  Serial.println(rotationCount);
  Serial.print("Error Status: ");
  Serial.println(errorFlag ? "ERROR" : "No Errors");
  Serial.println("=======================");
}

// Handle errors and halt operation
void handleError(const char* errorMessage) {
  errorFlag = true;
  digitalWrite(ERROR_LED, HIGH);
  //digitalWrite(ERROR_CONTACT_PIN, HIGH); // Indicate error via dry contact (commented as requested)
  stopDCMotor();
  Serial.print("ERROR: ");
  Serial.println(errorMessage);
  delay(1000); // Wait before retry
}

// Clear error state
void clearError() {
  errorFlag = false;
  digitalWrite(ERROR_LED, LOW);
  //digitalWrite(ERROR_CONTACT_PIN, LOW); // Reset dry contact (commented as requested)
}

// // Web server root handler
// void handleRoot() {
//   server.send(200, "text/html", "<h1>Wire Preparation Machine</h1><p>Use /set?length=VALUE&sheath=VALUE to set parameters.</p>");
// }

// Web server parameter handler
// void handleSetParameters() {
  // if (server.hasArg("length")) {
    // wireLength = server.arg("length").toInt();
  // }
  // if (server.hasArg("sheath")) {
    // sheathLength = server.arg("sheath").toInt();
  // }
//   String message = "<h1>Parameters Updated</h1><p>Wire Length: ";
//   message += wireLength;
//   message += " cm<br>Sheath Length: ";
//   message += sheathLength;
//   message += " cm</p>";
//   server.send(200, "text/html", message);
// }
