#define DIR 40
#define STEP 41
#define VibMotor 47
#define button1 20  //-
#define button2 21  //+
#define button3 22 //ok/start
#define startDelay 2000 // start up delay
#define swDelay 100  //switch contacts delay
#define mDelay 15  // motor delay
#define OneDoseWeight 5 //weight of one dose
#define serialSpeed 100000  // baud
#define scaleGain 896.f 
#define steps 50 //quantify of steps between scale readings
#define qty 5 // average of n measurements

#include<LiquidCrystal.h>
#include "HX711.h"

int counter = 0;
int feedValue =0;

LiquidCrystal Lcd(12, 11, 5, 4, 3, 2);
HX711 Scale(A0, A1);

void myPrint(int pos, int line, String txt, int timeDelay)
{
 Lcd.setCursor(pos, line);
 Lcd.print(txt);
 delay(timeDelay);
 return 0; 
}

bool ifButtonPressed(int n) {
  if (digitalRead(n) == true) {
    delay(swDelay);
    if (digitalRead(n) == true)
    {
      return true;
    }
  }
  return false;
}


void setup() 
{
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  digitalWrite(DIR, HIGH);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(VibMotor, OUTPUT);
  digitalWrite(VibMotor, HIGH);
  Serial.begin(serialSpeed);
  Scale.set_scale(scaleGain);
  Lcd.begin(16, 2);
  myPrint(0,0,"MILKING MACHINE",0); 
  myPrint(0, 1,"     START!     ", startDelay); 
}

void loop()
{

while(ifButtonPressed(button3)==HIGH)
{
  myPrint(0,0,"Ilosc Miarek: ",0);
  myPrint(0,1,"  -     +     OK",0);
  myPrint(14, 0, String(counter),0);
  if (ifButtonPressed(button1)) { counter=counter+1;}
  if (ifButtonPressed(button2)) {counter=counter-1;}
}
feedValue=counter*OneDoseWeight;
Lcd.clear();
myPrint(2,0,"Waga=0   g",0);
Scale.tare(); 
while ((int(Scale.get_units(qty)) < feedValue))
    {
    digitalWrite(VibMotor, LOW);
    for (int h=0; h<steps;h++)
        {
        digitalWrite(STEP, 1);
        delay(mDelay);
        digitalWrite(STEP, 0);
        delay(mDelay);
        }
    myPrint(7,0,String(int(Scale.get_units(qty))),0);   
    }

digitalWrite(VibMotor, HIGH);
while(ifButtonPressed(button3)==HIGH)
    {  myPrint(0,1,"     GOTOWE   OK",0);
    }
delay(200);
}
