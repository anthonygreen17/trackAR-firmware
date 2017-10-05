//Use to test transceiver functionality from one transceiver to the other


void setup() {
  Serial.begin(9600);                       // Open serial port to computer
  Serial1.begin(9600);                         // Open serial port to HC12
} 

void loop() {
  if(Serial1.available()){                     // If Arduino's HC12 rx buffer has data
    Serial.write(Serial1.read());              // Send the data to the computer
    }
  if(Serial.available()){                   // If Arduino's computer rx buffer has data
    Serial1.write(Serial.read());              // Send that data to serial
  }
}

