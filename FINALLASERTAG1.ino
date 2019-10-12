//this code is for arduino NANO , so change the board to Arduino Nano

#include<IRremote.h>
#include <LiquidCrystal.h>
int rec = 4;
int led = 7;
#define button 12
const int rs = 2, en = 5, d4 = 6, d5 = 7 , d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long int TEMP = 2863311530;

IRsend irsend;
IRrecv IRrec(rec);
decode_results results;
int health = 20;
  int AMMO = 100;

void setup() {
  
Serial.begin(9600);
IRrec.enableIRIn();
pinMode(button,INPUT_PULLUP);
pinMode(LED_BUILTIN,OUTPUT);
//digitalWrite(button,HIGH);
pinMode(led,OUTPUT);
//pinMode(10,OUTPUT);
//analogWrite(10,255);
//lcd.backlight();
}
void loop()
{
  lcd.begin(16,2);
  //lcd.setBacklight(HIGH);
  if(AMMO > 0)
  {
  lcd.print("Ammo:");
  lcd.print(AMMO);
  }
  else
  {
    lcd.setCursor(0,0);
        lcd.print("no ammo left");
  }
  if(health > 0)
  {
  lcd.setCursor(0,1);
  lcd.print("Health:");
  lcd.print(health);
  }
  else
  {
    lcd.setCursor(0,1);
  lcd.print("DEAD");
  }
int a = digitalRead(button); 
 
  if(a == 0)
    {
      if(AMMO != 0)
      {
       
      digitalWrite(LED_BUILTIN,HIGH);
      irsend.sendNEC(0xBBBBBBBB,32);
      Serial.println("in if");
      AMMO--;
      }
      
      Serial.println("sent");
       IRrec.enableIRIn();
    }
  else
    {
      digitalWrite(LED_BUILTIN,LOW);
      Serial.println("in else");
      delay(100);
      if (IRrec.decode(&results))
      {              
        if(results.value == TEMP)
        {//check for infrared signal
      if(health != 0)
      {
      Serial.println(results.value,HEX );
      delay(100);
      //print ir hex code  
      Serial.println("in");
      health--;
      }
      else
      {
  exit(0);
      }
      delay(100);
      //reduce health by 1
     IRrec.resume();
        }
      }
}
}
