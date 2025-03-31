// https://www.tinkercad.com/things/fezEL3AWHUP-daring-jarv

const int tsens = 0;
void setup() {
	Serial.begin(9600);
}

void loop() {
	int val = analogRead(tsens);
    Serial.print(val);
    Serial.print(" > ");
    float Volt = val * 5.0/1024;
    float celsius = (Volt - 0.5) * 100;
    Serial.print(celsius);
  	Serial.println(" degrees Celcius");
    delay(200);
}
