#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include <DHT.h>

// WiFi 설정
const char* ssid = "101-2";    // WiFi 이름 입력
const char* password = "77777777";    // WiFi 비밀번호 입력

// 미세먼지 API 설정
String dusturl = "http://openAPI.seoul.go.kr:8088/52414a44616368653132364a67564550/xml/ListAirQualityByDistrictService/1/5/111121/";

// 날씨 API 설정
String weatherurl = "http://www.weather.go.kr/w/rss/dfs/hr1-forecast.do?zone=1114057000";

// 서보 설정
Servo mg995;
const int servoPin = D5;    // 서보 핀 설정

// 미세먼지 센서 설정
const int dustSensorPin = A0;
int internalDust = 0;

// DHT22 센서 설정
#define DHTPIN D6     // DHT22 센서 연결 핀
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(100);

  dht.begin();

  // WiFi 연결 시작
  WiFi.disconnect(true);
  delay(1000);
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, password); // WiFi에 연결

  while (WiFi.status() != WL_CONNECTED)   // WiFi 접속하는 동안 "." 출력
  { 
    delay(500);
    Serial.print(".");
  }

  // WiFi 연결 성공시 정보 출력
  Serial.println();
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());   // 접속된 와이파이 주소 출력
  
  mg995.attach(servoPin);
  mg995.write(0); // 초기 상태는 닫힌 상태
}

void loop() {
  // 내부 미세먼지 농도 측정
  internalDust = analogRead(dustSensorPin);

  // 외부 미세먼지 농도 측정
  int externalDust = getExternalDust();

  // 날씨 정보 가져오기
  int weather = getWeather();

  if (weather != 0) {
    mg995.write(0);   // 모터 0도로 회전 (close)
    Serial.println("Rain ! ! !");
    Serial.println("Window Closed");
  }
  else {
    Serial.println("Not Rain ! ! !");
    Serial.print("내부 미세먼지 농도: ");
    Serial.print(internalDust);
    Serial.print(", ");
    Serial.print("외부 미세먼지 농도: ");
    Serial.println(externalDust);
    
    // 미세먼지 농도에 따라 창문 제어
    if (internalDust >= externalDust) {
      mg995.write(90);    // 모터 90도로 회전 (open)
      Serial.println("Window Opened");
    } 
    else if(internalDust < externalDust) {
      mg995.write(0);   // 모터 0도로 회전 (close)
      Serial.println("Window Closed");
    }
  }

  // 온습도 정보 측정
  float humidity = dht.readHumidity();  // 습도 측정
  float temperature = dht.readTemperature();  // 온도 측정 (섭씨)

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    Serial.print("습도: ");  // 습도
    Serial.print(humidity); // 습도 값 출력
    Serial.print(" %, ");
    Serial.print("온도: ");  // 온도
    Serial.print(temperature); // 온도 값 출력
    Serial.println("°C");
  }

  delay(60000); // 1분마다 측정
}


int getExternalDust() {
  if (WiFi.status() == WL_CONNECTED) {    // 와이파이가 접속되어 있는 경우
    WiFiClient client;    // 와이파이 클라이언트 객체
    HTTPClient http;    // HTTP 클라이언트 객체

    if (http.begin(client, dusturl)) {    // HTTP
      // 서버에 연결하고 HTTP 헤더 전송
      int httpCode = http.GET();

      // httpCode 가 음수라면 에러
      if (httpCode > 0) {   // 에러가 없는 경우
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();    // 받은 XML 데이터를 String에 저장
          // XML 파싱 (여기서는 간단히 데이터 추출)
          int dustidx = payload.indexOf("<PM25>");
          int dustendIdx = payload.indexOf("</PM25>", dustidx);
          String dustStr = payload.substring(dustidx + 6, dustendIdx);
          int externalDust = dustStr.toInt();
          return externalDust;
        }
      } else {
        Serial.printf("[HTTP] GET... 실패, 에러코드: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.println("[HTTP] 접속 불가\n");
    }
  }
  return -1; // 오류 발생 시 -1 반환
}

int getWeather() {
  if (WiFi.status() == WL_CONNECTED) {    // 와이파이가 접속되어 있는 경우
    WiFiClient client;    // 와이파이 클라이언트 객체
    HTTPClient http;    // HTTP 클라이언트 객체

    if (http.begin(client, weatherurl)) {    // HTTP
      // 서버에 연결하고 HTTP 헤더 전송
      int httpCode = http.GET();

      // httpCode 가 음수라면 에러
      if (httpCode > 0) {   // 에러가 없는 경우
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();    // 받은 XML 데이터를 String에 저장
          // XML 파싱 (여기서는 간단히 데이터 추출)
          int weatheridx = payload.indexOf("<pty>");
          int weatherendIdx = payload.indexOf("</pty>", weatheridx);
          String weatherStr = payload.substring(weatheridx + 5, weatherendIdx);
          int weather = weatherStr.toInt();
          return weather;
        }
      } else {
        Serial.printf("[HTTP] GET... 실패, 에러코드: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.println("[HTTP] 접속 불가\n");
    }
  }
  return -1; // 오류 발생 시 -1 반환
}