// BOARD 1 with PIR Sensor SLAVE Board
#include <Wire.h>

const int motionSensorPin = 2; // Pin for the PIR motion sensor
const int ledPin = 13;         // Pin for the onboard LED

int motionState = 0;           
unsigned long lastDetectionTime = 0;  
unsigned long cooldown = 5000;  
int occupancy = 0;             

#define MOTION_SLAVE_ADDRESS 0x09 // I2C address of this slave

void setup() {
    DDRD &= ~(1 << motionSensorPin); 
    DDRB |= (1 << ledPin);
    Wire.begin(MOTION_SLAVE_ADDRESS);
    Wire.onRequest(sendMotionData);  
}

void loop() {
    motionState = PIND & (1 << motionSensorPin); // Read the state of the PIR sensor

    // Check if motion is detected and if cooldown period has passed
    if (motionState == HIGH && (millis() - lastDetectionTime) > cooldown) {
       PORTB |= (1 << ledPin);  
        occupancy++;  
        lastDetectionTime = millis();  
        delay(2000);  
    } else if (motionState == LOW) {
        PORTB &= ~(1 << ledPin);  // Turn off LED (no motion)
    }

    delay(200);  // Short delay for stabilization
}

void sendMotionData() {
    union {
        byte b[2];
        int value;
    } motionData;

    motionData.value = occupancy;

    // Send the motion count as 2 bytes
    Wire.write(motionData.b, 2);
}