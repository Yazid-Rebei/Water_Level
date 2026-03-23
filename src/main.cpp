#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define Trig 5
#define echo 16

#define Empty_led 25
#define plein_led 26

LiquidCrystal_I2C lcd(0x27, 16, 2);
const float hauteur_boite = 14.0;

const char* alert(float level){
  if(level >= 10.0){  
    digitalWrite(plein_led,HIGH);
    digitalWrite(Empty_led,LOW);
    return "Plein";
  }
  else if(level <= 0.5){
    digitalWrite(Empty_led,HIGH);
    digitalWrite(plein_led,LOW);
    return "Vide";
  }
  else if(level >=4.5 && level <= 5.5){
    digitalWrite(Empty_led,LOW);
    digitalWrite(plein_led,LOW);
    return "Moyen";
  }
  else{
    return " ";
  }
}

void trigger(){
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
}


long read_time(){
  return pulseIn(echo, HIGH, 30000);
}


float calc_distance(){
  trigger();
  long time = read_time();

  if(time == 0){
    return -1;
  }

  return (time * 0.0343) / 2.0;
}


float calc_level(float distance){
  return hauteur_boite - distance;
}


void waterLevel(){

  float distance = calc_distance();
  if(distance < 0){
    lcd.setCursor(0,0);
    lcd.print("ERREUR CAPTEUR ");
    return;
  }

  float level = calc_level(distance);
  const char* etat = alert(level);

  lcd.setCursor(0,0);
  lcd.print("Level:");
  lcd.print((int)level);
  lcd.print("cm   ");

  lcd.setCursor(0,1);
  lcd.print("Etat:");
  lcd.print(etat);
  lcd.print("   ");
}

void setup(){
  pinMode(echo,INPUT);
  pinMode(Trig,OUTPUT);

  pinMode(Empty_led,OUTPUT);
  pinMode(plein_led,OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("Begin");
  Serial.begin(9600);
}

void loop(){
  waterLevel();
  delay(500);
}
