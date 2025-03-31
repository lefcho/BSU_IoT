const int Min = 0;
const int Max = 650;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int Light = analogRead(0);
  int range = map(Light, Min, Max, 0, 3);

  switch(range)
  {
    case 0:
        Serial.println("dark");
        Serial.println(Light);
        break;
    case 1:
        Serial.println("dim");
        Serial.println(Light);
        break;
    case 2:
        Serial.println("medium");
        Serial.println(Light);
        break;
    case 3:
        Serial.println("bright");
        Serial.println(Light);
        break;
  }
  delay(500);
}
