//
// Created by federicomaniglio on 12/12/25.
//
// 01/06/2026 - Refactoring: rimosse le variabili legacy dei canali PWM per compatibilità con le API ESP32 V3.0+.
//
// 01/06/2026 - Refactoring: Rimosso il metodo superfluo nel costruttore per i motori in seguito alla rimozione delle variabili legacy
//

#ifndef MOTOR_TB6612FNG_TB6612FNGMOTORS_H
#define MOTOR_TB6612FNG_TB6612FNGMOTORS_H
#include <cstdint>


class TB6612FNGmotors {
private:
    int STBY = 4; // Default GPIO4 - Pin Standby (deve essere HIGH per abilitare il driver)
    int AIN1 = 14; // Default GPIO14 - Pin controllo direzione motore A
    int AIN2 = 13; // Default GPIO13 - Pin controllo direzione motore A
    int PWMA = 12; // Default GPIO12 - Pin PWM per controllo velocità motore A

    // Motore B
    int BIN1 = 17; // Default GPIO17 - Pin controllo direzione motore B
    int BIN2 = 18;
    int PWMB = 16;

    // Parametri PWM
    uint32_t PWM_FREQ = 5000; // 5 KHz - frequenza standard per motori
    uint8_t PWM_RESOLUTION = 8; // 8 bit (0-255)

    void initializePins() const;

    void initializePWM() const; // Nuova funzione

public:
    // Costruttore di default
    TB6612FNGmotors();

    // Costruttore senza STBY
    TB6612FNGmotors(int ain1, int ain2, int pwma, int bin1, int bin2, int pwmb);

    // Costruttore avanzato con configurazione PWM personalizzata
    TB6612FNGmotors(int pin_stby, int ain1, int ain2, int pwma, int bin1, int bin2, int pwmb,
                    uint8_t pwm_channel_a, uint8_t pwm_channel_b,
                    uint32_t pwm_freq = 5000, uint8_t pwm_resolution = 8);

    void enableDriver() const;

    void disableDriver() const;

    void stopMotorA() const;

    void stopMotorB() const;

    void forwardMotorA(uint8_t motorPowerPWM = 180) const;

    void backwardMotorA(uint8_t motorPowerPWM = 180) const;

    void forwardMotorB(uint8_t motorPowerPWM = 180) const;

    void backwardMotorB(uint8_t motorPowerPWM = 180) const;

    void stopMotors() const;

    void forwardMotors(uint8_t motorPowerPWM = 180, bool oppositePolarity = false) const;

    void backwardMotors(uint8_t motorPowerPWM = 180, bool oppositePolarity = false) const;

    void motorBrake_A() const;

    void motorBrake_B() const;

    void motorBrake() const;
};

#endif //MOTOR_TB6612FNG_TB6612FNGMOTORS_H
