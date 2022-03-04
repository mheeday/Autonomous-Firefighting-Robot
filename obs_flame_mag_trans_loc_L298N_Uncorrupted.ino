#include <L298N.h>
#include <RH_ASK.h> // Include RadioHead Amplitude Shift Keying Library
#include <NewPing.h>
#include <Servo.h>
#include <Wire.h> //Wire Librarey for I2C communication 
#include <MechaQMC5883.h> //QMC5883 Librarey is added since mine is QMC583 and not HMC5883

#include <SPI.h> // Include dependant SPI Library

MechaQMC5883 qmc;
RH_ASK rf_driver(2000, 22, 12, 10, false); // Receiver pin at D22 on Mega

//L298N motor control pins
const unsigned int EN_R1 = 8;
const unsigned int INR1 = 9;
const unsigned int INR2 = 10;

const unsigned int INR3 = 11;
const unsigned int INR4 = 12;
const unsigned int EN_R2 = 13;

L298N left_motor(EN_R1, INR1, INR2);
L298N right_motor(EN_R2, INR3, INR4);

//sensor pins
#define trig_pin 31 //Digital input 31
#define echo_pin 33 //Digital input 33

#define maximum_distance 200
boolean goesForward = false;
int distance = 150;

NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function

Servo servo_motor;
Servo water_servo;

int left_flame_pin = A8;
int center_flame_pin = A9;
int right_flame_pin = A10;
int left_flame_value;
int center_flame_value;
int right_flame_value;

int RelayPin = 53;
int left_distance;
int right_distance;

int flame_threshold_value = 500;
int close_to_flame = 100;

int current_location = 0;


void setup() {
  // put your setup code here, to run once:
  left_motor.setSpeed(150);
  right_motor.setSpeed(150);

  servo_motor.attach(50); //our servo pin
  servo_motor.write(99); //Move Servo forward
  delay(1000);

  water_servo.attach(24);
  water_servo.write(110);
  delay(1000);  

  distance = readPing();
  delay(100);

  pinMode(left_flame_pin, INPUT);
  pinMode(center_flame_pin, INPUT);
  pinMode(right_flame_pin, INPUT);

  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, HIGH);

  Wire.begin(); //Begin I2C communication
  Serial.begin(9600); //Begin Serial Communication
  qmc.init(); //Initialise the QMC5883 Sensor
  rf_driver.init(); // Initialize ASK Object
}

void loop() {
  // put your main code here, to run repeatedly
  //------------------------------------------------------------------------------------------
  check_left_for_fire();
  check_right_for_fire();
  Serial.println(check_center_for_fire());
  if (check_center_for_fire() == true) {
      put_off_fire();
      }

  int transmitter_value = check_transmitter();
  
  int counter = 0;
  while (transmitter_value == 5) {
    Serial.println("No reading from Trans, would check again");
    transmitter_value = check_transmitter();
    counter+=1;
    Serial.println(counter);
    Serial.println(transmitter_value);
    delay(random(1,10));
    
    if (counter >= 100) {
      Serial.println("Tried 100 times, no fire boss");
      break;
      }
    } 
  Serial.print("Transmitter Value = ");
  Serial.println(transmitter_value);
  
  
  if (transmitter_value > 0 and transmitter_value < 5 ) {
    bool close_to_zero;
    int min_deg, max_deg;
    
    if (transmitter_value == 1) {
      if (current_location == 0 or current_location == 3){
          close_to_zero = false; min_deg = 100; max_deg = 120;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
          }
          
      else if (current_location == 1 ){
          check_left_for_fire();
          check_right_for_fire();
          if (check_center_for_fire() == true) {
            put_off_fire();
            }
        }
      else if (current_location == 2){
          close_to_zero = false; min_deg = 196; max_deg = 210;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
        }
      else if (current_location == 4){
          close_to_zero = false; min_deg = 281; max_deg = 295;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
          
        }
      current_location == 1;
      }  

          
    else if (transmitter_value == 2) {
      if (current_location == 0 or current_location == 4){
          close_to_zero = false; min_deg = 306; max_deg = 320;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
          }
      else if (current_location == 1){
          close_to_zero = true; min_deg = 7; max_deg = 352;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
        }
      else if (current_location == 2){
          check_left_for_fire();
          check_right_for_fire();
          if (check_center_for_fire() == true) {
            put_off_fire();
            }
        }
      else if (current_location == 3){
          close_to_zero = false; min_deg = 277; max_deg = 291;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
        }
    current_location == 2;
    }

    
    else if (transmitter_value == 3) {
      if (current_location == 0 or current_location == 1){
          close_to_zero = false; min_deg = 34; max_deg = 48;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
          }
      else if (current_location == 2){
          close_to_zero = false; min_deg = 80; max_deg = 94;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
        }
      else if (current_location == 3){
          check_left_for_fire();
          check_right_for_fire();
          if (check_center_for_fire() == true) {
            put_off_fire();
            }
        }
      else if (current_location == 4){
          close_to_zero = true; min_deg = 7; max_deg = 353;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
        }
    current_location == 3;
    }

    
else if (transmitter_value == 4) {
      if (current_location == 0 or current_location == 2){
          close_to_zero = false; min_deg = 123; max_deg = 137;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
          }
      else if (current_location == 1){
          close_to_zero = false; min_deg = 79; max_deg = 93;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
        }
      else if (current_location == 3){
          close_to_zero = true; min_deg = 194; max_deg = 208;
          align_direction (close_to_zero, min_deg, max_deg);
          remoteMove(close_to_zero, min_deg, max_deg);
        }
      else if (current_location == 4){
          check_left_for_fire();
          check_right_for_fire();
          if (check_center_for_fire() == true) {
            put_off_fire();
            }
        }
      
    current_location == 4;
    }
    }
//--------------------------------------------------------------------------------------------------------------------
}
void remoteMove(int a, int b, int c){
   
     bool flame_extin = false;

     while (!flame_extin) {
        Serial.println("X");
        if (avoidObstacle()==1) {
          align_direction(a,b,c);
          }
          
        moveForward();
        
        check_left_for_fire();
        Serial.println("Y");
        check_right_for_fire();
        Serial.println("Z");
        Serial.print("Before flame check, Is flame off = ");
        
        Serial.println(bool(flame_extin));
        check_center_for_fire();
        if (check_center_for_fire() == true) {
            moveStop();
            Serial.println("FOUND FIRE");
            put_off_fire();
            flame_extin = true;
            break;
            }
        Serial.println("XX");
        Serial.print("After flame check, Is flame off = ");
        Serial.println(bool(flame_extin));
        
        Serial.println("YY");
        Serial.print("END OF WHILE LOOP, Is flame off = ");
        Serial.println(bool(flame_extin));
      }
  }


void check_left_for_fire(){
  left_flame_value = analogRead(left_flame_pin);
  center_flame_value = analogRead(center_flame_pin);
  Serial.println(left_flame_value);
  
  while (left_flame_value <= flame_threshold_value and left_flame_value < center_flame_value) {
    Serial.println("Fire towards the left");
    left_distance = lookLeft();
    delay(500);
    if (left_distance <= 20) {
      moveStop();
      delay(100);
      moveBackward();
      delay(200);
      moveStop();
      delay(100);
      }
    turnLeft();
    moveStop();
    left_flame_value = analogRead(left_flame_pin);
    Serial.println("Reading left again");
    Serial.println("Reading middle");
    center_flame_value = analogRead(center_flame_pin);
  }
  }


bool check_center_for_fire(){
  left_flame_value = leftFlameSensor();
  center_flame_value = centerFlameSensor();
  Serial.println(center_flame_value);
  right_flame_value = rightFlameSensor();

  if (center_flame_value < flame_threshold_value and center_flame_value < left_flame_value and center_flame_value < right_flame_value) {      //Comparing left, center and right values
    Serial.println("Center flame is the greatest");
    while (center_flame_value > close_to_flame and center_flame_value <= flame_threshold_value) {
      moveForward();
      center_flame_value = analogRead(center_flame_pin);
      Serial.println(center_flame_value);
      }
      moveStop();
      return true;
    }
    Serial.println("Step 5");
    return false;
  }

void put_off_fire () {
  center_flame_value = analogRead(center_flame_pin);
  Serial.println("Before loop: Fire Value: ");
  Serial.println(center_flame_value);
  
  if (center_flame_value <= close_to_flame) {
      moveStop();
      while (center_flame_value <= close_to_flame) {
        Serial.println("Step ZZ");
        Serial.println("Fire in the hole");
        Serial.println("Fire Value: ");
        Serial.println(center_flame_value);
        digitalWrite(RelayPin, LOW);
        Serial.println("Relay ON");
        int pos;
        for (pos = 50; pos <= 170; pos += 1) { 
            // in steps of 1 degree
            water_servo.write(pos);              
            delay(15);               }
          for (pos = 170; pos >= 50; pos -= 1) { 
            water_servo.write(pos);              
            delay(15);               }
        Serial.println("ZZII");
        center_flame_value = analogRead(center_flame_pin);
        }
        
        water_servo.write(99);
        Serial.println("ZZV");
        digitalWrite(RelayPin, HIGH);
        Serial.println("ZZVI");
        delay(5000);
      }
  }

void check_right_for_fire(){
    right_flame_value = analogRead(right_flame_pin);
    center_flame_value = analogRead(center_flame_pin);

    Serial.println(right_flame_value);
    
    while (right_flame_value <= flame_threshold_value and right_flame_value < center_flame_value) {
    Serial.println("Fire towards the right");
    right_distance = lookRight();
    if (right_distance <= 20) {
      moveStop();
      delay(100);
      moveBackward();
      delay(200);
      moveStop();
      delay(100);
       }

    turnRight();
    moveStop();
    right_flame_value = analogRead(right_flame_pin);
    Serial.println("Reading right value again");
    Serial.println("Reading middle");
    center_flame_value = analogRead(center_flame_pin);
  }
  }


int leftFlameSensor(){
  return analogRead(left_flame_pin);
  }

int centerFlameSensor(){
  return analogRead(center_flame_pin);
  }

int rightFlameSensor(){
  return analogRead(right_flame_pin);
  }


int avoidObstacle () {
  int distanceRight = 0;
  int distanceLeft = 0;
  
  distance = readPing();
  Serial.print("Distance ahead is ");
  Serial.println(distance);
  
  if (distance <= 15){ //Check for obstacle in front
    moveStop();
    Serial.println("Obstacle Close by");
    delay(300);
    Serial.println("Moving Backwards");
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    Serial.print("Checking right, distance is ");
    distanceRight = lookRight();
    Serial.println(distanceRight);
    delay(300);
    Serial.print("Checking left");
    distanceLeft = lookLeft();
    Serial.println(distanceLeft);
    delay(300); 

    if (distance >= distanceLeft){
      turnRight();
      delay(60);
      moveStop();
        }
    else{
      turnLeft();
      delay(60);
      moveStop();
        }
    return 1;
   }
   return 2;
  }

int lookRight() {
  servo_motor.write(80);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(99);
  return distance;
}

int lookLeft() {
  servo_motor.write(130);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(99);
  return distance;
  delay(100);
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

void moveStop() {

  left_motor.stop();
  right_motor.stop();
}

void moveForward() {

  //if(!goesForward){

  goesForward = true;

    left_motor.forward();
    right_motor.forward();
    delay(100);



  //}
}

void moveBackward() {

  goesForward = false;

  left_motor.backward();
  right_motor.backward();
  delay(50);

}


void turnRight() {

  left_motor.forward();
  right_motor.backward();
  delay(60);
  moveStop();
}

void turnLeft() {

  left_motor.backward();
  right_motor.forward();
  delay(500);
  moveStop();
}

void turnRightSlowly() {

  left_motor.forward();
  right_motor.backward();

  delay(100);
}

void turnLeftSlowly() {

  left_motor.backward();
  right_motor.forward();

  delay(100);
}

void movement () {
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 20) {
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distance >= distanceLeft) {
      turnRight();
      moveStop();
    }
    else {
      turnLeft();
      moveStop();
    }
  }
  else {
    moveForward();
  }
  distance = readPing();
}

void align_direction (bool close_to_zero, int min_deg, int max_deg) {
  
  bool facing_direction = false;

  int heading = bearing ();

  if (close_to_zero) {
    if (heading < min_deg or heading > max_deg){
        facing_direction = true;
        Serial.println("Facing Direction now");
        }
     while (!facing_direction) {
        Serial.print("Not aligned to direction, currently at:(1) ");
        Serial.println(heading);
        turnRight();
        delay(50);
        moveStop();

        heading = bearing ();
        
        if (heading < min_deg or heading > max_deg){
            facing_direction = true;
          }
      }
    }
    
  else {
    if (heading > min_deg and heading < max_deg){
      facing_direction = true;
      }
      
    while (!facing_direction){
      Serial.print("Not aligned to direction, currently at:(2) ");
      Serial.println(heading);
      turnRight();
      Serial.println("Right");

      heading = bearing ();

      if (heading > min_deg and heading < max_deg){
        facing_direction = true;
        break;
        }
  }
  moveStop();
  }
}

int check_transmitter() {
    uint8_t buf[1]; // Set buffer to size of expected message
    uint8_t buflen = sizeof(buf);
    if (rf_driver.recv(buf, &buflen)) { // Check if received packet is correct size 
      // Message received with valid checksum
      Serial.print("Message Received: ");
      Serial.println((char*)buf);  
        String strBuf = (char*)buf;
        if (strBuf.indexOf("1") != -1 ) {
            return 1;}
        else if (strBuf.indexOf("2") != -1 ) {
            return 2;}
        else if (strBuf.indexOf("3") != -1 ) {
            return 3;}
        else if (strBuf.indexOf("4") != -1 ) {
            return 4;}
      }
     else {
            return 5;}
}

int bearing (){
  int x, y, z;
  qmc.read(&x, &y, &z);
  int heading = atan2(x, y) / 0.0174532925;
  
  //Convert result into 0 to 360
  if (heading < 0)
    heading += 360;
    
  heading = 360 - heading;
  return heading;
  }
