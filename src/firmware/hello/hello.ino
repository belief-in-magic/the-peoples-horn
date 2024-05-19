
void setup() {
  
    Serial.begin(115200);
    delay(4000);
    //while (!Serial);
    Serial.println("Hello world test");

    Serial.println("Begin looping");
}

void loop() {

  for(int i = 0; ; i++) {
    delay(1000);
    Serial.print("Hello world: ");
    Serial.println(i);
  }

}

