#include <Servo.h>
#include <math.h>

Servo s1; // Left Side Motor
Servo s2; // Uarm Base Motor
Servo s3; // Right Side Motor
Servo s4; // Gripper Rotator
Servo s5; // Gripping Motor

// Cylindrical Coordinates for the gripper with Uarm base as a shifted origin.
float r = 100; // radial distance of object from base
float theta = 0; // Rotation of the Uarm base
float z = 0; // height of the gripper from the ground
float rot = 0; // Rotation angle for the gripper

// output angles to the motors
float theta1 = 0; // Output angle to servo s3
float theta2 = 0; // Output angle to servo s1

// Uarm dimension Constants (Given in the pdf), Will be used for computing
float n = 148;
float l = 160;

void setup() {
  s1.attach(9); // left
  s2.attach(10); // middle
  s3.attach(11); // right
  s4.attach(12); // Gripper Rotator
  s5.attach(13); // Gripper // Range 130 --- 165 , (You need to set it manually)

  // Moving to default Configuration initially
  s1.write(110);
  s2.write(90);
  s3.write(20);
  s4.write(0);
  s5.write(162); // range 130(Open) -- 165(Close),, Gripper is closed initially

  Serial.begin(9600);
}

void loop() {


  // The Cylindrical Coordinates will be taken as Input through Serial.

  Serial.println("Enter cylindrical coordinates1:");
  Serial.println("Enter radius");

  while (Serial.available() == 0) {

  }
  // Radial distance of object from base
  r = Serial.parseFloat();
  Serial.println(r);

  // Rotation of the Uarm base
  Serial.println("Enter angle:");
  while (Serial.available() == 0) {

  }
  theta = Serial.parseFloat();
  Serial.println(theta);

  // Height of the Gripper from the base
  Serial.println("Enter height");
  while (Serial.available() == 0) {

  }
  z = Serial.parseFloat();
  Serial.println(z);

  // Gripper Rotation Angle
  Serial.println("Enter the rotation angle of gripper:");
  while (Serial.available() == 0) {

  }
  rot = Serial.parseFloat();
  Serial.println(rot);


  // Move to Function. The gripper can go to any cylindrical coordinate.
  moveTo(r, theta, z);


}

void moveTo(float r, float theta, float z) {

  // a and b are the output angles given to the motors
  float a = 0; // for left motor
  float b = 0; // for right motor


  // By geomatry (given in the pdf), we have these two relations
  //r = l1cos(b) + l2cos(a)
  //z = l1sin*(b) - l2sin*(a)
  // On solving these relations (We did it through matlab, It can be done manually also), we get the solutions:
  a = 2 * atan(((296 * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - (3696 * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - 616 * z + (r * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) + (z * z * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696)) / (r * r + 320 * r + z * z + 3696));
  b = 2 * atan((296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864))) / (r * r + 296 * r + z * z - 3696));

  // Converting radian into degrees
  a = (a) * 180 / M_PI;
  b = (b) * 180 / M_PI;

  float i2 = s2.read(); // Base motor
  float i1 = s1.read(); // left side motor
  float i3 = s3.read(); // right side motor


  // Code for slowly moving the gripper to the required postion. The delay time can be used to control the speed of motors(but it will increase current in wire and motor, so take care of that)
  float m = max(max(a, b), theta);
  for (int i = 0; i <= m; i++) {
    if (i2 < theta && i + i2 <= theta) {
      s2.write(i + i2);
    }
    else if (i2 > theta && i2 - i >= theta) {
      s2.write(i2 - i);
    }
    if (i1 > b && i1 - i >= b) {
      s1.write(i1 - i);
    }
    else if (i1 < b && i + i1 <= b) {
      s1.write(i1 + i);
    }
    if (i3 > a && i3 - i >= a) {
      s3.write(i3 - i);
    }
    else if (i3 < a && i + i3 <= a) {
      s3.write(i3 + i);
    }
    delay(30);
  }

}



void pickUp(float r, float theta, float z) {
  float a = 0;
  float b = 0;

  // For my Uarm, Ground was configured at z = 13.
  // Here, the gripper will go to z = 70 and then go down to ground at z = 13.
  for (z = 70; z >= 13; z--) {
    a = 2 * atan(((296 * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - (3696 * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - 616 * z + (r * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) + (z * z * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696)) / (r * r + 320 * r + z * z + 3696));
    b = 2 * atan((296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864))) / (r * r + 296 * r + z * z - 3696));
    theta1 = (a) * 160 / M_PI;
    theta2 = (b) * 160 / M_PI;
    s1.write(theta2);
    s3.write(theta1);
    delay(30);
  }
  delay(500);

  // Close the gripper now
  s5.write(128);

  delay(1000);

  //Now, the gripper moves upward(with closed gripper) to z = 70.
  for (z = 13; z <= 70; z++) {
    a = 2 * atan(((296 * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - (3696 * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - 616 * z + (r * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) + (z * z * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696)) / (r * r + 320 * r + z * z + 3696));
    b = 2 * atan((296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864))) / (r * r + 296 * r + z * z - 3696));
    theta1 = (a) * 160 / M_PI;
    theta2 = (b) * 160 / M_PI;
    s1.write(theta2);
    s3.write(theta1);
    delay(30);
  }

}

void originalPos() { // Returns the Uarm to Original/ Default Configuration
  s1.write(110);
  s2.write(90);
  s3.write(20);
  s4.write(0);
  delay(1000);
  s5.write(162);

}


// For Droping the object
void drop() {
  // Open the Gripper
  s5.write(162);
}



