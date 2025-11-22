#include <WebServer.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define SERVO_FL 13 // Предно ляво
#define SERVO_FR 14 // Предно дясно
#define SERVO_RL 25 // Задно ляво
#define SERVO_RR 27 // Задно дясно

Servo servoFL, servoFR, servoRL, servoRR;

const char* ssid = "Stoyan’s iPhone";
const char* password = "12345678";

WebServer server(80);

const int STOP_ANGLE = 90;
const int FORWARD_ANGLE = 60;
const int BACKWARD_ANGLE = 120;

void stopAll() {
  servoFL.write(STOP_ANGLE);
  servoFR.write(STOP_ANGLE);
  servoRL.write(STOP_ANGLE);
  servoRR.write(STOP_ANGLE);
}

void moveBackward() {
  servoFL.write(BACKWARD_ANGLE);
  servoFR.write(BACKWARD_ANGLE);
  servoRL.write(FORWARD_ANGLE);
  servoRR.write(FORWARD_ANGLE);
}

void moveForward() {
  servoFL.write(FORWARD_ANGLE);
  servoFR.write(FORWARD_ANGLE);
  servoRL.write(BACKWARD_ANGLE);
  servoRR.write(BACKWARD_ANGLE);
}

void turnRight() {
  servoFL.write(BACKWARD_ANGLE);
  servoFR.write(BACKWARD_ANGLE);
  servoRL.write(BACKWARD_ANGLE);
  servoRR.write(BACKWARD_ANGLE);
}

void turnLeft() {
  servoFL.write(FORWARD_ANGLE);
  servoFR.write(FORWARD_ANGLE);
  servoRL.write(FORWARD_ANGLE);
  servoRR.write(FORWARD_ANGLE);
}

String getHTML() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Car Control</title>
  <style>
    body {
      font-family: Arial;
      text-align: center;
      background-color: #f0f0f0;
    }
    h1 {
      color: #333;
    }
    .button {
      padding: 20px 40px;
      margin: 10px;
      font-size: 24px;
      border: none;
      border-radius: 10px;
      background-color: #2196F3;
      color: white;
      cursor: pointer;
    }
    .button:hover {
      background-color: #0b7dda;
    }
  </style>
</head>
<body>
  <h1>ESP32 Car Control</h1>
  <button class="button" onclick="fetch('/forward')">Forward</button><br>
  <button class="button" onclick="fetch('/left')">Left</button>
  <button class="button" onclick="fetch('/stop')">Stop</button>
  <button class="button" onclick="fetch('/right')">Right</button><br>
  <button class="button" onclick="fetch('/backward')">Backward</button>
</body>
</html>
)rawliteral";
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void setup() {
  Serial.begin(115200);

  servoFL.attach(SERVO_FL);
  servoFR.attach(SERVO_FR);
  servoRL.attach(SERVO_RL);
  servoRR.attach(SERVO_RR);

  stopAll();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/forward", []() {
    moveForward();
    server.send(200, "text/plain", "Moving Forward");
  });
  server.on("/backward", []() {
    moveBackward();
    server.send(200, "text/plain", "Moving Backward");
  });
  server.on("/left", []() {
    turnLeft();
    server.send(200, "text/plain", "Turning Left");
  });
  server.on("/right", []() {
    turnRight();
    server.send(200, "text/plain", "Turning Right");
  });
  server.on("/stop", []() {
    stopAll();
    server.send(200, "text/plain", "Stopped");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}