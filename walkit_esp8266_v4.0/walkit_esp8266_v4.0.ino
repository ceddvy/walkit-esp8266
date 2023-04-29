// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#define trigPin D2
#define echoPin D1

#include "BlynkEdgent.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D5, D6); // Rx, Tx pins

BlynkTimer timer; 
int control;
int setDistance;
int value = 0;
int val;
int mode;


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  
  BlynkEdgent.begin();
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, Notif);
    
}

void loop() {
  BlynkEdgent.run();  
  timer.run(); 
  
}

BLYNK_CONNECTED()
{
  mode = 1;
  int slider = 30;
  Blynk.virtualWrite(V1, mode);
  Blynk.virtualWrite(V5, slider);
}

BLYNK_WRITE(V1){
  int getMode = param.asInt();

  if(getMode == 1){
    mode = 1; //mode is 1 which is on
  }else{
    mode = 3; //mode is 2 which is off
  }
}


//Forward
BLYNK_WRITE(V6){
  // int setValue = param.asInt();
  int getForward = param.asInt();
  if (getForward == 6){
    
    control = getForward;
  }  

}
//Backward
BLYNK_WRITE(V7){
  int getBack = param.asInt();
  if (getBack == 7){
    control = getBack;
  } 

}
//LEFT
BLYNK_WRITE(V8){
  int getLeft = param.asInt();
  if (getLeft == 8){
    control = getLeft;
  }

}
//Right
BLYNK_WRITE(V9){
  int getRight = param.asInt();
  if (getRight == 9){
    control = getRight;
  }

}

BLYNK_WRITE(V5){
  int getDistance = param.asInt();
  if (getDistance > 10){
    setDistance = getDistance;
  }

}

void sendSensor(){
  
  if(control != 0){

    Serial.println(control);
    control = 0;   

  } 

  if(setDistance >= 10 && setDistance != 0){

    Serial.println(setDistance);
    setDistance = 0;   

  } 

  AutomationOnOff();


}

void AutomationOnOff(){
  if(mode==1) {
    Serial.println("1"); //automation is on
    mode = 0;
  } 
  else if(mode == 3){
     Serial.println("3"); //automation is off
     mode = 0;  
  }
}

void Notif(){
   if(mySerial.available()){
      String msg = mySerial.readStringUntil('\n');   
      msg.trim();
      
      int index = msg.indexOf(':');

      if (index != -1) { // Check if colon character found
      String valueStr = msg.substring(index + 1); // Extract the substring after the colon character
      value = valueStr.toInt(); // Convert the extracted substring to integer
      }
     
    
     if(value>0 && value<=100){
       Blynk.virtualWrite(V2, value);
     }

     if(value == 101){
        Blynk.logEvent("distance_notif", String("Walker detected an obstacle. Now Turning."));
      }

  }
}