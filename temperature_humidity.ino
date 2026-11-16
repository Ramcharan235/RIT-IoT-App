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

// DHT11 DATA -> D5 (GPIO14)
#define DHT_PIN 14
DHT dht(DHT_PIN, DHT11);

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectWiFi();
  server.on("/status", [](){ sendJson(200, "{\"device\":\"nodemcu\",\"project\":\"dht\"}"); });
  // GET /dht -> {"t":28.0,"h":55.0}
  server.on("/dht", [](){
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (isnan(t) || isnan(h)) { sendJson(500, "{\"error\":\"dht_read_failed\"}"); return; }
    sendJson(200, "{\"t\":" + String(t, 1) + ",\"h\":" + String(h, 1) + "}");
  });
  server.begin();
}
void loop() { server.handleClient(); }
