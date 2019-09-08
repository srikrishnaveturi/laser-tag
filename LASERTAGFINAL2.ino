#include<IRremote.h>
#include <LiquidCrystal.h>
int rec = 4;
int led = 7;
#define button 12
const int rs = 2, en = 5, d4 = 6, d5 = 7 , d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


IRsend irsend;
IRrecv IRrec(rec);
long int temp = 3149642683;
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
 
}
void loop()
{
   lcd.begin(16,2);
  if(AMMO>0)
  {
 
   lcd.print("Ammo:");
  lcd.print(AMMO);
  }
  lcd.setCursor(0,1);
  if(health>0)
  {
  lcd.print("Health:");
  lcd.print(health);
  }
   if(AMMO==0)
      {
       
        lcd.setCursor(0,0);
        lcd.print("Ammo over");
       
      }
       if(health==0)
      {
       
        lcd.setCursor(0,0
        );
        lcd.print("DEAD");
        exit(0);
       
      }
 
 
 
int a = digitalRead(button); 
 
  if(a == 0)
    {
     
      if(AMMO!=0)
      {
      digitalWrite(LED_BUILTIN,HIGH);
      irsend.sendNEC(0xAAAAAAAA,32);
      Serial.println("in if");
      AMMO--;
      
     
    
    
      Serial.println("sent");
       IRrec.enableIRIn();
      }
    
    }
  else
    {
      digitalWrite(LED_BUILTIN,LOW);
      Serial.println("in else");
      delay(100);
      if (IRrec.decode(&results))
      { 
      if(results.value==temp)
      {
      Serial.println(results.value,HEX );
      delay(100);
      //print ir hex code  
      Serial.println("in");
      health--;
     
      }
      if(health==0)
      {
       
        lcd.setCursor(0,1);
        lcd.print("DEAD");
        exit(0);
       
      }
     
      delay(100);
      //reduce health by 1
     IRrec.resume();
    
      }
}
}
