//Use to test transceiver functionality from one transceiver to the other
//Specifically, this transceiver sends the data
//Input "$" to make the transceiver sleep (it cannot transmit during sleep)
//Input "%" to make the transceiver return to power
#include "hc12.h"


void setup() {
  Serial.begin(9600);                       // Open serial port to computer
//  Serial1.begin(9600);                         // Open serial port to HC12
  //pinMode(A1, OUTPUT);
  //digitalWrite(A1, HIGH); 
  hc12::initialize();
} 

void loop() {
  while(Serial.available() > 0){
    int readVal = Serial.read();
    if (readVal == 36){  //Type "$" to make the transceiver sleep
      hc12::sleep();
    }else if (readVal == 37){ //Type "%" to make the transceiver return to power
      hc12::unsleep();
    }else{
      Serial1.write(readVal);
    }
  }  
}

void transceiverSleep(){
  digitalWrite(A1, LOW); 
  delay(40);
  Serial1.write("AT+SLEEP");
  Serial1.flush();
  digitalWrite(A1, HIGH); 
  //MIGHT need an additonal delay here!!!!
  Serial.println("Entering Sleep Mode");
}

void transceiverDefault(){
  digitalWrite(A1, LOW); 
  //delay(40);
  //Serial1.write("AT+DEFAULT");
  //Serial1.flush();
  digitalWrite(A1, HIGH); 
  //MIGHT need an additonal delay here!!!!
  Serial.println("Exiting Sleep Mode");
}

