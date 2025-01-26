#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi Credentials
const char* ssid = "Lenovo";
const char* password = "0123456789";

// Pin definitions
const int enA = D5;   // Motor driver enA
const int enB = D6;   // Motor driver enB
const int flameSensor = D8; // Flame sensor
const int buzzer = D7;      // Buzzer
const int waterPump = D4;   // Water pump relay

// Motor control pins
const int motorS0 = D0; // IN1
const int motorS1 = D1; // IN2
const int motorS2 = D2; // IN3
const int motorS3 = D3; // IN4

ESP8266WebServer server(80);
int speedValue = 0; // Initial speed

// HTML page for web control
// String html = R"rawliteral(
// <!DOCTYPE html>
// <html>
//   <head>
//     <title>Ignis Sentinel Controller</title>
//     <style>
//       body {
//         font-family: Arial, sans-serif;
//         text-align: center;
//         background-color: #f4f4f4;
//         margin: 0;
//         padding: 0;
//       }
//       .container {
//         max-width: 400px;
//         margin: 40px auto;
//         padding: 20px;
//         background-color: #fff;
//         border-radius: 15px;
//         box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
//       }
//       h1 {
//         color: #333;
//         margin-bottom: 20px;
//       }
//       .remote {
//         display: grid;
//         grid-template-columns: 1fr 1fr 1fr;
//         gap: 10px;
//         justify-items: center;
//         align-items: center;
//         margin: 20px 0;
//       }
//       .remote button {
//         background-color: #007BFF;
//         color: white;
//         border: none;
//         border-radius: 8px;
//         padding: 10px 20px;
//         font-size: 16px;
//         cursor: pointer;
//         transition: background-color 0.2s ease, transform 0.1s ease;
//       }
//       .remote button:hover {
//         background-color: #0056b3;
//         transform: scale(1.05);
//       }
//       .remote .stop {
//         background-color: #FF4D4D;
//       }
//       .remote .stop:hover {
//         background-color: #cc0000;
//       }
//       .remote .pump-on {
//         background-color: #90EE90; /* Light Green */
//         color: black;
//       }
//       .remote .pump-on:hover {
//         background-color: #7ed47e;
//       }
//       .remote .pump-off {
//         background-color: #FF6347; /* Red */
//       }
//       .remote .pump-off:hover {
//         background-color: #e5533d;
//       }
//       .slider-container {
//         margin-top: 20px;
//         text-align: center;
//       }
//       .slider-container label {
//         font-weight: bold;
//         display: block;
//         margin-bottom: 8px;
//       }
//       .slider-container input {
//         width: 100%;
//       }
//       footer {
//         margin-top: 20px;
//         font-size: 12px;
//         color: #777;
//       }
//     </style>
//   </head>
//   <body>
//     <div class="container">
//       <h1>Ignis Sentinel Remote</h1>
//       <div class="remote">
//         <button onclick="move('forward')">Forward</button>
//         <button onclick="move('stop')" class="stop">Stop</button>
//         <button onclick="move('backward')">Backward</button>
//         <button onclick="move('left')">Left</button>
//         <button onclick="move('right')">Right</button>
//         <button onclick="controlPump('on')" class="pump-on">Pump ON</button>
//         <button onclick="controlPump('off')" class="pump-off">Pump OFF</button>
//       </div>
//       <div class="slider-container">
//         <label for="speed">Adjust Speed:</label>
//         <input type="range" id="speed" min="0" max="255" value="0" oninput="setSpeed(this.value)">
//         <p>Speed: <span id="speedValue">0</span></p>
//       </div>
//     </div>
//     <footer>
//       &copy; 2025 Ignis Sentinel Controller | Powered by NodeMCU
//     </footer>
//     <script>
//       function move(direction) {
//         fetch('/move?dir=' + direction);
//       }
//       function setSpeed(value) {
//         document.getElementById('speedValue').innerText = value;
//         fetch('/speed?value=' + value);
//       }
//       function controlPump(state) {
//         fetch('/pump?state=' + state);
//       }
//     </script>
//   </body>
// </html>
// )rawliteral";





String html = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>Ignis Sentinel Controller</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        text-align: center;
        background-color: #f4f4f4;
        margin: 0;
        padding: 0;
      }
      .container {
        max-width: 400px;
        margin: 40px auto;
        padding: 20px;
        background-color: #fff;
        border-radius: 15px;
        box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
      }
      h1 {
        color: #333;
        margin-bottom: 20px;
      }
      .remote {
        display: grid;
        grid-template-columns: 1fr 1fr 1fr;
        gap: 10px;
        justify-items: center;
        align-items: center;
        margin: 20px 0;
      }
      .remote button {
        background-color: #007BFF;
        color: white;
        border: none;
        border-radius: 8px;
        padding: 10px 20px;
        font-size: 16px;
        cursor: pointer;
        transition: background-color 0.2s ease, transform 0.1s ease;
      }
      .remote button:hover {
        background-color: #0056b3;
        transform: scale(1.05);
      }
      .remote .stop {
        background-color: #FF4D4D;
      }
      .remote .stop:hover {
        background-color: #cc0000;
      }
      .remote .pump-on {
        background-color: #90EE90; /* Light Green */
        color: black;
      }
      .remote .pump-on:hover {
        background-color: #7ed47e;
      }
      .remote .pump-off {
        background-color: #FF6347; /* Red */
      }
      .remote .pump-off:hover {
        background-color: #e5533d;
      }
      .slider-container {
        margin-top: 20px;
        text-align: center;
      }
      .slider-container label {
        font-weight: bold;
        display: block;
        margin-bottom: 8px;
      }
      .slider-container input {
        width: 100%;
      }
      footer {
        margin-top: 20px;
        font-size: 12px;
        color: #777;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>Ignis Sentinel Remote</h1>
      <div class="remote">
        <div></div>
        <button onclick="move('forward')">Forward</button>
        <div></div>
        <button onclick="move('left')">Left</button>
        <button onclick="move('stop')" class="stop">Stop</button>
        <button onclick="move('right')">Right</button>
        <div></div>
        <button onclick="move('backward')">Backward</button>
        <div></div>
      </div>
      <div class="remote" style="grid-template-columns: 1fr 1fr; gap: 20px;">
        <button onclick="controlPump('on')" class="pump-on">Pump ON</button>
        <button onclick="controlPump('off')" class="pump-off">Pump OFF</button>
      </div>
      <div class="slider-container">
        <label for="speed">Adjust Speed:</label>
        <input type="range" id="speed" min="0" max="255" value="0" oninput="setSpeed(this.value)">
        <p>Speed: <span id="speedValue">0</span></p>
      </div>
    </div>
    <footer>
      &copy; 2025 Ignis Sentinel Controller | Powered by NodeMCU
    </footer>
    <script>
      function move(direction) {
        fetch('/move?dir=' + direction);
      }
      function setSpeed(value) {
        document.getElementById('speedValue').innerText = value;
        fetch('/speed?value=' + value);
      }
      function controlPump(state) {
        fetch('/pump?state=' + state);
      }
    </script>
  </body>
</html>
)rawliteral";


void setup() {
  // Setup serial communication
  Serial.begin(115200);

  // Setup pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(motorS0, OUTPUT);
  pinMode(motorS1, OUTPUT);
  pinMode(motorS2, OUTPUT);
  pinMode(motorS3, OUTPUT);
  pinMode(flameSensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, LOW); // Ensure pump is off initially

  // Initialize WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected! IP Address: " + WiFi.localIP().toString());

  // Setup server routes
  server.on("/", []() { server.send(200, "text/html", html); });
  server.on("/move", handleMovement);
  server.on("/speed", handleSpeed);
  server.on("/pump", handlePump);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Flame detection logic
  int flameValue = digitalRead(flameSensor);
  if (flameValue == LOW) { // Flame detected
    digitalWrite(buzzer, HIGH);
    Serial.println("Flame detected! Buzzer activated.");
  } else {
    digitalWrite(buzzer, LOW);
  }
}

// Handle movement requests
void handleMovement() {
  String dir = server.arg("dir");
  if (dir == "forward") {
    moveForward();
  } else if (dir == "backward") {
    moveBackward();
  } else if (dir == "left") {
    turnLeft();
  } else if (dir == "right") {
    turnRight();
  } else if (dir == "stop") {
    stopCar();
  }
  server.send(200, "text/plain", "OK");
}

// Handle speed adjustments
void handleSpeed() {
  String value = server.arg("value");
  speedValue = value.toInt();
  analogWrite(enA, speedValue);
  analogWrite(enB, speedValue);
  server.send(200, "text/plain", "Speed set to " + value);
}

// Handle pump control
void handlePump() {
  String state = server.arg("state");
  if (state == "on") {
    digitalWrite(waterPump, HIGH);
    Serial.println("Water pump turned ON");
  } else if (state == "off") {
    digitalWrite(waterPump, LOW);
    Serial.println("Water pump turned OFF");
  }
  server.send(200, "text/plain", "Pump state set to " + state);
}

// Movement functions
void moveForward() {
  digitalWrite(motorS0, LOW);
  digitalWrite(motorS1, HIGH);
  digitalWrite(motorS2, LOW);
  digitalWrite(motorS3, HIGH);
  Serial.println("Moving forward");
}

void moveBackward() {
  digitalWrite(motorS0, HIGH);
  digitalWrite(motorS1, LOW);
  digitalWrite(motorS2, HIGH);
  digitalWrite(motorS3, LOW);
  Serial.println("Moving backward");
}

void turnLeft() {
  digitalWrite(motorS0, LOW);
  digitalWrite(motorS1, HIGH);
  digitalWrite(motorS2, HIGH);
  digitalWrite(motorS3, LOW);
  Serial.println("Turning left");
}

void turnRight() {
  digitalWrite(motorS0, HIGH);
  digitalWrite(motorS1, LOW);
  digitalWrite(motorS2, LOW);
  digitalWrite(motorS3, HIGH);
  Serial.println("Turning right");
}

void stopCar() {
  digitalWrite(motorS0, LOW);
  digitalWrite(motorS1, LOW);
  digitalWrite(motorS2, LOW);
  digitalWrite(motorS3, LOW);
  Serial.println("Car stopped");
}
