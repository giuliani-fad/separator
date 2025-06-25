#include <Arduino.h>
#include "HX711.h"
#include <LiquidCrystal_I2C.h>

// Ultrasuoni
#define trig_pin_1 13
#define echo_pin_1 14
#define trig_pin_2 26
#define echo_pin_2 25

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;

// Relè
const int rele_1 = 32;
const int rele_2 = 33;
const int rele_3 = 18;
const int rele_4 = 19;

const int buttonPin = 35;
const int buttonLed = 2;
const int rele_1Led = 23;
const int rele_2Led = 17;

int pezzo = 0;
bool standBy = false;
bool button = false;
int peso = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

HX711 scale;

void setup() {
  // Ultrasuoni
  pinMode(trig_pin_1, OUTPUT);
  pinMode(echo_pin_1, INPUT);
  pinMode(trig_pin_2, OUTPUT);
  pinMode(echo_pin_2, INPUT);

  // Relè
  pinMode(rele_1, OUTPUT);
  pinMode(rele_2, OUTPUT);
  pinMode(rele_3, OUTPUT);
  pinMode(rele_4, OUTPUT);
  digitalWrite(rele_1, LOW);
  digitalWrite(rele_2, LOW);
  digitalWrite(rele_3, LOW);
  digitalWrite(rele_4, LOW);

  pinMode(buttonPin, INPUT);
  pinMode(buttonLed, OUTPUT);
  pinMode(rele_1Led, OUTPUT);
  pinMode(rele_2Led, OUTPUT);

  // Inizializzazione LCD
  lcd.init();
  lcd.backlight();        // Accensione retroilluminazione LCD
  lcd.setCursor(0, 0);    // Posizionamento prima colonna, prima riga
  lcd.print("Avvio...");  // Messaggio

  Serial.begin(115200);

  //Inizializzazione bilancia
  init_peso();

  // Caricamento primo pezzo
  caricatore();
}

void loop() {

  while(!standBy){
    if (peso <= 1){
      peso = round(weight());

      if (peso > 1){
        delay(1000);
        peso = round(weight());
      }
      Serial.print("Peso: ");
      Serial.println(peso);

      String lcdMessage = "Peso: " + String(peso);
      lcdPrint(0, 0, true, "Peso: " + String(peso));
    }
    
    while(peso > 1){

      // SENSORE 2
      if (peso < 50){
        lcdPrint(0, 1, false, "Attesa sensore 2");
        digitalWrite(rele_2Led, HIGH);

        // Verifica passaggio del pezzo
        long distanza = distance(trig_pin_2, echo_pin_2);

        if (distanza < 20){
          delay(2000);
          rele(rele_2);
          
          Serial.println("Sensore 2 attivato!");
          lcdPrint(0, 1, true, "Sen. 2 attivato!");
          digitalWrite(rele_2Led, LOW);
          caricatore();
          peso = 0;
        }
      }

      // SENSORE 1
      if (peso > 60){
        lcdPrint(0, 1, false, "Attesa sensore 1");
        digitalWrite(rele_1Led, HIGH);

        // Verifica passaggio del pezzo
        long distanza = distance(trig_pin_1, echo_pin_1);

        if (distanza < 20){
          delay(2000);
          rele(rele_1);

          Serial.println("Sensore 1 attivato!");
          lcdPrint(0, 1, true, "Sen. 1 attivato!");
          digitalWrite(rele_1Led, LOW);
          caricatore();
          peso = 0;
        }
        
      }
    }
  }
  
  lcdPrint(0, 0, true, "Sostituire il");
  lcdPrint(0, 1, false, "caricatore");

  digitalWrite(buttonLed, HIGH);

  button = digitalRead(buttonPin);
  Serial.println(button);
  delay(250);
  if (button){
    standBy = false;
    digitalWrite(buttonLed, LOW);
    caricatore();
  }

}

// Inizializzazione bilancia
void init_peso(){
  Serial.println("HX711 Demo");

  Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
            
  scale.set_scale(208);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}

float weight(){
  float peso = scale.get_units(10);
  scale.power_down();             // put the ADC in sleep mode
  delay(1000);
  scale.power_up();

  return peso;
}

long distance(int trig, int echo){
  digitalWrite(trig, LOW);    // imposta l'uscita del trigger LOW
  digitalWrite(trig, HIGH);   // imposta l'uscita del trigger HIGH per 10 microsecondi
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // calcolo del tempo attraverso il pin di echo
  long durata = pulseIn(echo, HIGH);
  long distanza = durata/58.31;

  return distanza;
}

void caricatore(){
  // NUOVO CARICATORE
  if (pezzo == 9 && !standBy){
    digitalWrite(rele_4, HIGH);
    delay(3000);
    digitalWrite(rele_4, LOW);
    pezzo = 0;
    standBy = true;
  }

  // NUOVO PEZZO
  if(!standBy && pezzo <= 9){
    pezzo += 1;
    lcdPrint(0, 1, true, "Carico pezzo " + String(pezzo));
    digitalWrite(rele_3, HIGH);
    delay(130);
    digitalWrite(rele_3, LOW);
  }
}

void rele(int pin){
  digitalWrite(pin, HIGH);
  delay(300);
  digitalWrite(pin, LOW);
}

void lcdPrint(int column, int row, bool clear, String message){
  if (clear){
    lcd.clear();
  }

  lcd.setCursor(column, row);
  lcd.print(message); 
}