#include<Servo.h>
#include<DHT.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
Servo servo;
LiquidCrystal_I2C lcd(0x27,16,2);

int buzzer=8;
int fan1= 9, in1=3,in2=2,ena=11;
int gasValue,motion,flag=0;
int led = 7;
int mot=6; //motion sensor-
int DHTPIN=5;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
void setup() 
{
  // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(buzzer,OUTPUT);
    pinMode(fan1,OUTPUT);
    servo.attach(10);
    pinMode(led,OUTPUT);
    pinMode(mot,INPUT);
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT);
    pinMode(ena,OUTPUT);
    dht.begin();
    lcd.init();
    lcd.backlight();
}

void fan(int sp)
{
    analogWrite(11,sp);
    digitalWrite(3,LOW);
    digitalWrite(2,HIGH);
    delay(3000);
    digitalWrite(2,LOW);
}

void displayTem(int t,int h)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(String("Temperture : ") + String(t));
    lcd.setCursor(0,1);
    lcd.print(String("Humidity : ") + String(h));
}

void loop()
 {
  // put your main code here, to run repeatedly:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Smart Detection");
    lcd.setCursor(6,1);
    lcd.print("House");
    gasValue = analogRead(A0);
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    motion = digitalRead(mot);
        //If condition for gas sensor
    Serial.print(gasValue);
    Serial.print("\n");
    Serial.print(motion);
    Serial.print("\n");
    if(gasValue<600)
    {
      if(flag==0)
      {
        digitalWrite(buzzer,LOW);
        digitalWrite(fan1,LOW);
        servo.write(5);
        flag=1;
      }
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Gas leaked");
      digitalWrite(buzzer,HIGH);
      digitalWrite(fan1,HIGH);
      servo.write(90);
      flag=0;
      delay(5000);
    }
        // If condition for motion sensor
    if(motion==1)
    {
      digitalWrite(led,LOW);
      displayTem(t,h);
      if(t>=33)
      {
        fan(255);
      }
      else if(t>=30 && t<33)
      {
        fan(150);
      }
      else
      {
        fan(75);
      }
    }
    else
    {
      digitalWrite(led,HIGH);
    }
    delay(2000);
}