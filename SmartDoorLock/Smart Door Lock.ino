/**********************************************************************************************************
  Program Name: Smart Door Lock
  Group name: CMTP DOGS
  Programers Names: TALHA TALLAT, Marcin Majchzak, Paulius Brickus and Cain Margey
  Decripition: This program allows user to connect to wifi module NodeMCU (ESP8266) to the wifi for controlling any 
               electrincs device using the BLYNK Application on any smart phone or tablets from any where
               in the world.
               
               BLYNK ?!!!!!!!    
               Blynk is a platform with iOS and Android apps to control Arduino, Raspberry Pi and the 
               linkes over the Internet. You can easily build graphic interfaces for all your projects by
               simply dragging and dropping widgets.

               This program lets a user to press a button on their phones to rotate the Servo motor 90-degree 
               clockwise and press button to turn it off then the motor goes back to orignal posistion. 
               This program also reads the value of the Reed switch to give a notification on the phone.
 Note: This program is adopted for certain extent.
 Note: All of the library that are listed below must include for use. might have to download them from github
*********************************************************************************************************/

#define BLYNK_PRINT Serial   // This function allows us to print prograss onto serial monitor

#include <ESP8266WiFi.h>    // Including the libraries for the wifi module ESP8266

#include <BlynkSimpleEsp8266.h>  // Including the library for the wifi module to connect to Blynk app

#include <DNSServer.h> // Including the library for specific DNS servers 

BlynkTimer timer; // Initialising a variable at BlynkTimer funcation

#include <ESP8266WebServer.h> // Including a Library for setting up web server for Esp8266 module

#include <WiFiManager.h> // Must include this library for WIFI use to connect

#include <EEPROM.h> // Stores values read fron analog 0 into the EEPROM. these values stays in the EEPROM when the board id turned off 

#include <Servo.h> // Including this library for Servo motor

Servo myservo; // Create servo object to control a serve

char auth[] = "9bba21ec41cb4d27bfe5827686c57b2d"; /* This is a Auth Token code that we got from by logging in to our BLYNK app account 
and setting up a ESP8266 device to the BlYNK. Another words, this is how we are connecting our phone to that wifi module. */

int flag=0; // Variable to store the Reed switch posistion 

void notifyOnButtonPress() // Made new function for Reed switch 
{
  int isButtonPressed = digitalRead(D1); // digitalRead function reads the value from the pin number D1 and stores at isbuttonPressed
  
  if (isButtonPressed==1 && flag==0) {          // If this variable reads value high and flags reads 0 than do the follwing..
    Serial.println("Someone Opened the door");       // Prints out "Some one opend the door" on the serial monitor 
    Blynk.notify("Alert : Someone Opened the door"); // Prints out on phone using BLYNK app
    flag=1;                                          // Set the varible to high
  }
  
  else if (isButtonPressed==0){  // Or if the varible is low than..
    flag=0;                     // Than reed switch is low
  }
}

void setup() // In setup everything runs only ones
{
  EEPROM.begin(512); // Exact amount of storage needed to save memory and retuen
  
  Serial.begin(9600); //The serial monitor is used for shown the information provided and bot rate is det onto 9600
  
  myservo.attach(4); //Attach the servo variable to a pin
  
  myservo.write(180); // Maximum spin of 180 degrees 
  
  WiFiManager wifiManager; // Try to ask for a ssid and password on phone to connect to wifi 
  
  wifiManager.resetSettings(); //Uncomment this to wipe WiFi settings from EEPROM on boot.  Comment out and recompile/upload after 1 boot cycle.
  
  wifiManager.autoConnect("NodeMCU"); // After starting ESP8366 than it will try to connect with WIFI automatically
 
  Serial.println("connected...:)"); // Prints out in serial Moniter that its connected
  
  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the BUILTIN_LED pin as an output
  
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());

  pinMode(D1,INPUT_PULLUP);  // Setup notification button on pin D1
  
 timer.setInterval(16000L,notifyOnButtonPress);
}

BLYNK_WRITE(V2) // Setting up the pin at V2 for servo 
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V1 to a variable
  
  Serial.print("V1 Slider value is: "); // prints "V1 Slider value is: " on serial monitor the slider value 
  
  Serial.println(pinValue);    // Shows the pin value on serial monitor after checking it 
  
  Blynk.virtualWrite(V0, "Door State"); //  writes "Door State" on lcd widgets on phone using BLYNK application and sets a pin to V0
  
  if(pinValue)  // if pin does something than..
  {
    Blynk.virtualWrite(V1, "Open");  // Prints out open on the lcd widgets
    servo();  
  }
  
  else // else than..
  {
    Blynk.virtualWrite(V1, "Closed"); //Prints out 'Closed' on the lcd widgets
    
    myservo.write(180); // Can allowed maxium of 180 degree rotation of servo
  }
  
}

void loop()  //Everthing in the loop function runs continuosly
{
  Blynk.run();  // Initiates blynk application which runs rapidly 
  
  timer.run(); // Initiates BlynkTimer
}

void servo() // Created new function for servo 
{
  myservo.write(90); // Servo only spin at 90 degree rapidly if state is on, which closes the door.
}



