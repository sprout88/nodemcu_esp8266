#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// WiFi 설정
const char* ssid = "REDACTED";
const char* password = "REDACTED";

// 서보 설정
Servo servo;
const int servoPin = D4; // 서보 핀 설정

// 웹 서버 객체 생성
ESP8266WebServer server(80);

// 웹 페이지 HTML 코드
const char* htmlPage = 
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<title>Remote Servo Control</title>"
  "<script>"
  "function moveServo(angle) {"
  "  var xhttp = new XMLHttpRequest();"
  "  xhttp.open('GET', '/move?angle=' + angle, true);"
  "  xhttp.send();"
  "}"
  "</script>"
  "</head>"
  "<body>"
  "<h1>Remote Servo Control</h1>"
  "<p>Enter angle between 0 to 180:</p>"
  "<input type='text' id='angleInput' placeholder='Angle'>"
  "<button onclick='moveServo(document.getElementById(\"angleInput\").value)'>Move Servo</button>"
  "</body>"
  "</html>";

void setup() {
  Serial.begin(9600);
  
  // 서보 모터 초기화
  servo.attach(servoPin);

  // WiFi 연결
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  // 웹 서버 시작
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlPage);
  });

  // 서보 모터 이동 핸들러
  server.on("/move", HTTP_GET, []() {
    String angle = server.arg("angle");
    int servoAngle = angle.toInt();
    servo.write(servoAngle);
    server.send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
