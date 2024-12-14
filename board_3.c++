// BOARD 3 with the Buzzer
#include <Wire.h>

// Define memory-mapped I/O registers
#define PORTD_REG (*(volatile uint8_t *)0x2B)  // PORTD
#define DDRD_REG (*(volatile uint8_t *)0x2A)   // DDRD
#define PIND_REG (*(volatile uint8_t *)0x29)   // PIND

// Define pins
#define BUZZER_PIN 7  
#define BUTTON_PIN 2 

// Define I2C address
#define BUZZER_SLAVE_ADDRESS 0x0A // I2C address of this slave

void setup() {
    // Configure BUZZER_PIN as output
    DDRD_REG |= (1 << BUZZER_PIN);

    // Configure BUTTON_PIN as input with pull-up
    DDRD_REG &= ~(1 << BUTTON_PIN);  
    PORTD_REG |= (1 << BUTTON_PIN);  

    // Turn off Buzzer by default
    PORTD_REG &= ~(1 << BUZZER_PIN);

    // Initialize I2C communication
    Wire.begin(BUZZER_SLAVE_ADDRESS); // Join I2C bus as a slave
    Wire.onReceive(receiveBuzzerCommand); // Register function to handle commands
}

void loop() {
    // Check if the button is pressed (active LOW)
    if (!(PIND_REG & (1 << BUTTON_PIN))) {  // Check if BUTTON_PIN is LOW
        PORTD_REG &= ~(1 << BUZZER_PIN);    // Turn off the Buzzer
    }
}

// Function to handle I2C commands from the master
void receiveBuzzerCommand(int numBytes) {
    while (Wire.available()) {
        char command = Wire.read(); // Read command from master

        if (command == 'A') { // Command to turn on the buzzer
            PORTD_REG |= (1 << BUZZER_PIN); // Turn on Buzzer
        }
    }
}