/*  HC12 Send/Receive Example Program 1
    By Mark J. Hughes 
    for AllAboutCircuits.com
    
    Connect HC12 "RXD" pin to Arduino Digital Pin 4
    Connect HC12 "TXD" pin to Arduino Digital Pin 5
    Connect HC12 "Set" pin to Arduino Digital Pin 6
   
    Do not power over USB.  Per datasheet, 
    power HC12 with a supply of at least 100 mA with 
    a 22 uF - 1000 uF reservoir capacitor.
    Upload code to two Arduinos connected to two computers.
    
    Transceivers must be at least several meters apart to work.
   
 */

void setup() {
  Serial1.begin(9600);
  delay(500);
  Serial.begin(9600);                        
  delay(500);
}

void loop() {
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}
