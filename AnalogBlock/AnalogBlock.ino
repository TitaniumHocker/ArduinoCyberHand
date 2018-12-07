#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
int EMG_PIN = A0;
int thresholdTop = 0;
int thresholdBot = 0;
int i = 20;

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
  delay(5000);
}

void loop() {
  const byte msg = B1101;
  float sensorValue = analogRead(EMG_PIN);
  float filtered_sensorValue = filter(sensorValue);
  if (i == 20){
    thresholdTop = filtered_sensorValue * 1.20;
    thresholdBot = filtered_sensorValue * 1.10;
    i = 0;
  }
  Serial.print("$");
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.print(filtered_sensorValue);
  Serial.print(" ");
  Serial.print(thresholdTop);
  Serial.print(" ");
  Serial.print(thresholdBot);
  Serial.println(";");
  if (filtered_sensorValue > thresholdBot && filtered_sensorValue < thresholdTop){
    radio.write(&msg, 4);
    i = 19;
  }
  delay(2);
  i++;
  /*
  radio.write(&msg, 4);
  delay(5000);
  */
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
