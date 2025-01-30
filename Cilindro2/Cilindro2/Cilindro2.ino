#define TRIG_PIN 8
#define ECHO_PIN 7
#define LED_PIN 11

long durata;
int cm;
int peso[] = {180, 200, 220, 180, 220, 250, 100};

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
   
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    durata = pulseIn(ECHO_PIN, HIGH);
    cm = durata / 58;

    // Serial.print("Distanza:");
    // Serial.println(cm);
    // Serial.println("Peso:");
    // Serial.println(peso[2]);

    
    for(int i = 0; i < 7; i++){
    Serial.print("Distanza:");
    Serial.println(cm);
    Serial.println("Peso:");
    Serial.println(peso[i]);
    if (cm <= 20) {
        if (peso[i] >209 && peso[i] < 251) {  
            delay(1000);
            digitalWrite(LED_PIN, HIGH);
            delay(500); 
        } else {
            digitalWrite(LED_PIN, LOW);
        }
    } else {
        digitalWrite(LED_PIN, LOW);
    }

    delay(500);
    } 
}
