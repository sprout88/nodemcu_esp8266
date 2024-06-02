#include <Servo.h>

Servo servo;

void setup() {
  servo.attach(D4); // D1에 연결된 핀 사용, 필요에 따라 다른 핀을 사용할 수 있습니다.
}

void loop() {
  servo.write(90); // 180도 각도로 서보 모터를 이동
  delay(3000);     // 1초 대기

  servo.write(180);  // 0도 각도로 서보 모터를 이동
  delay(3000);     // 1초 대기
}