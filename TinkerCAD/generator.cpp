const int encoderPinA = 4;
const int encoderPinB = 2;
const int encoderStepsPerRevolution = 16;
int angle = 0;
int val;
int encoderPos = 0;
boolean encoderALast = LOW; // запаметяване на предишно състояние

void setup() {
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);
    digitalWrite(encoderPinA, HIGH);
    digitalWrite(encoderPinB, HIGH);
    Serial.begin(9600);
}

void loop() {
    boolean encoderA = digitalRead(encoderPinA);
    if ((encoderALast == HIGH) && (encoderA == LOW)) {
        if (digitalRead(encoderPinB) == LOW) {
            encoderPos--;
        } else {
            encoderPos++;
        }

      angle = (encoderPos %  encoderStepsPerRevolution) * 360/encoderStepsPerRevolution;

      Serial.print(encoderPos);
      Serial.print("    ");
      Serial.print(angle);
      
    }

    encoderALast = encoderA;
}
