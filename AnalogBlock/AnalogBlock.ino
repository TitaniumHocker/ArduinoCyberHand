
const int EMG_PIN1 = A2;
const int EMG_PIN2 = A7;
int threshold = 150;

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
  Serial.begin(38400);;
  delay(5000);
  startTime = millis() / 1000;
}

void loop() {
  int sensorValue1 = analogRead(EMG_PIN1);
  int sensorValue2 = analogRead(EMG_PIN2);
  int filtered_sensorValue = filter(sensorValue1);
  filtered_sensorValue = filter(-sensorValue2);
  
  char msg[50];
  sprintf(msg, "%i %i %i %i %i;\n", 
          sensorValue1, 
          -sensorValue2, 
          filtered_sensorValue, 
          threshold, 
          -threshold);
  Serial.write(msg);
  delay(50);
}
