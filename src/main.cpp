
#include <Arduino.h>
#include <TB6612FNGmotors.h>
TB6612FNGmotors motors(4,14,12,13,17,18,16);

void setup() {
    motors.enableDriver();
}

void loop() {
    motors.backwardMotorA(255);
    motors.forwardMotorB(255);
    delay(2000);
    // motors.stopMotors();
    delay(2000);

}