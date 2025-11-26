#include <Arduino.h>

// Definizione pin per TB6612FNG con ESP32
// Pin PWM compatibili con ESP32
#define PWMA 25   // GPIO25 - Pin PWM per controllo velocità motore A
#define AIN1 26   // GPIO26 - Pin controllo direzione motore A
#define AIN2 27   // GPIO27 - Pin controllo direzione motore A
#define STBY 14   // GPIO14 - Pin Standby (deve essere HIGH per abilitare il driver)

// Configurazione canale PWM per ESP32
#define PWM_CHANNEL 0
#define PWM_FREQ 5000      // 5 KHz
#define PWM_RESOLUTION 8   // 8 bit (0-255)

// Funzione per muovere il motore avanti
void motorAvanti(int velocita) {
  digitalWrite(STBY, HIGH);  // Abilita il driver
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  ledcWrite(PWM_CHANNEL, velocita);  // velocita: 0-255
}

// Funzione per muovere il motore indietro
void motorIndietro(int velocita) {
  digitalWrite(STBY, HIGH);  // Abilita il driver
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  ledcWrite(PWM_CHANNEL, velocita);  // velocita: 0-255
}

// Funzione per fermare il motore
void
motorStop() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  ledcWrite(PWM_CHANNEL, 0);
}

// Funzione per mettere il driver in standby (basso consumo)
void motorStandby() {
  digitalWrite(STBY, LOW);
}

// Funzione per frenare il motore (cortocircuito)
void motorFrena() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  ledcWrite(PWM_CHANNEL, 255);
}

void setup() {
  // Inizializzazione comunicazione seriale per debug
  Serial.begin(115200);  // ESP32 usa tipicamente 115200 baud
  Serial.println("Controllo Motore TB6612FNG con ESP32");

  // Configurazione PWM per ESP32
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWMA, PWM_CHANNEL);

  // Configurazione pin come OUTPUT
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Motore fermo all'avvio
  motorStop();

  Serial.println("Sistema inizializzato!");
}

void loop() {
  // Esempio di utilizzo del motore

  Serial.println("Motore AVANTI - velocità media");
  motorAvanti(150);  // Velocità media (0-255)
  delay(2000);

  Serial.println("Motore STOP");
  motorStop();
  delay(1000);

  Serial.println("Motore INDIETRO - velocità massima");
  motorIndietro(255);  // Velocità massima
  delay(2000);

  Serial.println("Motore FRENA");
  motorFrena();
  delay(1000);

  Serial.println("Motore AVANTI - velocità bassa");
  motorAvanti(80);  // Velocità bassa
  delay(2000);

  Serial.println("Motore in STANDBY");
  motorStandby();
  delay(3000);
}