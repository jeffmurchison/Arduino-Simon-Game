/*
 Application Name: Simon Says Game Kit
 Description: A simple game of Simon Says.
 Author: Jeff Murchison
 Contact: jeff@jeffmurchison.com
 Date: November 19, 2012
 Version: 1.0.1
 License: 
 
 This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 http://creativecommons.org/licenses/by-nc-sa/3.0/
 
 To purchase a kit, please visit:
 http://jeffmurchison.com/blog/simon-says-game-kit/ 
 
 Notes:
 
 This code uses cores from the arduino-tiny project, and is designed for use on an ATtiny84.
 For more information and to download the cores, visit the arduino-tiny Google Code page:
 http://code.google.com/p/arduino-tiny/
 
 
 Changelog
 
 Version 1.0.1:
 
 - Added support for ATtiny44A (reduced maxturns to 60, 100 was eating too much memory)
 - Added an option to disable the buzzer tone on startup (feature request from my girlfriend who got annoyed with all of my testing)
 
 */


#include <EEPROM.h>   // We're going to be writing some stuff to the microcontroller's EEPROM

int buzzerPin = 2;  // The buzzer pin
int buttonPin[] = {
  0, 7, 8, 9, 10}; // The four button input pins. We're not using the first one so as to avoid having a "button #0"
int ledPin[] = {
  0, 3, 4, 5, 6};  // LED pins (
int buzzerTone[] = {
  50, 200, 400, 600, 800};  // Buzzer Tones for the different buttons (fail noise, 1, 2, 3, 4)

int turnCount = 1;  // Turn counter
int buttonState = 0;  // Button state checker

const int maxTurns = 60; // The maximum number of random numbers that will be generated.
byte randomArray[maxTurns]; // Array that will store the random numbers for the game.
byte inputArray[maxTurns];  // Array that will store user input.

int eepromValue = 0;  // A temporary value that will hold the EEPROM information after it has been read from the EEPROM.
                      // Saves us from reading the EEPROM multiple times per turn.

boolean playedThisTurn = false;  // Boolean variable for if the sequence has been played already.
boolean buttonPressed = false; // Boolean variable for f a button has been pressed.



void setup()
{
  //Serial.begin(9600);  // Serial is commented out as the ATtiny doesn't support it. Useful for debugging on an ATmega328.

  pinMode(buzzerPin, OUTPUT);  // Set the buzzer to output.

  for(int x=1; x <= 4; x++) {  // Set pinmodes for button, LED pins, set LEDs HIGH (off)
    pinMode(ledPin[x], OUTPUT);
    digitalWrite(ledPin[x], HIGH);
    pinMode(buttonPin[x], INPUT);
  }

  if (digitalRead(buttonPin[4]) == HIGH) { // Checking for button push on startup
    buzzerPin = 20;                        // If button pressed, we change the buzzer pin to 
  }                                        // an arbitrary nonexistant pin to disable it.
  //Serial.print("---------");
  //Serial.println("");
  randomArrayGen();
  //Serial.print("Random Array Generated.");
}


void loop() 
{ 
  eepromValue = EEPROM.read(0);
  if ((eepromValue + turnCount) >= 255) {   // EEPROM bytes can only hold a value from 0 to 255. This ensures we don't overflow.
    EEPROM.write(0, turnCount);
  }
  else {
    EEPROM.write(0, (turnCount + eepromValue)); // Add the turncount to EEPROM value 0. We use this for random seed.
  }
  if (playedThisTurn == false) {  // If the player hasn't pushed a button this turn yet, then..
    //Serial.println("");
    //Serial.println("");
    //Serial.print("Turn:");
    //Serial.print(turnCount);
    //Serial.println("");
    //Serial.println("Playing Array");
    //Serial.println("---------");
    playedThisTurn = true;  

    for (int x=1; x <= turnCount; x++) {  // Play all of the moves in the random array at the beginning of each turn
      LEDandTone(ledPin[randomArray[x]], buzzerTone[randomArray[x]]);
      //Serial.println(randomArray[x]);
    }
    //Serial.println("---------");
  }

  for (int z=1; z <= turnCount; z++){
    //Serial.println("");
    //Serial.println("");
    //Serial.print("inputNumber: ");
    //Serial.print(z);
    buttonCheck(z);
    buttonPressed = false; 
  }
  turnCount++;
  delay(500);
}


void randomArrayGen ()  // Function for generating the random array
{
  randomSeed(EEPROM.read(0));
  for (int x=1; x < maxTurns; x++) {
    randomArray[x] = random(1, 5); 
    //Serial.println(randomArray[x]);
  } 
  for (int t=600; t <= 1000; t=t+7){
    tone(buzzerPin, t);
    delay(20);
    noTone(buzzerPin); 
    delay(10);
  }
  delay(500);
}

void LEDandTone(int ledPin2, int buzzerTone2) 
{
  tone(buzzerPin, buzzerTone2);  
  digitalWrite(ledPin2, LOW);
  delay(400);
  digitalWrite(ledPin2, HIGH);
  noTone(buzzerPin);
  delay(100);
}


void buttonCheck(int inputNumber)  //Function for allowing user input and checking input against the generated array
{
  while (buttonPressed == false){  // As long as a button has not been pressed this turn, keep checking button inputs
    for(int y=1; y <= 4; y++) {
      buttonState = digitalRead(buttonPin[y]);
      if (buttonState == HIGH) { //Checking for button push

        //Serial.println("");
        //Serial.print("Button Pressed: ");
        //Serial.print(y);
        //Serial.println("");

        LEDandTone(ledPin[y], buzzerTone[y]);
        inputArray[inputNumber] = y;

        playedThisTurn = false;
        buttonPressed = true;
        y = 5;

        if (inputArray[inputNumber] == randomArray[inputNumber]) { //Checks value input by user and checks it against
          //Serial.print("CORRECT!");
          //Serial.println("");                 
        }                                    
        else {
          //Serial.print("INCORRECT");
          //Serial.println("");
          //Serial.println("");
          //Serial.println("------- DEBUG INFO -------");
          //Serial.println("");
          //Serial.print("inputArray[inputNumber]: ");
          //Serial.print(inputArray[inputNumber]);
          //Serial.println("");
          //Serial.print("randomArray[inputNumber]: ");
          //Serial.print(randomArray[inputNumber]);
          //Serial.println("");
          //Serial.print("[inputNumber]: ");
          //Serial.print(inputNumber);
          //Serial.println("");
          //Serial.print("turnCount: ");
          //Serial.print(turnCount);
          //Serial.println("");
          fail();  
        }
      }
      /*if (buttonPressed == true){
       Serial.println("Exiting buttonState loop.");
       }*/
    }
    /*if (buttonPressed == true){
     //Serial.println("Exiting for loop.");
     }*/
  }
  //Serial.print("Exiting while loop, inputNumber should increment after this");
  delay(200);
}


void fail()  //Function used if the player fails to match the sequence
{
  tone(buzzerPin, 50);
  for (int x=1; x <= 4; x++){
    digitalWrite(ledPin[x], LOW);
  }
  delay(2000);
  //EEPROM.write(0, turnCount+(EEPROM.read(0)));
  turnCount = 0; //Resets turn value so the game starts over without need for a reset button
  for (int x=1; x <= 4; x++){
    digitalWrite(ledPin[x], HIGH);
  }
  noTone(buzzerPin);
  delay(1000);
  randomArrayGen();
}



