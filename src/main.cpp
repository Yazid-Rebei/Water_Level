#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define braudrate 115200
#define Trig 5
#define echo 8
#define Empty_led 24
#define plein_led 25
#define meduim_led 26

LiquidCrystal_I2C lcd(0x27, 16, 2);
const float hauteur_boite = 30; // en cm  

char* alert(float level){
  if(level <=5){
    digitalWrite(Empty_led,LOW);
    digitalWrite(plein_led,HIGH);
    digitalWrite(meduim_led,LOW);
    return "Plein !!";
  }
  else if (level >= 25)
  {
    digitalWrite(Empty_led,HIGH);
    digitalWrite(plein_led,LOW);
    digitalWrite(meduim_led,LOW);
    return "Vide";

  }
  else{
    digitalWrite(Empty_led,LOW);
    digitalWrite(plein_led,LOW);
    digitalWrite(meduim_led,LOW);
    return " ";
  }  
}

void trigger(){
  digitalWrite(Trig,LOW);
  delayMicroseconds(5);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  
}
long read_time(){
  long time=pulseIn(echo,HIGH,30000);
  return time;
}


float calc_distacne(){
  trigger();
  long time = read_time();
  if(time ==0 ){
    return -1;
  }
  return (time/2) * 0.0343;
}
float calc_level(float distance){
  return hauteur_boite - distance;
}

void waterLevel(){
  float distance = calc_distacne();
  if(distance <=-1){
    lcd.setCursor(0,0);
    lcd.print("ERREUR CAPTEUR !");
    return;
  }

  float level = calc_level(distance);
  char* etat = alert(level);
  lcd.setCursor(0,0);
  lcd.print("Level:");
  lcd.print(level);
  lcd.print("cm  ");
  lcd.setCursor(0,1);
  lcd.print("Etat: ");
  lcd.print(etat);
  lcd.print("   ");
}



void setup(){
  pinMode(echo,INPUT);
  pinMode(Trig,OUTPUT);
  pinMode(Empty_led,OUTPUT);
  pinMode(plein_led,OUTPUT);
  pinMode(meduim_led,OUTPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(braudrate);
}

void loop(){
  waterLevel();
  delay(500);
}