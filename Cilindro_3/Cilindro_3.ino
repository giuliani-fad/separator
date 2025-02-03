int peso[]{1, 123, 0, 133, 0};

void setup() {
  pinMode(7, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  for (int i = 0; i < 5; i++) {
    Serial.println(peso[i]);
    if (peso[i] == 0) {
      
      digitalWrite(7, HIGH);
      delay(3000); 
    }
    
    digitalWrite(7, LOW); 
    delay(3000); 
  }
  
  
  delay(1000); 
}
