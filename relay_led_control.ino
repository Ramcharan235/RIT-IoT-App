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

// Relay module IN -> D1 (GPIO5), external LED -> D2 (GPIO4)
#define RELAY_PIN 5
#define LED_PIN 4
bool isOn = false;

void apply() {
  digitalWrite(RELAY_PIN, isOn ? LOW : HIGH); // most relay modules are active-LOW
  digitalWrite(LED_PIN, isOn ? HIGH : LOW);
}
void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  apply();
  connectWiFi();
  server.on("/status", [](){ sendJson(200, "{\"device\":\"nodemcu\",\"project\":\"relay\"}"); });
  // GET /relay?state=1 (on) or state=0 (off)
  server.on("/relay", [](){
    isOn = (server.arg("state") == "1");
    apply();
    sendJson(200, String("{\"relay\":") + (isOn ? "true" : "false") + "}");
  });
  server.begin();
}
void loop() { server.handleClient(); }
