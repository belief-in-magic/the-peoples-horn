
// Test program, usb-c xiao rp2040 + mcp23017 + sleep mode

#include <Adafruit_MCP23X17.h>

#include <MCP23017.h>

#define MCP23017_I2C_ADDRESS 0x20

// GPIO Pins
#define RP2040_SDA_PIN 21
#define RP2040_SCL_PIN 22
// TODO: NOOOOO these are incorrect, they don't match with the default i2c interfaces

#define RP2040_INT_PIN 23

#define S1 8
#define S2 9
#define S3 10
#define S4 11

#define PICO_I2C_INSTANCE   i2c1

Adafruit_MCP23X17 mcp;


void setup() {

    Serial.begin(115200);
    delay(4000);
        
    Serial.println("Starting MCP23017 Test");
    

    if (!mcp.begin_I2C()) {
      Serial.println("Error init mcp");
      while(1);
    }

    mcp.pinMode(8, INPUT_PULLUP);
    mcp.pinMode(9, INPUT_PULLUP);
    mcp.pinMode(10, INPUT_PULLUP);
    mcp.pinMode(11, INPUT_PULLUP);

    
    Serial.println("Looping");
}

void loop() {

  for(int i = 8; i <= 11; i++) {

    if (!mcp.digitalRead(i)) {
      Serial.print("Pin:");
      Serial.println(i);
    }
  }

}

