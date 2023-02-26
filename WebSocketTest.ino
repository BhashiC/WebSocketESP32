#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "Zone_51_2.4G";
const char* password = "LaNaLaN324";

WebSocketsServer webSocket = WebSocketsServer(81);
int ledPin = 5;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  digitalWrite(ledPin, ledState);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("[%u] Connected!\n", num);
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Received text: %s\n", num, payload);
      if (strcmp((char*)payload, "toggle_led") == 0) {
        ledState = !ledState;
        webSocket.broadcastTXT(String(ledState).c_str());
      }
      break;
    case WStype_BIN:
      Serial.printf("[%u] Received binary data\n", num);
      break;
    default:
      Serial.printf("[%u] Unknown event type\n", num);
      break;
  }
}
