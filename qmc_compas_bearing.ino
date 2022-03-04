/*
 * Program for Arduino Digital Compass using QMC5883
 * Project by: Aswinth Raj
 * Dated: 1-11-2018
 * Website: www.circuitdigest.com
 * Lib. from https://github.com/keepworking/Mecha_QMC5883L
 * WARNING: This code works only for QMC5883 Sensor which is commonly being sold as HMC5883 read article to find the actual name of the sensor you have.
 */

#include <Wire.h> //Wire Librarey for I2C communication 
#include <MechaQMC5883.h> //QMC5883 Librarey is added since mine is QMC583 and not HMC5883

MechaQMC5883 qmc; //Create an object name for the snsor, I have named it as qmc

int ledPins[] = {2,3,4,5,6,7,8,9}; //Array of output pin to which the LED is connected to
char led_count = 7; //Total number of LED pins 

  
void setup() {
  Wire.begin(); //Begin I2C communication 
  Serial.begin(9600); //Begin Serial Communication 
  qmc.init(); //Initialise the QMC5883 Sensor 

  for (int thisPin=0; thisPin <= led_count; thisPin++){ //Navigate through all the pins in array 
    pinMode(ledPins[thisPin],OUTPUT); //Declare them as output 
  }

}

void loop() { //Infinite Loop
  int x,y,z;
  qmc.read(&x,&y,&z); //Get the values of X,Y and Z from sensor 
  
  int heading=atan2(x, y)/0.0174532925; //Calculate the degree using X and Y parameters with this formulae 

 //Convert result into 0 to 360
  if(heading < 0) {
  heading+=360;}
  heading = 360-heading;
  
  Serial.println(heading); //Print the value of heading in degree for debugging 

//Based on the value of heading print the result for debugging and glow the respective LED.
  if (heading > 338 || heading < 22)
  {
    Serial.println("NORTH");
    digitalWrite(ledPins[0],HIGH);
  }
  if (heading > 22 && heading < 68)
  {
    Serial.println("NORTH-EAST");
    digitalWrite(ledPins[7],HIGH);
  }
  if (heading > 68 && heading < 113)
  {
    Serial.println("EAST");
    digitalWrite(ledPins[6],HIGH);
  }
  if (heading > 113 && heading < 158)
  {
    Serial.println("SOUTH-EAST");
    digitalWrite(ledPins[5],HIGH);
  }
  if (heading > 158 && heading < 203)
  {
    Serial.println("SOUTH");
    digitalWrite(ledPins[4],HIGH);
  }
  if (heading > 203 && heading < 248)
  {
    Serial.println("SOTUH-WEST");
    digitalWrite(ledPins[3],HIGH);
  }
  if (heading > 248 && heading < 293)
  {
    Serial.println("WEST");
    digitalWrite(ledPins[2],HIGH);
  }
  if (heading > 293 && heading < 338)
  {
    Serial.println("NORTH-WEST");
    digitalWrite(ledPins[1],HIGH);
  }

  delay(500); // update position of LED for every alf seconds 
//Turn off the all the LED 
    for (int thisPin=0; thisPin <= led_count; thisPin++){
     digitalWrite(ledPins[thisPin],LOW);
  }
}
