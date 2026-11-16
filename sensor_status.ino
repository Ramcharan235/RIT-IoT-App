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
#include <DHT.h>

// DHT11 -> D5 (GPIO14) | LDR divider -> A0 | HC-SR04 TRIG D6 (GPIO12), ECHO D7 (GPIO13)
#define DHT_PIN 14
#define TRIG_PIN 12
#define ECHO_PIN 13
DHT dht(DHT_PIN, DHT11);

long readCm() {
  digitalWrite(TRIG_PIN, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long us = pulseIn(ECHO_PIN, HIGH, 30000);
  return us == 0 ? -1 : us / 58;
}
void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  dht.begin();
  connectWiFi();
  server.on("/status", [](){ sendJson(200, "{\"device\":\"nodemcu\",\"project\":\"sensors\"}"); });
  // GET /sensors -> {"t":..,"h":..,"ldr":..,"cm":..}
  server.on("/sensors", [](){
    float t = dht.readTemperature(), h = dht.readHumidity();
    if (isnan(t)) t = -999;  if (isnan(h)) h = -999;   // -999 = sensor error
    int ldr = analogRead(A0);                          // 0-1023
    sendJson(200, "{\"t\":" + String(t, 1) + ",\"h\":" + String(h, 1) + ",\"ldr\":" + ldr + ",\"cm\":" + readCm() + "}");
  });
  server.begin();
}
void loop() { server.handleClient(); }
