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
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    
    if (text[1] == "u" && turned == false){
      S1.write(180);
      turned = true;
      delay(1000);
    } else if (text[1] == "u" && turned == true){
      S1.write(0);
      turned = false;
      delay(1000);
    }
    
  }
}
