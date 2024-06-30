
// Test program for I2C mcp23017 connectivity

#include <Wire.h>
#include <Adafruit_MCP23X17.h>
// #include <MCP23017.h> // don't use this library, we current use the adafruit lib. 

#define IO_I2C_SDA 20
#define IO_I2C_SCL 21
#define IO_I2C_INT 22 /* Interrupt pin for lower power mode, not tied to any interface */
#define IO_I2C_IF Wire


Adafruit_MCP23X17 mcp;


void setup() {

    Serial.begin(115200);
    delay(4000);

    Serial.println("Starting MCP23017 Test");

    IO_I2C_IF.setSDA(IO_I2C_SDA);
    IO_I2C_IF.setSCL(IO_I2C_SCL);

    IO_I2C_IF.begin();

    if (!mcp.begin_I2C(MCP23XXX_ADDR, &IO_I2C_IF)) {
      Serial.println("Error init mcp");
      while(1);
    }

    
    // mcp pins 8,9,10,11 are connected to the button switches
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

