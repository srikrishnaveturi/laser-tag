//GUN 2
#include<IRremote.h>
#include <LiquidCrystal.h>
#include<EEPROM.h>
int i;
int rec = 12;

int buzzer = 2;
int rumble = 11;
int button = 4;
int flag = 0;

const int rs = 10, en = 9, d4 = 8, d5 = 7 , d6 = 6, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int resett = 29986;                //Decimal for Reset Hex

IRsend irsend;
IRrecv IRrec(rec);
decode_results results;

int ammo_addr = 0;          //addr of ammo variable
int health_addr = 1;        //addr of health variable

int health = 20;
int ammo = 90;

void setup() {

  Serial.begin(9600);
  IRrec.enableIRIn();
  pinMode(button, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(button,HIGH);
  pinMode(buzzer, OUTPUT);
  pinMode(rumble, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(rumble, LOW);


  ammo = EEPROM.read(ammo_addr);           //reading previous value from EEPROM
  health = EEPROM.read(health_addr);

  EEPROM.write(ammo_addr, ammo);           //Overwriting previous value
  EEPROM.write(health_addr, health);
  lcd.begin(16, 2);
}
void loop()
{
  digitalWrite(LED_BUILTIN, LOW);
  ammo = EEPROM.read(ammo_addr);           //reading previous value from EEPROM
  health = EEPROM.read(health_addr);
  //Sending IR signal
  int a = digitalRead(button);
  if (a == 0)
  {
    digitalWrite(rumble, HIGH);
    delay(500);

    if (ammo != 0)
    {
      Serial.print("shooting");
      digitalWrite(LED_BUILTIN, HIGH);
      irsend.sendNEC(0xBBBBBBBB, 32);       //Send IR code BBBBBBBB
      ammo--;
      EEPROM.write(ammo_addr, ammo);        //Saving updated ammo
    }
    IRrec.enableIRIn();
  }
  //Receiving IR signal through TSOP
  else
  {
    delay(100);
    if (IRrec.decode(&results))
    {
      Serial.println(results.value, HEX);
      if (results.value == 0xAAAAAAAA || results.value == 0xCDF65D5)
      { //check for infrared signal
        if (health != 0)
        {
          Serial.println(results.value, HEX );         //print ir hex code
          delay(100);
          health--;
          Serial.println(health);
          EEPROM.write(health_addr, health);           //Saving updated health
          if (health == 0 ) {
            lcd.setCursor(0, 1);
            lcd.print("DEAD            ");
            digitalWrite(buzzer, HIGH);
            // exit(0);
          }
        }
        delay(100);
      }
      //Resetting Ammo and Health
      if (results.value == resett) {
        health = 20;
        ammo = 90;
        digitalWrite(buzzer, LOW);
        EEPROM.write(ammo_addr, ammo);
        EEPROM.write(health_addr, health);

        ammo = EEPROM.read(ammo_addr);
        health = EEPROM.read(health_addr);
        IRrec.resume();
      }
      IRrec.resume();
    }
  }

  //Printing values on LCD
  if (ammo > 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("AMMO:");
    lcd.print(ammo);
    lcd.print("         ");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("NO AMMO LEFT       ");
    if (flag == 0)
    {
      for (i = 0; i < 5; i++)
      {
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
        delay(500);
      }
      flag = 1;
    }

  }
  if (health > 0)
  {
    lcd.setCursor(0, 1);
    lcd.print("HEALTH:");
    lcd.print(health);
    lcd.print("         ");
  }
  digitalWrite(rumble, LOW);
}
