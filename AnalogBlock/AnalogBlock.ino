#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
int EMG_PIN = A0;
int threshold = 0;

// kalman
float varVolt = 3.65;  // среднее отклонение (ищем в excel = 3,655747022)
float varProcess = 0.02; // скорость реакции на изменение (подбирается вручную)
float Pc = 0.0;
float G = 0.0;
float P = 1.0;
float Xp = 0.0;
float Zp = 0.0;
float Xe = 0.0;
//

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  const byte msg = B1101;
  float sensorValue = analogRead(EMG_PIN);
  float filtered_sensorValue = filter(sensorValue);
  threshold = filtered_sensorValue * 1.5;
  Serial.print("$");
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.print(filtered_sensorValue);
  Serial.print(" ");
  Serial.print(threshold);
  Serial.println(";");
  if (filtered_sensorValue > threshold){
    radio.write(&msg, 4);
  }
  delay(2);
}

float filter(float val) {  //функция фильтрации
  Pc = P + varProcess;
  G = Pc/(Pc + varVolt);
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(val-Zp)+Xp; // "фильтрованное" значение
  return(Xe);
}
