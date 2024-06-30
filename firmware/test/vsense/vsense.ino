
#define RP2040_VSENSE_PIN 29
#define RP2040_CHARGE_EN_PIN 28

#define ANALOG_READ_RESOLUTION 12

void setup() {

  //pinMode(RP2040_CHARGE_EN_PIN, OUTPUT);
  //digitalWrite(RP2040_CHARGE_EN_PIN, LOW); // set low to enable charging, based on the bq25172 spec

  Serial.begin(115200);
  analogReadResolution(ANALOG_READ_RESOLUTION);

}


float toVoltage(int resolution, float nominalV, int val) {

  return val * (nominalV/  (1 << resolution));
}

void loop() {

  int potValue = analogRead(RP2040_VSENSE_PIN);
  float voltage = toVoltage(ANALOG_READ_RESOLUTION, 3.3, potValue); 
  Serial.print("Voltage: ");
  Serial.println(voltage);

  if (voltage > 2.5) {
    Serial.println("Voltage is pretty high, disabling");
    digitalWrite(RP2040_CHARGE_EN_PIN, HIGH);
  }
  delay(2000);
}
