#include <WiFi.h>
#include <WebSocketsClient.h>

/*====== WiFi настройки ======*/
#define ssid "TEST"    // имя WiFi-сети, поднятой внешней ESP32
#define password NULL  // пароль, если точка без пароля — оставь пустым

/*====== WebSocket настройки ======*/
#define host "192.168.4.1"  // IP внешней esp32
#define port 81       // порт WebSocket-сервера

WebSocketsClient webSocket;

#define LED_PIN 2  // встроенный светодиод


void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  delay(200);

  // ====== Подключение к WiFi ======
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("WiFi is connected");
  Serial.print("IP client's: ");
  Serial.println(WiFi.localIP());

  // ====== Настройка WebSocket-клиента ======
  webSocket.begin(host, port, "/");
  webSocket.onEvent([](WStype_t type, uint8_t *payload, size_t length) { /*====== Функция обработки входящих сообщений ======*/
    switch (type) {
      case WStype_CONNECTED:
        Serial.println("[WebSocket] connected!");
        digitalWrite(LED_PIN, HIGH);
        break;

      case WStype_TEXT:
        Serial.print("[WebSocket] Message received: ");
        Serial.println((char*)payload);
        break;

      case WStype_DISCONNECTED:
        Serial.println("[WebSocket] disconnected!");
        digitalWrite(LED_PIN, LOW);
        break;
    }
  });
  webSocket.setReconnectInterval(2000);  // каждые 2 секунд пытаться переподключиться

  // Serial.println("Waiting for messages from the server...");
}

void loop() {
  webSocket.loop();
  delay(1);
}
