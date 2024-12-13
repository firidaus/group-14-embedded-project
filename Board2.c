// Board 2 with LM 35 Temperature sensor
#include <Wire.h>

#define TEMP_SENSOR_PIN A0
#define LED_PIN 9
#define SLAVE_ADDRESS 0x08 // I2C address for this slave

int tempThreshold = 20; // Default temperature threshold in Celsius

void setup() {
    // Configure ADC
    ADMUX = (1 << REFS0); // Reference voltage set to AVCC (5V)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC with prescaler 64

    // Configure LED pin
    DDRB |= (1 << LED_PIN);       
    PORTB &= ~(1 << LED_PIN);     

    // Initialize I2C communication
    Wire.begin(SLAVE_ADDRESS); // Join I2C bus as a slave
    Wire.onRequest(sendTemperatureToMaster); // Register the request event
}

float readTemperature() {
    int sensorValue = analogRead(TEMP_SENSOR_PIN);
    float voltage = sensorValue * (5.0 / 1023.0); // Convert to voltage
    return voltage * 100; // Convert to temperature for LM35
}

void loop() {
    float temp = readTemperature();

    // Compare temperature with the threshold and control the LED
    if (temp > tempThreshold) {
       PORTB |= (1 << LED_PIN);  

    } else {
       PORTB &= ~(1 << LED_PIN); 
    }

    delay(500); // Pause for stability
}

void sendTemperatureToMaster() {
    float temperature = readTemperature();

    union {
        byte b[4];
        float value;
    } tempData;

    tempData.value = temperature;

    // Send temperature as 4 bytes
    Wire.write(tempData.b, 4);
}