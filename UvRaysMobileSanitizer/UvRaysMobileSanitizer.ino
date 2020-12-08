/*
 Author : ...
 Start Date : 19 JUN 2020
 Modified Date : 19 JUN 2020
 Dec : It is a Sanitizer machine which can sanitiz mobiles, cash, wallet, etc. with the help of uv rays
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x3f for upload ardeuno 0X3F
// the setup function runs once when you press reset or power the board
const int buttonPin = 2;
int buttonState = 0;
int DoorState = 0;
int PreState = 0;
int CurrentState = HIGH;
int PushButtonCount = 0;
int SanitiztionProcess = -1;
int IsAlarmed = 0;
int WaitTimeCount = 0;
int IsUnsafe = 0;
int IsCollacte = 0;
const int pingPin = 10;
const int echoPin = 11;
const int UvOut = 4;
int distance = 50000;
int  IsStart = 0;
void setup() {
  Serial.begin(9600);
  lcd.init();             // initialize the lcd 
  lcd.backlight();        // initialize the lcd 
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("GIT PROJECT");
  lcd.setCursor(0,1);
  lcd.print("cash sanitizer machine");
  DDRD = 0x00;
  PORTD = 0x01; 
  // initialize digital pin UvOut as an output. YT 
  pinMode(UvOut, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(8, INPUT);
  pinMode(pingPin, OUTPUT); // initialising pin 10 as output
  pinMode(echoPin, INPUT); // initialising pin 11 as input

}

// the loop function runs over and over again forever
void loop()
{
    DoorState = digitalRead(8);
    long duration, inches, cm;
  
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    
    digitalWrite(pingPin, LOW);

  
    duration = pulseIn(echoPin, HIGH); // using pulsin function to determine total time
    inches = microsecondsToInches(duration); // calling method
    cm = microsecondsToCentimeters(duration); // calling method
    distance = cm;

    if(distance < 20){ // change UltraSonic sensor lenth change
      IsStart = 1;
      CurrentState = HIGH;
    }else{
      if(IsStart != 1){
        IsStart = 0;
        CurrentState = LOW;
      }
    }
    Serial.print(DoorState);

    if(DoorState == HIGH){
      digitalWrite(UvOut, LOW);
      digitalWrite(7, LOW);
      digitalWrite(12, HIGH);
      IsStart = 0;
      lcd.setCursor(0,1);
      if(SanitiztionProcess == 2 and IsUnsafe != 2){
        IsUnsafe = 1; 
      }
      if((SanitiztionProcess == 2 or SanitiztionProcess == -2) and IsUnsafe == 1){
         IsUnsafe = 1;
         SanitiztionProcess = -2;
         lcd.print("Door opend [Unsafe].................................");
      }else{
        if(SanitiztionProcess == 1 or IsCollacte == 1 or IsUnsafe == 2){
          IsCollacte = 1;
        }else{
          IsCollacte = 0;
        }
        IsUnsafe = 0;    
        SanitiztionProcess = -2;
        if(IsCollacte == 1){
          lcd.print("Door opend[Collacte].................................");
        }else{
          lcd.print("Door opend.................................");
        }
      }
      String  PushButtonCountMsg = "Cash sanitiz : ";
      PushButtonCountMsg.concat(PushButtonCount);
      lcd.setCursor(0,0);
      lcd.print(PushButtonCountMsg);
    }else if(CurrentState == HIGH and (SanitiztionProcess == 0 or SanitiztionProcess == 2)){
      IsCollacte = 0;
      IsAlarmed = 0;
      digitalWrite(12, LOW);
      digitalWrite(7, LOW);
      lcd.setCursor(0,1);
      lcd.print("running....................................");
      digitalWrite(UvOut, HIGH);
      String  TmpWaitTime = "Wait time :";
      TmpWaitTime.concat((60*1) - WaitTimeCount); // Uv Led delay time
      TmpWaitTime.concat("......");
      lcd.setCursor(0,0);
      lcd.print(TmpWaitTime);
      delay(70);
      if(((60*1) - WaitTimeCount) <= 1){  // Uv Led delay time
        SanitiztionProcess = 1;
        PushButtonCount++;
      }else{
        SanitiztionProcess = 2;
      }
      WaitTimeCount++;
    }else if(CurrentState == HIGH and SanitiztionProcess == 1){
      digitalWrite(UvOut, LOW);
      IsCollacte = 0;
      IsUnsafe = 2;
      WaitTimeCount = 0;
      digitalWrite(12, LOW);
      lcd.setCursor(0,1);
      lcd.print("completed..................................");
      if(IsAlarmed == 0){
        digitalWrite(7, HIGH);
        WaitAlarmTime(20);   // Alarm delay time
        digitalWrite(7, LOW);
        IsAlarmed = 1;
      }
    }else{
      digitalWrite(UvOut, LOW);
      digitalWrite(12, LOW);
      digitalWrite(7, LOW);
      WaitTimeCount = 0;
      IsCollacte = 0;
      lcd.setCursor(0,1);
      if(SanitiztionProcess == -1 or SanitiztionProcess == -2){
         SanitiztionProcess = 0;
      }else if(SanitiztionProcess == 0 or SanitiztionProcess == 1){
        SanitiztionProcess = 0;
        if(IsUnsafe != 2){
          IsUnsafe = 0;
        } 
        lcd.print("Ready...................................");
      }else{
        digitalWrite(UvOut, LOW);
        digitalWrite(12, HIGH);
        lcd.print("Cancelled...................................");
      }
      IsAlarmed = 1;
    }
    
    PreState = CurrentState;
    if(CurrentState == LOW or SanitiztionProcess == 1){
      String  PushButtonCountMsg = "Cash sanitiz : ";
      PushButtonCountMsg.concat(PushButtonCount);
      lcd.setCursor(0,0);
      lcd.print(PushButtonCountMsg);
    }
}
void WaitAlarmTime(int timesec){
  for(int i = 0; i <= timesec; i++){
    String  WaitTime = "Alarming for: ";
    WaitTime.concat((timesec) - i);
     WaitTime.concat(".........");
    lcd.setCursor(0,0);
    lcd.print(WaitTime);
    delay(125);
  }
}

long microsecondsToInches(long microseconds){
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds){
  return microseconds / 29 / 2;
}
