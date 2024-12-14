// BOARD 4 MASTER BOARD with LCD to display the Temperature and Occupancy of the room 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TEMP_SLAVE_ADDRESS 0x08   // I2C address of the temperature slave
#define MOTION_SLAVE_ADDRESS 0x09 // I2C address of the motion/occupancy slave
#define BUZZER_SLAVE_ADDRESS 0x0A // I2C address of the buzzer slave


LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
    // Initialize the LCD
    lcd.begin(16, 2);   
    lcd.backlight();     
    lcd.print("Initializing...");
    delay(1000);         
    lcd.clear();

    // Initialize I2C communication
    Wire.begin();
}

void loop() {
    float temperature = requestTemperatureFromSlave();
    int motionCount = requestMotionFromSlave();

    // Check if temperature is greater than 20 and send buzzer command to third slave
    if (temperature > 20.0 || motionCount > 15) {
    sendBuzzerCommand('A'); // Command to turn on buzzer
}


    // Display temperature and Occupancy count on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(temperature, 1);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("Occupancy: ");
    lcd.print(motionCount);

    delay(1000);
}


float requestTemperatureFromSlave() {
    Wire.requestFrom(TEMP_SLAVE_ADDRESS, 4); 

    union {
        byte b[4];
        float value;
    } tempData;

    int i = 0;
    while (Wire.available() && i < 4) {
        tempData.b[i++] = Wire.read();
    }

    return tempData.value;
}

int requestMotionFromSlave() {
    Wire.requestFrom(MOTION_SLAVE_ADDRESS, 2); 

    union {
        byte b[2];
        int value;
    } motionData;

    int i = 0;
    while (Wire.available() && i < 2) {
        motionData.b[i++] = Wire.read();
    }

    return motionData.value; 
}

void sendBuzzerCommand(char command) {
    Wire.beginTransmission(BUZZER_SLAVE_ADDRESS);
    Wire.write(command);                        
    Wire.endTransmission();                     
}
