//
// Created by federicomaniglio on 12/12/25.
//
// 01/06/2026 - Refactoring: rimosse le variabili legacy dei canali PWM per compatibilità con le API ESP32 V3.0+.
// 
// 01/06/2026 - Refactoring: sostituiti le funzioni rimosse ledcSetup e ledcAttachPin con ledcAttach per la compatibilità con le API ESP32 V3.0+
//
#include <Arduino.h>

#include "../include/TB6612FNGmotors.h"

void TB6612FNGmotors::initializePins() const {
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(STBY, OUTPUT);
}

void TB6612FNGmotors::initializePWM() const {
    // Nuova configurazione PWM per ESP32 V3.0+ PWM (ledcSetup e ledcAttachPin sono riuniti in ledcAttach)
    ledcAttach(PWMA, PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(PWMB, PWM_FREQ, PWM_RESOLUTION);
}

// Costruttore di default
TB6612FNGmotors::TB6612FNGmotors() {
    initializePWM();
    initializePins();
}

// Costruttore con pin personalizzati
TB6612FNGmotors::TB6612FNGmotors(const int pin_stby, const int ain1, const int ain2, const int pwma,
                                 const int bin1, const int bin2, const int pwmb)
    : STBY(pin_stby), AIN1(ain1), AIN2(ain2), PWMA(pwma), BIN1(bin1), BIN2(bin2), PWMB(pwmb) {
    initializePWM();
    initializePins();
}

// Costruttore senza STBY
TB6612FNGmotors::TB6612FNGmotors(const int ain1, const int ain2, const int pwma,
                                 const int bin1, const int bin2, const int pwmb)
    : AIN1(ain1), AIN2(ain2), PWMA(pwma), BIN1(bin1), BIN2(bin2), PWMB(pwmb) {
    initializePWM();
    initializePins();
}

// Costruttore avanzato con configurazione PWM personalizzata
TB6612FNGmotors::TB6612FNGmotors(const int pin_stby, const int ain1, const int ain2, const int pwma,
                                 const int bin1, const int bin2, const int pwmb,
                                 const uint32_t pwm_freq, const uint8_t pwm_resolution)
    : STBY(pin_stby), AIN1(ain1), AIN2(ain2), PWMA(pwma), BIN1(bin1), BIN2(bin2), PWMB(pwmb),
      PWM_FREQ(pwm_freq), PWM_RESOLUTION(pwm_resolution) {
    initializePWM();
    initializePins();
}

void TB6612FNGmotors::enableDriver() const {
    digitalWrite(STBY, HIGH);
}

void TB6612FNGmotors::disableDriver() const {
    digitalWrite(STBY, LOW);
}

void TB6612FNGmotors::stopMotorA() const {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, 0);
}

void TB6612FNGmotors::stopMotorB() const {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, 0);
}

void TB6612FNGmotors::forwardMotorA(const uint8_t motorPowerPWM) const {
    digitalWrite(STBY, HIGH);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, motorPowerPWM);
}

void TB6612FNGmotors::backwardMotorA(const uint8_t motorPowerPWM) const {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    ledcWrite(PWMA, motorPowerPWM); // velocita: 0-255
}

void TB6612FNGmotors::forwardMotorB(const uint8_t motorPowerPWM) const {
    digitalWrite(STBY, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, motorPowerPWM);
}

void TB6612FNGmotors::backwardMotorB(const uint8_t motorPowerPWM) const {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(PWMB, motorPowerPWM); // velocita: 0-255
}

void TB6612FNGmotors::stopMotors() const {
    this->stopMotorA();
    this->stopMotorB();
}

void TB6612FNGmotors::forwardMotors(const uint8_t motorPowerPWM, const bool oppositePolarity) const {
    if (oppositePolarity) {
        this->backwardMotorA(motorPowerPWM);
        this->forwardMotorB(motorPowerPWM);
    } else {
        this->forwardMotorA(motorPowerPWM);
        this->forwardMotorB(motorPowerPWM);
    }
}

void TB6612FNGmotors::backwardMotors(uint8_t motorPowerPWM, bool oppositePolarity) const {
    if (oppositePolarity) {
        this->backwardMotorB(motorPowerPWM);
        this->forwardMotorA(motorPowerPWM);
    } else {
        this->backwardMotorB(motorPowerPWM);
        this->backwardMotorA(motorPowerPWM);
    }
}

void TB6612FNGmotors::motorBrake_A() const {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, HIGH);
    ledcWrite(PWMA, 255);
}
void TB6612FNGmotors::motorBrake_B() const {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, HIGH);
    ledcWrite(PWMB, 255);
}

void TB6612FNGmotors::motorBrake() const {
    this->motorBrake_A();
    this->motorBrake_B();
}
