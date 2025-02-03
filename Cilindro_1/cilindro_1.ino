void setup() {
    pinMode(7, INPUT);
    pinMode(8, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(7, LOW);
    delay(30);

    Serial.begin(9600);
}

void loop() {
    digitalWrite(8, HIGH);
    delayMicroseconds(10);
    digitalWrite(8, LOW);
    int durata = pulseIn(7, HIGH);
    long cm = durata / 58;
    Serial.println(cm);
    
    int listaPesi[11] = {10, 180, 190, 200, 232, 34, 102, 2090, 1000, 1120, 180};
    
    for(int Index = 0; Index < sizeof(listaPesi); Index++){
      int peso = listaPesi[Index];

      if(peso == 180 && (cm > 1 and cm < 20)){
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
      }
      delay(1000);
    }
    
      
}