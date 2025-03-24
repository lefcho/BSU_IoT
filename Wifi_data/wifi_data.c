// Пример за AI_IoT A. Toshkov с добавен потенциометър
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Определение на изводите *********************************/
const int LED_1 = 33; // За визуализиране на управлението на нагревателя
const int LED_2 = 25; // За визуализиране на сигнал към реле или друго изходно устройство
const int LED_3 = 26; // За визуализиране на сигнал към реле или друго изходно устройство
const int VoltageAnalogInputPin = 32; // Аналогов вход за потенциометъра

/************************* WiFi точка за достъп *********************************/
#define WLAN_SSID "Wokwi-GUEST"
#define WLAN_PASS "Wokwi-GUEST"

/************************* Настройка на Adafruit.io *********************************/
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883 // използвайте 8883 за SSL
#define IO_USERNAME  "ivandk"
#define IO_KEY       "key"

/************ Глобално състояние (не е необходимо да го променяте!) ******************/
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);

/****************************** Adafruit Feeds ***************************************/
Adafruit_MQTT_Subscribe Out_1 = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/heater");
Adafruit_MQTT_Subscribe Out_2 = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Subscribe Out_3 = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/relay3");
// Новият канал за потенциометъра
Adafruit_MQTT_Publish Potentiometer = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/touchsensor");

/*************************** Код на скицата ************************************/
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Настройка на изводите
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(VoltageAnalogInputPin, INPUT);
  
  Serial.println(F("Adafruit MQTT demo with Potentiometer. // Wait 10 sec."));
  Serial.println();
  Serial.print("Свързване към ");
  Serial.println(WLAN_SSID);

  // Свързване към WiFi
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi свързан");
  Serial.print("IP адрес: ");
  Serial.println(WiFi.localIP());
  
  // Абонаменти към MQTT каналите
  mqtt.subscribe(&Out_1);
  mqtt.subscribe(&Out_2);
  mqtt.subscribe(&Out_3);
}

void loop() {
  MQTT_connect();
  
  // Четене на стойността от потенциометъра
  int potValue = analogRead(VoltageAnalogInputPin);
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);

  // Ако стойността е над прага (20), публикуваме към Adafruit IO
  if (potValue > 20) {
    Serial.println("Potentiometer value exceeded threshold! Publishing to Adafruit IO...");
    // Cast potValue to int32_t to resolve the ambiguous overload error
    if (!Potentiometer.publish((int32_t)potValue)) {
      Serial.println("Failed to publish potentiometer value!");
    } else {
      Serial.println("Potentiometer value published successfully!");
    }
  }

  // Обработка на входящите MQTT съобщения
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Out_1) {
      Serial.print(F("Нагревател (LED1/Relay1) получи: "));
      Serial.println((char *)Out_1.lastread);
      int Out_1_State = atoi((char *)Out_1.lastread);
      digitalWrite(LED_1, Out_1_State ? HIGH : LOW);
    }
    if (subscription == &Out_2) {
      Serial.print(F("Relay2/LED2 получи: "));
      Serial.println((char *)Out_2.lastread);
      int Out_2_State = atoi((char *)Out_2.lastread);
      digitalWrite(LED_2, Out_2_State ? HIGH : LOW);
    }
    if (subscription == &Out_3) {
      Serial.print(F("Relay3/LED3 получи: "));
      Serial.println((char *)Out_3.lastread);
      int Out_3_State = atoi((char *)Out_3.lastread);
      digitalWrite(LED_3, Out_3_State ? HIGH : LOW);
    }
  }

  // Пинг за поддържане на MQTT връзката
  if (!mqtt.ping()) {
    mqtt.disconnect();
  }

  delay(100); // Кратко забавяне, за да не претоварваме MQTT сървъра
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) return;

  Serial.print("Свързване с MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // връща 0 ако е успешно свързан
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      // Ако не успее, спира изпълнението
      while (1);
    }
  }
  Serial.println("MQTT Свързан!");
}
