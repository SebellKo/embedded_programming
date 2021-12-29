#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

String readString;

LiquidCrystal_I2C lcd(0x27, 16, 2);
int echoPin = 8;
int trigPin = 9;

SoftwareSerial BTSerial(2, 3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
//  lcd.print("Hello, world!");
//  lcd.setCursor(0, 1);
//  lcd.print("lcd test");
}

void loop() {
  // put your main code here, to run repeatedly:

  // 초음파 센서 모듈 이용
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);

  float distance = ((float)(340 * duration) / 10000) / 2;

  int isCustomMessage = 0;

//  Serial.print(distance);
//  Serial.println("Cm\n");
  
   // 초음파 센서 모듈 이용 종료
   
  // 블루투스 모듈 이용
  if (distance < 20) {
      Serial.write("distance\n");
      BTSerial.println("외부인이 출입했습니다"); // 블루투스로 텍스트 전송
      delay(300);
   }
  while (BTSerial.available()) {
//    char data[100];
      char data = BTSerial.read();
      readString += data;
//    for (int i = 0; i < 100; i++)
//    {
//      data[i] = BTSerial.read();
//    } // 블루투스 데이터 읽기
    Serial.write(data);
    
    
  // 블루투스로 읽어오는 텍스트가 1 일때 lcd에 문자열 띄우기
    if(data == '1') {
      lcd.init();
      lcd.backlight();
      lcd.print("Give me a call.");
      lcd.setCursor(0,1);
      lcd.print("010-6645-4161");
      Serial.write("connected\n");
      readString = "";
    }
    if(data == '2') {
      lcd.init();
      lcd.backlight();
      lcd.print("Give me a text.");
      lcd.setCursor(0,1);
      lcd.print("010-6645-4161");
      Serial.write("connected\n");
      readString = "";
    }
    if(data == '3') {
      lcd.init();
      lcd.backlight();
      lcd.print("Please leave it");
      lcd.setCursor(2,1);
      lcd.print("at the door.");
      Serial.write("connected\n");
      readString = "";
    }
    if (data != '1' && data != '2' && data != '3')
    {
      lcd.clear();
      lcd.print(readString);
      Serial.println(readString);
      isCustomMessage = 1;
    }
  }
  delay(1000);
  if (isCustomMessage == 1)
  {
    for (int posCnt = 0; posCnt < readString.length(); posCnt++) { // 움직일 글자 수 13
        lcd.scrollDisplayLeft();
        delay(1000);
      }
      
      // 글자 오른쪽 끝으로 이동
      for (int posCnt = 0; posCnt < readString.length() + 16; posCnt++) { // 글자 수 13 + 칸 수 16
        lcd.scrollDisplayRight();
        delay(1000);    
      }
    
      // 글자 원위치
      for (int posCnt = 0; posCnt < readString.length(); posCnt++) { // 칸 수 16
        lcd.scrollDisplayLeft();
        delay(1000);
      }
  }
  readString = "";
  
  // 블루투스 모듈 이용 종료
  if (Serial.available())
  {
    BTSerial.write(Serial.read());
  }
  

  delay(500);
}
