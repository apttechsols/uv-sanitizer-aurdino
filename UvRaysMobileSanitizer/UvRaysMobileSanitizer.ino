/*
 Author : ...
 Start Date : 19 JUN 2020
 Modified Date : 19 JUN 2020
 Dec : It is a Sanitizer machine which can sanitiz mobiles, cash, wallet, etc. with the help of uv rays
 Parner : GIT organization
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,5);  // set the LCD address to 0x3f for upload ardeuno 0X3F
// the setup function runs once when you press reset or power the board
const int buttonPin = 2;
int buttonState = 0;
int DoorState = 0;
int PreState = 0;
int CurrentState = 0;
int PushButtonCount = 0;
int SanitiztionProcess = -1;
int IsAlarmed = 0;
int WaitTimeCount = 0;
int IsUnsafe = 0;
void setup() {
  Serial.begin(9600);
  lcd.init();             // initialize the lcd 
  lcd.backlight();        // initialize the lcd 
  lcd.begin(16, 5);
  lcd.setCursor(0,0);
  lcd.print("GIT PROJECT");
  lcd.setCursor(0,1);
  lcd.print("cash sanitizer machine");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(8, INPUT);

}

// the loop function runs over and over again forever
void loop()
{
    
    buttonState = digitalRead(buttonPin);
    DoorState = digitalRead(8);
    
    if (buttonState == HIGH){
      CurrentState = HIGH;
      //SanitizationTime(5);
    }else{
      CurrentState = LOW;
    }

    if(DoorState == LOW){
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(7, LOW);
      digitalWrite(10, HIGH);
      lcd.setCursor(0,1);
      if(SanitiztionProcess == 2){
        IsUnsafe = 1; 
      }
      if((SanitiztionProcess == 2 or SanitiztionProcess == -2) and IsUnsafe == 1){
         IsUnsafe = 1;
         SanitiztionProcess = -2;
         lcd.print("Door opend [Unsafe].................................");
      }else{
        IsUnsafe = 0;
        SanitiztionProcess = -2;
        lcd.print("Door opend.................................");
      }
      String  PushButtonCountMsg = "Cash sanitiz : ";
      PushButtonCountMsg.concat(PushButtonCount);
      lcd.setCursor(0,0);
      lcd.print(PushButtonCountMsg);
    }else if(CurrentState == HIGH and (SanitiztionProcess == 0 or SanitiztionProcess == 2)){
      IsAlarmed = 0;
      digitalWrite(10, LOW);
      digitalWrite(7, LOW);
      lcd.setCursor(0,1);
      //lcd.print("Cash sanitiztion process is running...");
      lcd.print("running....................................");
      digitalWrite(LED_BUILTIN, HIGH);
      //SanitiztionProcess = 1;
      //WaitSanitizationTime(5);
      String  TmpWaitTime = "Wait time sec: ";
      TmpWaitTime.concat((5 + 1) - WaitTimeCount);
      lcd.setCursor(0,0);
      lcd.print(TmpWaitTime);
      delay(1000);
      if(((5 + 1) - WaitTimeCount) <= 1){
        SanitiztionProcess = 1;
      }else{
        SanitiztionProcess = 2;
      }
      WaitTimeCount++;
    }else if(CurrentState == HIGH and SanitiztionProcess == 1){
      WaitTimeCount = 0;
      digitalWrite(10, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      lcd.setCursor(0,1);
      //lcd.print("Cash sanitizeration process completed");
      lcd.print("completed..................................");
      digitalWrite(LED_BUILTIN, LOW);
      if(IsAlarmed == 0){
        PushButtonCount++;
        digitalWrite(7, HIGH);
        WaitAlarmTime(2);
        digitalWrite(7, LOW);
        IsAlarmed = 1;
      }
    }else{
      WaitTimeCount = 0;
      digitalWrite(10, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(7, LOW);
      lcd.setCursor(0,1);
      //lcd.print("Welcome in cash sanitization machine...");
      if(CurrentState == HIGH and (SanitiztionProcess == -1 or SanitiztionProcess == -2)){
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(10, LOW);
        digitalWrite(7, LOW);
        lcd.print("Do ready...................................");
      }else if(CurrentState == LOW and (SanitiztionProcess == -1 or SanitiztionProcess == -2)){
         SanitiztionProcess = 0;
      }else if(SanitiztionProcess == 0 or SanitiztionProcess == 1){
        SanitiztionProcess = 0;
        IsUnsafe = 0; 
        lcd.print("Ready...................................");
      }else{
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(10, HIGH);
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

void WaitSanitizationTime(int timesec){
  for(int i = 0; i <= timesec; i++){
    String  WaitTime = "Wait time sec: ";
    WaitTime.concat((timesec + 1) - i);
    lcd.setCursor(0,0);
    lcd.print(WaitTime);
    delay(1000);
  }
}

void WaitAlarmTime(int timesec){
  for(int i = 0; i <= timesec; i++){
    String  WaitTime = "Alarming for : ";
    WaitTime.concat((timesec + 1) - i);
    lcd.setCursor(0,0);
    lcd.print(WaitTime);
    delay(1000);
  }
}
