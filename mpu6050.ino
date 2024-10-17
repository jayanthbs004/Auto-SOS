#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
 
#define FONA_RX            2
#define FONA_TX            3
#define FONA_RST           4
 
#define FONA_RI_INTERRUPT  0
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
 
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);


char PHONE_1[21] = "6364712698"; // Enter your Number here.

 
char theftalertmessage[141]= "Accident Detected"  ;

int pirsensor = 0; 
Adafruit_MPU6050 mpu;

void setup(void) {
  //Serial.begin(115200);
  pinMode(8,INPUT);
  Serial.begin(9600);
  Serial.println(F("Initializing....(May take 3 seconds)"));
  Serial.println(F("Initializing....(May take 12 seconds)"));
  delay(5000);
  fonaSS.begin(9600); // if you're using software serial
  if (! fona.begin(fonaSS)) {           // can also try fona.begin(Serial1) 
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

   fona.print ("AT+CSMP=17,167,0,0\r");
    Serial.println(F("FONA is OK")); 

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

void loop() {
sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 float x_axis = a.acceleration.x;
  //sprintf(str, "%d %d %d", x, y, z);
   int pirsensor = digitalRead(8);
  Serial.print("Sensor Value:");
  Serial.println(pirsensor);
  if(pirsensor==1)
  {     
     Serial.println("accident ");
    make_multi_call();
    send_multi_sms();

  }

  else if( x_axis >1 && x_axis < 10 ){
    Serial.println("accident");
    send_multi_sms();
  
  }
  else  
  { 
    pirsensor = 0;
    Serial.println("Safe");
  }
  /* Get new sensor events with the readings */
  

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
}


void make_multi_call()
{
  if(PHONE_1 != ""){
    Serial.print("Phone 1: ");
    make_call(PHONE_1);
    delay(5000);
  } 
}

void make_call(String phone)
{
    Serial.println("calling....");
    fona.println("ATD"+phone+";");
    delay(20000); //20 sec delay
    fona.println("ATH");
    delay(1000);
}
void send_multi_sms()
{
  if(PHONE_1 != ""){
    Serial.print("Phone 1: ");
    fona.sendSMS(PHONE_1,theftalertmessage);
    delay(20000);
  } 
}
