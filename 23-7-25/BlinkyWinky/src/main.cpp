/*
This top block comment should describe what program is doing.
Put relevant information here like date, version, brief notes, etc.
Anything that could help you remember or help someone learn what you're doing
*/

#include <Arduino.h>

//start with global constants.
//GlobalConstants are capitalized CamelCase naming - every word is captilized but no spaces
//MACROS use ALL_CAPS_NAMING - every lettter is capital and spacing is done with underscore
//Libraries traditionally use MACROS for naming but you may also use global constants
//The difference is that a compiler may not optimize global constants so will use memory to store them.
//However, global constants are easier to troubleshoot if you are doing more than just naming.

const uint16_t BlinkDelay = 1000; //Long delay 
const uint16_t BlinkDelayShort = 250; //Short delay
//the use of global constants like the one above will allow you to adjust parameters easily on your robot

//next you can put in any global variables. You will use global variables to store the state of your robot
//across all functions in your sketch/program.
uint8_t ledState = 0; //notice the first word is lowercase then camelCase indicating its value may vary.

//then declare any special functions you may be defining. Arduino doesn't require you do this
//but it is necessary to do so when using the Arduino core plus it makes your code more understandble
//as your delcarations announce what things your program will do

void blinkBuiltinLed();

//this defines the setup fucntion;it is called only once by the core
void setup()
{
  pinMode(9, OUTPUT);
}

//this defines the loop function; it is called in an infinite loop by the core
void loop()
{
  blinkBuiltinLed(); //function call
}

//write your custom funciton definitions at the bottom. These are unlikely to change once they're tested and perfected.
void blinkBuiltinLed(){
  digitalWrite(9, HIGH);
  ledState = 1; //this is redundant since the state will always be back to 0 when function returns (unless it is used in an interrupt service routine!), just an example of using a global variable.
  delay(BlinkDelay);
  digitalWrite(9, LOW);
  ledState = 0;
  delay(BlinkDelayShort);
}