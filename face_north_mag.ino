#include <Wire.h> //Wire Librarey for I2C communication 
#include <MechaQMC5883.h> //QMC5883 Librarey is added since mine is QMC583 and not HMC5883

MechaQMC5883 qmc; //Create an object name for the snsor, I have named it as qmc
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //Begin I2C communication 
  Serial.begin(9600); //Begin Serial Communication 
  qmc.init(); //Initialise the QMC5883 Sensor 


}

void loop() {
  // put your main code here, to run repeatedly:

    check_north();
    delay(1000);
}
    
int check_north () {
  int x,y,z;
    qmc.read(&x,&y,&z);
    int heading=atan2(x, y)/0.0174532925;
    //Convert result into 0 to 360
   if(heading < 0)
    heading+=360;
    heading = 360-heading;
    
    
  if (!(heading < 22 or heading > 338))
    {
      Serial.print("Not facing North: ");
      Serial.println(heading);
      delay(500);

      int x,y,z;
      qmc.read(&x,&y,&z);
      int heading=atan2(x, y)/0.0174532925;
  
      //Convert result into 0 to 360
     if(heading < 0)
      heading+=360;
      heading = 360-heading;
      delay(500);
      }

    if (heading > 338 || heading < 22)
    {
      Serial.println("Now North: ");
      Serial.println(heading);
      delay(500);
      
      int x,y,z;
      qmc.read(&x,&y,&z);
      int heading=atan2(x, y)/0.0174532925;
      //Convert result into 0 to 360
     if(heading < 0)
      heading+=360;
      heading = 360-heading;
      delay(500);
      }
  }
