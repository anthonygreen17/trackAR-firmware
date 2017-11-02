//Use to test transceiver functionality from one transceiver to the other
//Specifically, this transceiver recieves the data

void setup() {
  Serial.begin(9600);                       // Open serial port to computer
  Serial1.begin(9600);                         // Open serial port to HC12
} 

//This code is for reading data
void loop() {
  while(Serial1.available() > 0){
      Serial.write(Serial1.read());
  }  
}                    
