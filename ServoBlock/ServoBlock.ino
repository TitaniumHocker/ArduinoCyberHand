#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

Servo S1;
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
bool turned = false;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  S1.attach(3);
  S1.write(180);
  delay(2000);
  S1.write(0);
  delay(3000);
}

void loop() {
  if (radio.available()) {
    byte msg = B0000;
    radio.read(&msg, 4);
    Serial.println(msg);
    //delay(500);
    if (msg == B1101 && turned == false){
      S1.write(90);
      turned = true;
      delay(1000);
    } else if (msg == B1101 && turned == true){
      S1.write(0);
      turned = false;
      delay(1000);
    }
    msg = B0000;
  }
}
