#include <SoftwareSerial.h>

#define EMG_PIN1 A2;
#define EMG_PIN2 A7;
#define BT_RxD 6;
#define BT_TxD 7;

const byte standByCommand = B0001;
const byte toFlatCommand = B0002;
const byte toFistCommand = B0003;

double multipler = 1.5;
int threshold;
bool standbyFlag = false;
bool firstStart = true;
int startTime;
int timer;

SoftwareSerial btSerial(BT_RxD, BT_TxD);

// kalman
double varVolt = 3.65;  // среднее отклонение (ищем в excel = 3,655747022)
double varProcess = 0.02; // скорость реакции на изменение (подбирается вручную)
double Pc = 0.0;
double G = 0.0;
double P = 1.0;
double Xp = 0.0;
double Zp = 0.0;
double Xe = 0.0;
//
double filter(double val) {  //функция фильтрации
  Pc = P + varProcess;
  G = Pc/(Pc + varVolt);
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(val-Zp)+Xp; // "фильтрованное" значение
  return(Xe);
}


void setup() {
  Serial.begin(9600);
  btSerial.begin(38400);
  delay(5000);
  startTime = millis() / 1000;
}

void loop() {
  double sensorValue1 = analogRead(EMG_PIN1);
  double sensorValue2 = analogRead(EMG_PIN2);
  double filtered_sensorValue = filter(sensorValue1);
  double filtered_sensorValue2 = filter(sensorValue2);
  
  Serial.print("$");
  Serial.print(sensorValue1);
  Serial.print(" ");
  Serial.print(filtered_sensorValue);
  Serial.print(" ");
  Serial.print(sensorValue2);
  Serial.print(" ");
  Serial.print(threshold);
  Serial.print(" ");
  Serial.print(-threshold * 2);
  Serial.println(";");

  
  if (filtered_sensorValue > threshold * multipler * 0.8){
    btSerial.write(toFistCommand);
  } else if (filtered_sensorValue < -threshold * multipler){
    btSerial.write(toFlatCommand);
  } else {
    btSerial.write(standByCommand);
  }
}
