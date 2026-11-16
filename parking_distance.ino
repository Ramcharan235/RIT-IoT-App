#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* pass = "YOUR_WIFI_PASSWORD";
ESP8266WebServer server(80);

// Sends JSON with CORS enabled so the app can read it
void sendJson(int code, const String& j) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(code, "application/json", j);
}
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());   // type this IP into the app
}

// HC-SR04: TRIG -> D6 (GPIO12), ECHO -> D7 (GPIO13) through a voltage divider
// Warning LED -> D2 (GPIO4)
#define TRIG_PIN 12
#define ECHO_PIN 13
#define LED_PIN 4

long readCm() {
  digitalWrite(TRIG_PIN, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long us = pulseIn(ECHO_PIN, HIGH, 30000);   // 30 ms timeout
  return us == 0 ? -1 : us / 58;
}
void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  connectWiFi();
  server.on("/status", [](){ sendJson(200, "{\"device\":\"nodemcu\",\"project\":\"parking\"}"); });
  // GET /distance -> {"cm":32,"state":"near"}
  server.on("/distance", [](){
    long cm = readCm();
    const char* s = (cm < 0) ? "none" : (cm < 15) ? "stop" : (cm < 40) ? "near" : "free";
    digitalWrite(LED_PIN, (cm >= 0 && cm < 40) ? HIGH : LOW);
    sendJson(200, "{\"cm\":" + String(cm) + ",\"state\":\"" + s + "\"}");
  });
  server.begin();
}
void loop() { server.handleClient(); }
