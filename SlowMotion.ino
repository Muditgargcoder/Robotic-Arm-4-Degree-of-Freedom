#include <Servo.h>
#include <math.h>

Servo s1; // for theta
Servo s2; // for a
Servo s3; // for b
Servo s4; // UpGripper
Servo s5; // Gripper
float r1 = 0;
float a = 0;
float b = 0;
float z = 0;
float r = 100;
float theta = 0;
float rot = 0;
float theta1 = 0;
float theta2 = 0;
float rotdeg = 0;
int count = 0; // counter for pickup and drop

int pos[] = {0, 0, 0};
float servoAngle = 60;//Some constant

void setup() {
  s1.attach(9);//left (for b)
  s2.attach(10);//middle
  s3.attach(11);//right (for a)
  s4.attach(12);//UpGripper
  s5.attach(13); // Gripper // Range 130 --- 130

  s1.write(110); // 0 ahead
  s2.write(90);
  s3.write(20); // 0 behind
  s4.write(0);
  s5.write(162); // range 130 -- 165

  Serial.begin(9600);

  // put your setup code here, to run once:

}

void loop() {
  Serial.println("Enter cylindrical coordinates1:");
  Serial.println("Enter radius");

  while (Serial.available() == 0) {

  }
  r = Serial.parseFloat();
  
  if(r == 0){
    s1.write(110); // 0 ahead
    s2.write(90);
    s3.write(20); // 0 behind
    s4.write(0);
    delay(1000);
    s5.write(162);
    count++;
    return ;
  }
  //
  Serial.println("Enter angle:");
  while (Serial.available() == 0) {

  }
  theta = Serial.parseFloat();
  Serial.println("Enter height");
  while (Serial.available() == 0) {

  }
   z = Serial.parseFloat();
  Serial.println("Enter the rotation angle of gripper:");
  while (Serial.available() == 0) {

  }
  rot = Serial.parseFloat();

  float n = 148;
  float l = 160;
  Serial.println(r);
  if(r<r1){
    moveTo(r-50,theta,70);
  }
  moveTo(r,theta,70);
  r1 = r;

  
  if(count % 2 == 0){
    s4.write(rot);
    pickUp(r,theta,70);
  }else{
    drop();
  }
  count++;
  
}

void moveTo(float r,float theta,float z){
  //r = l1cos(b) + l2cos(a)
  //z = l1sin*(b) - l2sin*(a)
  if(r < 110){
    r = r*1.2 + 10;
  }else if(r < 160){ // 142, 176, 34 
    r = (r-110)*(176-142)/50 + 142;
  }else if(r<200) r = r*1.1;
  else r = 1.05*r;

  float t = r;
  float a = 0;
  float b = 0;
  
//  r = 100;
  a = 2 * atan(((296 * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - (3696 * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - 616 * z + (r * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) + (z * z * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696)) / (r * r + 320 * r + z * z + 3696));
  b = 2 * atan((296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864))) / (r * r + 296 * r + z * z - 3696));
  
  float i2 = s2.read(); // middle
  float i1 = s1.read(); // left, 0 ahead // angle b
  float i3 = s3.read(); // right, 0 behind // angle a

  a = (a) * 180 / M_PI;
  b = (b) * 180 / M_PI;

  
  
  float m = max(max(a,b),theta);
  for(int i = 0; i <= m; i++){
    if(i2 < theta && i + i2 <= theta){
      s2.write(i + i2);
    }
    else if(i2 > theta && i2 - i >= theta){
      s2.write(i2 - i);
    }
    if(i1 > b && i1 - i >= b){
      s1.write(i1 - i);
    }
    else if(i1 < b && i + i1 <= b){
      s1.write(i1 + i);
    }
    if(i3 > a && i3 - i >= a){
      s3.write(i3 - i);
    }
    else if(i3 < a && i + i3 <= a){
      s3.write(i3 + i);
    }
    delay(30);
  }
//
//  r = t;
//  a = 2 * atan(((296 * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - (3696 * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) - 616 * z + (r * r * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696) + (z * z * (296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864)))) / (r * r + 296 * r + z * z - 3696)) / (r * r + 320 * r + z * z + 3696));
//  b = 2 * atan((296 * z + sqrt(-(r * r + z * z - 144) * (r * r + z * z - 94864))) / (r * r + 296 * r + z * z - 3696));
//  a = (a) * 180 / M_PI;
//  b = (b) * 180 / M_PI;
//  i2 = s2.read(); // middle
//  i1 = s1.read(); // left, 0 ahead // angle b
//  i3 = s3.read(); // right, 0 behind // angle a
//  m = max(max(a,b),theta);
//  for(int i = 0; i <= m; i++){
//    if(i2 < theta && i + i2 <= theta){
//      s2.write(i + i2);
//    }
//    else if(i2 > theta && i2 - i >= theta){
//      s2.write(i2 - i);
//    }
//    if(i1 > b && i1 - i >= b){
//      s1.write(i1 - i);
//    }
//    else if(i1 < b && i + i1 <= b){
//      s1.write(i1 + i);
//    }
//    if(i3 > a && i3 - i >= a){
//      s3.write(i3 - i);
//    }
//    else if(i3 < a && i + i3 <= a){
//      s3.write(i3 + i);
//    }
//    delay(30);.
//  }
  
}

void pickUp(float r, float theta, float z){
   float a = 0;
   float b = 0;
   for(z = 70; z >= 13; z--){
      a = 2*atan(((296*r*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) - (3696*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) - 616*z + (r*r*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) + (z*z*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696))/(r*r + 320*r + z*z + 3696));
      b = 2*atan((296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864)))/(r*r + 296*r + z*z - 3696));
      theta1 = (a) * 160 / M_PI;
      theta2 = (b) * 160 / M_PI;
      s1.write(theta2);
      s3.write(theta1);
      delay(30);
    }
    delay(500);
    s5.write(128);
    delay(1000);


    
//    z = 70;
//      a = 2*atan(((296*r*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) - (3696*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) - 616*z + (r*r*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) + (z*z*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696))/(r*r + 320*r + z*z + 3696));
//      b = 2*atan((296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864)))/(r*r + 296*r + z*z - 3696));
//      theta1 = (a) * 160 / M_PI;
//      theta2 = (b) * 160 / M_PI;
//      s1.write(theta2);
//      s3.write(theta1);
//      delay(30);

  for(z = 13; z <= 70; z++){
      a = 2*atan(((296*r*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) - (3696*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) - 616*z + (r*r*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696) + (z*z*(296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864))))/(r*r + 296*r + z*z - 3696))/(r*r + 320*r + z*z + 3696));
      b = 2*atan((296*z + sqrt(-(r*r + z*z - 144)*(r*r + z*z - 94864)))/(r*r + 296*r + z*z - 3696));
      theta1 = (a) * 160 / M_PI;
      theta2 = (b) * 160 / M_PI;
      s1.write(theta2);
      s3.write(theta1);
      delay(30);
    }
                                                                                                                                                                                                         
}


void drop(){
  s5.write(162);
}



