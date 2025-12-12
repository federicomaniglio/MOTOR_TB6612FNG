// #include <Arduino.h>
//
// // Definizione pin per TB6612FNG con ESP32
// // Motore A
// #define AIN1 14   // GPIO14 - Pin controllo direzione motore A
// #define AIN2 13   // GPIO13 - Pin controllo direzione motore A
// #define PWMA 12   // GPIO12 - Pin PWM per controllo velocità motore A
//
// // Motore B
// #define BIN1 17   // GPIO17 - Pin controllo direzione motore B
// #define BIN2 18   // GPIO18 - Pin controllo direzione motore B
// #define PWMB 16   // GPIO16 - Pin PWM per controllo velocità motore B
//
// // Standby
// #define STBY 4    // GPIO4 - Pin Standby (deve essere HIGH per abilitare il driver)
//
// // Configurazione canali PWM per ESP32
// #define PWM_CHANNEL_A 0
// #define PWM_CHANNEL_B 1
// #define PWM_FREQ 5000      // 5 KHz
// #define PWM_RESOLUTION 8   // 8 bit (0-255)
//
// // ===== FUNZIONI MOTORE A =====
//
// // Funzione per muovere il motore A avanti
// void motorA_Avanti(int velocita) {
//   digitalWrite(STBY, HIGH);  // Abilita il driver
//   digitalWrite(AIN1, HIGH);
//   digitalWrite(AIN2, LOW);
//   ledcWrite(PWM_CHANNEL_A, velocita);  // velocita: 0-255
// }
//
// // Funzione per muovere il motore A indietro
// void motorA_Indietro(int velocita) {
//   digitalWrite(STBY, HIGH);  // Abilita il driver
//   digitalWrite(AIN1, LOW);
//   digitalWrite(AIN2, HIGH);
//   ledcWrite(PWM_CHANNEL_A, velocita);  // velocita: 0-255
// }
//
// // Funzione per fermare il motore A
// void motorA_Stop() {
//   digitalWrite(AIN1, LOW);
//   digitalWrite(AIN2, LOW);
//   ledcWrite(PWM_CHANNEL_A, 0);
// }
//
// // Funzione per frenare il motore A (cortocircuito)
// void motorA_Frena() {
//   digitalWrite(STBY, HIGH);
//   digitalWrite(AIN1, HIGH);
//   digitalWrite(AIN2, HIGH);
//   ledcWrite(PWM_CHANNEL_A, 255);
// }
//
// // ===== FUNZIONI MOTORE B =====
//
// // Funzione per muovere il motore B avanti
// void motorB_Avanti(int velocita) {
//   digitalWrite(STBY, HIGH);  // Abilita il driver
//   digitalWrite(BIN1, HIGH);
//   digitalWrite(BIN2, LOW);
//   ledcWrite(PWM_CHANNEL_B, velocita);  // velocita: 0-255
// }
//
// // Funzione per muovere il motore B indietro
// void motorB_Indietro(int velocita) {
//   digitalWrite(STBY, HIGH);  // Abilita il driver
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, HIGH);
//   ledcWrite(PWM_CHANNEL_B, velocita);  // velocita: 0-255
// }
//
// // Funzione per fermare il motore B
// void motorB_Stop() {
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, LOW);
//   ledcWrite(PWM_CHANNEL_B, 0);
// }
//
// // Funzione per frenare il motore B (cortocircuito)
// void motorB_Frena() {
//   digitalWrite(STBY, HIGH);
//   digitalWrite(BIN1, HIGH);
//   digitalWrite(BIN2, HIGH);
//   ledcWrite(PWM_CHANNEL_B, 255);
// }
//
// // ===== FUNZIONI CONTROLLO SIMULTANEO =====
//
// // Funzione per muovere entrambi i motori avanti
// void entrambiAvanti(int velocitaA, int velocitaB) {
//   digitalWrite(STBY, HIGH);
//   motorA_Avanti(velocitaA);
//   motorB_Avanti(velocitaB);
// }
//
// // Funzione per muovere entrambi i motori indietro
// void entrambiIndietro(int velocitaA, int velocitaB) {
//   digitalWrite(STBY, HIGH);
//   motorA_Indietro(velocitaA);
//   motorB_Indietro(velocitaB);
// }
//
// // Funzione per fermare entrambi i motori
// void entrambiStop() {
//   motorA_Stop();
//   motorB_Stop();
// }
//
// // Funzione per frenare entrambi i motori
// void entrambiFrena() {
//   digitalWrite(STBY, HIGH);
//   motorA_Frena();
//   motorB_Frena();
// }
//
// // Funzione per girare a destra (motore A avanti, motore B indietro)
// void giraDestr(int velocita) {
//   digitalWrite(STBY, HIGH);
//   motorA_Avanti(velocita);
//   motorB_Indietro(velocita);
// }
//
// // Funzione per girare a sinistra (motore A indietro, motore B avanti)
// void giraSinistra(int velocita) {
//   digitalWrite(STBY, HIGH);
//   motorA_Indietro(velocita);
//   motorB_Avanti(velocita);
// }
//
// // Funzione per mettere il driver in standby (basso consumo)
// void motorStandby() {
//   digitalWrite(STBY, LOW);
// }
//
// void setup() {
//   // Inizializzazione comunicazione seriale per debug
//   Serial.begin(115200);
//   Serial.println("Controllo Doppio Motore TB6612FNG con ESP32");
//
//   // Configurazione PWM per entrambi i motori
//   ledcSetup(PWM_CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
//   ledcAttachPin(PWMA, PWM_CHANNEL_A);
//
//   ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
//   ledcAttachPin(PWMB, PWM_CHANNEL_B);
//
//   // Configurazione pin come OUTPUT
//   pinMode(AIN1, OUTPUT);
//   pinMode(AIN2, OUTPUT);
//   pinMode(BIN1, OUTPUT);
//   pinMode(BIN2, OUTPUT);
//   pinMode(STBY, OUTPUT);
//
//   // Motori fermi all'avvio
//   entrambiStop();
//   digitalWrite(STBY, HIGH);
//
//   Serial.println("Sistema inizializzato - Entrambi i motori pronti!");
// }
//
// void loop() {
//   // Esempio di utilizzo di entrambi i motori
//
//   Serial.println("Entrambi i motori AVANTI - velocità media");
//   entrambiAvanti(150, 150);
//   delay(2000);
//
//   Serial.println("Entrambi i motori STOP");
//   entrambiStop();
//   delay(1000);
//
//   Serial.println("Entrambi i motori INDIETRO - velocità massima");
//   entrambiIndietro(255, 255);
//   delay(2000);
//
//   Serial.println("Entrambi i motori FRENANO");
//   entrambiFrena();
//   delay(1000);
//
//   Serial.println("GIRA A DESTRA");
//   giraDestr(200);
//   delay(1500);
//
//   Serial.println("STOP");
//   entrambiStop();
//   delay(1000);
//
//   Serial.println("GIRA A SINISTRA");
//   giraSinistra(200);
//   delay(1500);
//
//   Serial.println("STOP");
//   entrambiStop();
//   delay(1000);
//
//   Serial.println("Motore A avanti, Motore B indietro - velocità diverse");
//   motorA_Avanti(180);
//   motorB_Indietro(120);
//   delay(2000);
//
//   Serial.println("Motori in STANDBY");
//   motorStandby();
//   delay(3000);
// }

#include <Arduino.h>
#include <TB6612FNGmotors.h>
TB6612FNGmotors motors;

void setup() {
    motors.enableDriver();
}

void loop() {
    motors.forwardMotorA(255);
    motors.forwardMotorB(255);
    delay(1000);
    motors.stopMotorA();
    motors.stopMotorB();
    delay(2000);
    motors.backwardMotorA(255);
    motors.backwardMotorB(255);
    delay(2000);
    motors.stopMotors();
    delay(2000);
    motors.forwardMotors(255, true);
    delay(2000);
    motors.motorBrake();
    delay(2000);
    motors.backwardMotors(255, true);
    delay(2000);
    motors.motorBrake();
    delay(2000);

}