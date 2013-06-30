/*
 Program Name: Simon Says
 Author: Jeff Murchison
 Contact: jeff@jeffmurchison.com
 Date: October 2012
 Version: 1.0
 License: 
 
 Creative Commons Attribution-ShareAlike 3.0 Unported License.
 http://creativecommons.org/licenses/by-sa/3.0/
 */


const int btn1 = 0;
const int btn2 = 1;
const int btn3 = 2;
const int led1 = 4;
const int led2 = 5;
const int led3 = 6;
const int buzzer = 8;

void setup()
{
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);   // sets the pin as output
}
void loop()
{
  if (digitalRead(btn1) == LOW){
    buttonPress(btn1,led1);
    }
  else if (digitalRead(btn2) == LOW){
    buttonPress(btn2,led2);
    }
  else if (digitalRead(btn3) == LOW){
    buttonPress(btn3,led3);
    }
}

  void buttonPress (int button,int led){
    digitalWrite(led, HIGH);
    analogWrite(buzzer,60);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
  }







