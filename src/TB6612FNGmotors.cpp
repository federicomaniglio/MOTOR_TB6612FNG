//
// Created by federicomaniglio on 12/12/25.
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
    // Configurazione PWM per motore A
    ledcSetup(PWM_CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWMA, PWM_CHANNEL_A);

    // Configurazione PWM per motore B
    ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWMB, PWM_CHANNEL_B);
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
                                 const uint8_t pwm_channel_a, const uint8_t pwm_channel_b,
                                 const uint32_t pwm_freq, const uint8_t pwm_resolution)
    : STBY(pin_stby), AIN1(ain1), AIN2(ain2), PWMA(pwma), BIN1(bin1), BIN2(bin2), PWMB(pwmb),
      PWM_CHANNEL_A(pwm_channel_a), PWM_CHANNEL_B(pwm_channel_b),
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
    ledcWrite(PWM_CHANNEL_A, 0);
}

void TB6612FNGmotors::stopMotorB() const {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWM_CHANNEL_B, 0);
}

void TB6612FNGmotors::forwardMotorA(const uint8_t motorPowerPWM) const {
    digitalWrite(STBY, HIGH);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWM_CHANNEL_A, motorPowerPWM);
}

void TB6612FNGmotors::backwardMotorA(const uint8_t motorPowerPWM) const {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    ledcWrite(PWM_CHANNEL_A, motorPowerPWM); // velocita: 0-255
}

void TB6612FNGmotors::forwardMotorB(const uint8_t motorPowerPWM) const {
    digitalWrite(STBY, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWM_CHANNEL_B, motorPowerPWM);
}

void TB6612FNGmotors::backwardMotorB(const uint8_t motorPowerPWM) const {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(PWM_CHANNEL_B, motorPowerPWM); // velocita: 0-255
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
    ledcWrite(PWM_CHANNEL_A, 255);
}
void TB6612FNGmotors::motorBrake_B() const {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, HIGH);
    ledcWrite(PWM_CHANNEL_B, 255);
}

void TB6612FNGmotors::motorBrake() const {
    this->motorBrake_A();
    this->motorBrake_B();
}
